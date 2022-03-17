#include "api/request.h"

#include "api/jsonutils.h"

#include <QJsonDocument>
#include <QDir>
#include <QUrlQuery>
#include <QMimeDatabase>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QHttpPart>
#include <QtNetwork/QNetworkRequest>

#include <cstdlib>
#include <ctime>
#include <cstring>

namespace Api {

Requester::Requester(const QString& tokenp)
    : QObject()
{
    // Attributes initialization
    token = tokenp;
    currentRequestsNumber = 0;
    rateLimitEnd = 0; // No rate limit for now
    requestsToCheck = 0;
    stopped = false;

    // Start the request loop
    loop = QThread::create([this](){RequestLoop();});
    loop->start();

    QObject::connect(this, SIGNAL(requestEmit(int, QNetworkRequest, QByteArray *, QHttpMultiPart *)), this, SLOT(doRequest(int, QNetworkRequest, QByteArray *, QHttpMultiPart *)));
}

Requester::~Requester()
{
    // Stop the request loop
    stopped = true;
    loop->wait();
    requestWaiter.notify_all();
}

void Requester::writeFile()
{
    QFile file(requestQueue.front().outputFile);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    file.write(reply->readAll());
    file.close();
}

void Requester::readReply()
{
    RequestParameters parameters = requestQueue.front();
    requestQueue.pop();
    if (parameters.outputFile != "" && parameters.type == GetImage) {
        QDir dir("cache/");
        if (!dir.exists()) dir.mkpath(".");

        QFile file(parameters.outputFile);
        file.open(QIODevice::WriteOnly);
        file.write(reply->readAll());
        file.close();
    }
    QByteArray ba = reply->readAll();

    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (statusCode.toInt() == 429) { // We are rate limited
        // Set the end of the rate limit
        rateLimitEnd = std::time(0) + QJsonDocument::fromJson(ba)["retry_after"].toDouble();
    } else {
        switch (parameters.type) {
            case GetUser:
                {
                    User *user;
                    unmarshal<User>(QJsonDocument::fromJson(ba).object(), &user);
                    parameters.callback(static_cast<void *>(user));
                    break;
                }
            case GetGuilds:
                {
                    QVector<Guild *> guilds = unmarshalMultiple<Guild>(QJsonDocument::fromJson(ba).array());
                    parameters.callback(static_cast<void *>(&guilds));
                    break;
                }
            case GetGuildChannels:
                {
                    QVector<Channel *> channels = unmarshalMultiple<Channel>(QJsonDocument::fromJson(ba).array());
                    parameters.callback(static_cast<void *>(&channels));
                    break;
                }
            case GetPrivateChannels:
                {
                    QVector<Channel *> privateChannels = unmarshalMultiple<Channel>(QJsonDocument::fromJson(ba).array());
                    parameters.callback(static_cast<void *>(&privateChannels));
                    break;
                }
            case GetMessages:
                {
                    QVector<Message *> messages = unmarshalMultiple<Message>(QJsonDocument::fromJson(ba).array());
                    parameters.callback(static_cast<void *>(&messages));
                    break;
                }
            case GetClient:
                {
                    Client *client;
                    unmarshal<Client>(QJsonDocument::fromJson(ba).object(), &client);
                    parameters.callback(static_cast<void *>(client));
                    break;
                }
            case GetClientSettings:
                {
                    ClientSettings *clientSettings;
                    unmarshal<ClientSettings>(QJsonDocument::fromJson(ba).object(), &clientSettings);
                    parameters.callback(static_cast<void *>(clientSettings));
                    break;
                }
            case GetWsUrl:
                {
                    try {
                        QJsonValue jsonUrl = QJsonDocument::fromJson(ba)["url"];
                        QString url = jsonUrl.toString();
                        parameters.callback(static_cast<void *>(&url));
                    } catch(std::exception&) {
                        break;
                    }
                break;
                }
            case GetImage:
                {
                    QString imageFileName = parameters.outputFile;
                    lock.lock();
                    if (requestsToCheck == 0) parameters.callback(static_cast<void *>(&imageFileName));
                    lock.unlock();
                    break;
                }
        }
        currentRequestsNumber--;

        lock.lock();
        if (requestsToCheck > 0) requestsToCheck--;
        lock.unlock();
    }
    finishWaiter.notify_one();
}

void Requester::RequestLoop()
{
    while (!stopped) {
        if (requestQueue.size() > currentRequestsNumber) {
            do {
                RequestParameters parameters = requestQueue.front();
                currentRequestsNumber++;

                // Very basic rate limit checker
                if (rateLimitEnd != 0) { // We were rate limited
                    double sleepDuration = rateLimitEnd - std::time(0); // Time to sleep

                    // We are currently rate limited
                    if (sleepDuration > 0) QThread::msleep(sleepDuration);
                    rateLimitEnd = 0; // Reset rate limit
                }

                QNetworkRequest request(QUrl(parameters.url));

                /* Set the headers */
                if (parameters.type != GetFile)
                    request.setRawHeader(QByteArray("Authorization"), token.toUtf8());
                if (parameters.json) {
                    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
                }

                if (parameters.customRequest != "") {
                    if (parameters.customRequest == "POST") {
                        emit requestEmit(Post, request, new QByteArray(parameters.postDatas.toUtf8()), nullptr);
                    } else if (parameters.customRequest == "PUT") {
                        emit requestEmit(Put, request, new QByteArray(parameters.postDatas.toUtf8()), nullptr);
                    } else if (parameters.customRequest == "DELETE") {
                        emit requestEmit(Delete, request, nullptr, nullptr);
                    }
                } else if (parameters.fileName != "") {
                    QHttpMultiPart multiPart(QHttpMultiPart::FormDataType);
                    QHttpPart part;

                    part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QMimeDatabase().mimeTypeForFile(parameters.fileName).name()));
                    QFile file(parameters.fileName);
                    part.setBody(file.readAll());

                    multiPart.append(part);
                    emit requestEmit(Post, request, nullptr, &multiPart);
                } else {
                    emit requestEmit(Get, request, nullptr, nullptr);
                }

                std::unique_lock<std::mutex> uniqueLock(lock);
                finishWaiter.wait(uniqueLock);
            } while (!requestQueue.empty() && requestQueue.size() > currentRequestsNumber);
        } else {
            std::unique_lock<std::mutex> uniqueLock(lock);
            requestWaiter.wait(uniqueLock);
        }
    }
}

void Requester::doRequest(int requestType, QNetworkRequest request, QByteArray *postDatas, QHttpMultiPart *multiPart)
{
    switch (requestType) {
        case Get:
            reply = netManager.get(request);
            break;
        case Post:
            if (multiPart != nullptr) {
                reply = netManager.post(request, multiPart);
            } else {
                reply = netManager.post(request, *postDatas);
            }
            break;
        case Put:
            reply = netManager.put(request, *postDatas);
            break;
        case Delete:
            reply = netManager.deleteResource(request);
            break;
    }

    // Connect the finished and read signals to process the reply
    QObject::connect(reply, SIGNAL(finished()), this, SLOT(readReply()));
    if (requestQueue.front().type == GetFile)
        QObject::connect(reply, SIGNAL(readyRead()), this, SLOT(writeFile()));
}

void Requester::requestApi(const RequestParameters &parameters)
{
    lock.lock();
    requestQueue.push(parameters);
    requestWaiter.notify_one();
    lock.unlock();
}

void Requester::removeImageRequests()
{
    lock.lock();
    requestsToCheck = currentRequestsNumber;
    for (unsigned int i = requestQueue.size() ; i > 0 ; i--) {
        RequestParameters temp = requestQueue.front();
        requestQueue.pop();
        if (temp.type != GetImage) requestQueue.push(temp);
    }
    lock.unlock();
}

// Functions that request the API to retrieve data

void Requester::getImage(std::function<void(void *)> callback, const QString& url, const QString& fileName)
{
    requestApi({
        callback,
        url,
        "",
        "",
        "",
        "cache/" + fileName,
        GetImage,
        false});
}

void Requester::getPrivateChannels(std::function<void(void *)> callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/channels",
        "",        
        "",
        "",
        "",
        GetPrivateChannels,
        false});
}

void Requester::getGuilds(std::function<void(void *)> callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/guilds",
        "",        
        "",
        "",
        "",
        GetGuilds,
        false});
}

void Requester::getGuildChannels(std::function<void(void *)> callback, const Snowflake& id)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/guilds/" + id.toString() + "/channels",
        "",        
        "",
        "",
        "",
        GetGuildChannels,
        false});
}

void Requester::getMessages(std::function<void(void *)> callback, const Snowflake& channelId, const Snowflake& beforeId, unsigned int limit)
{
    limit = limit >= 50 ? 50 : limit;

    requestApi({
        callback,
        "https://discord.com/api/v9/channels/" + channelId.toString() + "/messages?" + (beforeId != 0 ? "before=" + beforeId.toString() + "&" : "" ) + "limit=" + QString::number(limit),
        "",        
        "",
        "",
        "",
        GetMessages,
        false});
}

void Requester::getClient(std::function<void(void *)> callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me",
        "",        
        "",
        "",
        "",
        GetClient,
        false});
}

void Requester::getClientSettings(std::function<void(void *)> callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/settings",
        "",        
        "",
        "",
        "",
        GetClientSettings,
        false});
}

void Requester::getUser(std::function<void(void *)> callback, const Snowflake& userId)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/" + userId.toString(),
        "",
        "",
        "",
        "",
        GetUser,
        false});
}

// Functions that request the API to send data

void Requester::setStatus(const QString& status)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/users/@me/settings",
        "status:" + status,
        "PATCH",
        "",
        "",
        SetStatus,
        false});
}

void Requester::sendTyping(const Snowflake& channelId)
{
    requestApi({
            nullptr,
            "https://discord.com/api/v9/channels/" + channelId.toString() + "/typing",
            "",
            "POST",
            "",
            "",
            SendTyping,
            false});
}

void Requester::sendMessage(const QString& content, const Snowflake& channelId)
{
    requestApi({
            nullptr,
            "https://discord.com/api/v9/channels/" + channelId.toString() + "/messages",
            "{\"content\":\"" + content + "\"}",
            "POST",
            "",
            "",
            SendMessage,
            true});
}

void Requester::sendMessageWithFile(const QString& content, const Snowflake& channelId, const QString& filePath)
{
    requestApi({
            nullptr,
            "https://discord.com/api/v9/channels/" + channelId.toString() + "/messages",
            "{\"content\":\"" + content + "\"}",
            "POST",
            filePath,
            "",
            SendMessageWithFile,
            false});
}

void Requester::deleteMessage(const Snowflake& channelId, const Snowflake& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId.toString() + "/messages/" + messageId.toString(),
        "",
        "DELETE",
        "",
        "",
        DeleteMessage,
        false});
}

void Requester::pinMessage(const Snowflake& channelId, const Snowflake& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId.toString() + "/pins/" + messageId.toString(),
        "",
        "PUT",
        "",
        "",
        PinMessage,
        false});
}

void Requester::unpinMessage(const Snowflake& channelId, const Snowflake& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId.toString() + "/pins/" + messageId.toString(),
        "",
        "PUT",
        "",
        "",
        UnpinMessage,
        false});
}

void Requester::getFile(const QString& url, const QString& filename)
{
    requestApi({
        nullptr,
        url,
        "",
        "",
        "",
        filename,
        GetFile,
        false});
}

} // namespace Api
