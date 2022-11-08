#include "api/request.h"

#include "api/jsonutils.h"
#include "api/ressourcemanager.h"
#include "api/objects/connection.h"

#include <QJsonDocument>
#include <QDir>
#include <QUrlQuery>
#include <QMimeDatabase>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QHttpPart>
#include <QtNetwork/QNetworkRequest>

namespace Api {

Requester::Requester(const QString& tokenp, QObject *parent)
    : QObject(parent)
{
    // Attributes initialization
    token = tokenp;
    currentRequestsNumber = 0;
    rateLimitEnd = 0;
    stopped = false;

    // Start the request loop
    loop = QThread::create([this](){RequestLoop();});
    loop->start();

    connect(this, SIGNAL(requestEmit(int,QNetworkRequest,QByteArray*,QHttpMultiPart*)), this, SLOT(doRequest(int,QNetworkRequest,QByteArray*,QHttpMultiPart*)));
}

Requester::~Requester()
{
    // Stop the request loop
    stopped = true;
    disconnect(reply, &QNetworkReply::finished, this, &Requester::readReply);
    reply->deleteLater();
    finishWaiter.wakeAll();
    requestWaiter.wakeAll();
    loop->wait();
    loop->deleteLater();
}

void const Requester::writeFile()
{
    QFile file(requestQueue.front().outputFile);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    file.write(reply->readAll());
    file.close();
}

void Requester::readReply()
{
    currentRequestsNumber--;
    
    RequestParametersNoCb parameters = requestQueue.dequeue();
    
    bool returnAfter = false;
    if (typesToCheck.contains((RequestTypes)parameters.type))
        returnAfter = true;
    if (urlsTocheck.contains(parameters.url))
        returnAfter = true;

    if (urlsTocheck.size() > 0) {
        QList<QString> urls = urlsTocheck.keys();
        int size = urlsTocheck.size();
        for (int i = 0 ; i < size ; i++) {
            urlsTocheck[urls[i]]--;
            if (urlsTocheck[urls[i]] <= 0)
                urlsTocheck.remove(urls[i]);
        }
    }
    if (typesToCheck.size() > 0) {
        QList<Api::RequestTypes> types = typesToCheck.keys();
        int size = typesToCheck.size();
        for (int i = 0 ; i < size ; i++) {
            typesToCheck[types[i]]--;
            if (typesToCheck[types[i]] <= 0)
                typesToCheck.remove(types[i]);
        }
    }
    if (returnAfter) {
        finishWaiter.wakeOne();
        return;
    }

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
    if (parameters.type == AddReaction || parameters.type == DeleteReaction) qDebug() << ba << statusCode.toInt();
    if (statusCode.toInt() == 401) {
        emit invalidToken();
    } else if (statusCode.toInt() == 429) { // We are rate limited
        // Set the end of the rate limit
        rateLimitEnd = QJsonDocument::fromJson(ba)["retry_after"].toDouble();
    } else {
        switch (parameters.type) {
            case GetUser:
                {
                    User *user;
                    unmarshal<User>(QJsonDocument::fromJson(ba).object(), &user);
                    callCallbacks(parameters, reinterpret_cast<void *>(user));
                    break;
                }
            case GetGuilds:
                {
                    QVector<Guild *> guilds = unmarshalMultiple<Guild>(QJsonDocument::fromJson(ba).array());
                    callCallbacks(parameters, reinterpret_cast<void *>(&guilds));
                    break;
                }
            case GetGuildMember:
                {
                    GuildMember *guildMember;
                    unmarshal<GuildMember>(QJsonDocument::fromJson(ba).object(), &guildMember);
                    callCallbacks(parameters, reinterpret_cast<void *>(guildMember));
                    break;
                }
            case GetGuildChannels:
                {
                    QVector<Channel *> channels = unmarshalMultiple<Channel>(QJsonDocument::fromJson(ba).array());
                    callCallbacks(parameters, reinterpret_cast<void *>(&channels));
                    break;
                }
            case GetPrivateChannels:
                {
                    QVector<Channel *> privateChannels = unmarshalMultiple<Channel>(QJsonDocument::fromJson(ba).array());
                    callCallbacks(parameters, reinterpret_cast<void *>(&privateChannels));
                    break;
                }
            case GetMessages:
                {
                    QVector<Message *> messages = unmarshalMultiple<Message>(QJsonDocument::fromJson(ba).array());
                    callCallbacks(parameters, reinterpret_cast<void *>(&messages));
                    break;
                }
            case GetClient:
                {
                    Client *client;
                    unmarshal<Client>(QJsonDocument::fromJson(ba).object(), &client);
                    callCallbacks(parameters, reinterpret_cast<void *>(client));
                    break;
                }
            case GetClientSettings:
                {
                    ClientSettings *clientSettings;
                    unmarshal<ClientSettings>(QJsonDocument::fromJson(ba).object(), &clientSettings);
                    callCallbacks(parameters, reinterpret_cast<void *>(clientSettings));
                    break;
                }
            case GetWsUrl:
                {
                    QJsonValue jsonUrl = QJsonDocument::fromJson(ba)["url"];
                    QString url = jsonUrl.toString();
                    callCallbacks(parameters, reinterpret_cast<void *>(&url));
                    break;
                }
            case GetImage:
                {
                    QString imageFileName = parameters.outputFile;
                    callCallbacks(parameters, reinterpret_cast<void *>(&imageFileName));
                    break;
                }
            case ChangeUsername:
                {
                    QJsonDocument doc = QJsonDocument::fromJson(ba);
                    if (doc["errors"].isUndefined()) {
                        Client *client;
                        unmarshal<Client>(doc.object(), &client);
                        reinterpret_cast<RessourceManager *>(parent())->setClient(client);
                        reinterpret_cast<RessourceManager *>(parent())->setToken(doc.object()["token"].toString());
                        callCallbacks(parameters, nullptr);
                    } else {
                        QVector<Error *> errors;
                        Error *error;
                        QJsonObject errorsJson = doc["errors"].toObject();
                        if (!errorsJson["discriminator"].isNull()) {
                            unmarshal<Error>(errorsJson["discriminator"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 0;
                            errors.append(new Error(*error));
                        }
                        if (!errorsJson["username"].isNull()) {
                            unmarshal<Error>(errorsJson["username"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 1;
                            errors.append(new Error(*error));
                        }
                        if (!errorsJson["password"].isNull()) {
                            unmarshal<Error>(errorsJson["password"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 2;
                            errors.append(new Error(*error));
                        }                            
                        callCallbacks(parameters, reinterpret_cast<void *>(&errors));
                    }
                    break;
                }
            case ChangeEmail:
                {
                    QJsonDocument doc = QJsonDocument::fromJson(ba);
                    if (doc["errors"].isUndefined()) {
                        Client *client;
                        unmarshal<Client>(doc.object(), &client);
                        reinterpret_cast<RessourceManager *>(parent())->setClient(client);
                        reinterpret_cast<RessourceManager *>(parent())->setToken(doc.object()["token"].toString());
                        callCallbacks(parameters, nullptr);
                    } else {
                        QVector<Error *> errors;
                        Error *error;
                        QJsonObject errorsJson = doc["errors"].toObject();
                        if (!errorsJson["email"].isNull()) {
                            unmarshal<Error>(errorsJson["email"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 0;
                            errors.append(new Error(*error));
                        }
                        if (!errorsJson["password"].isNull()) {
                            unmarshal<Error>(errorsJson["password"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 1;
                            errors.append(new Error(*error));
                        }                            
                        callCallbacks(parameters, reinterpret_cast<void *>(&errors));
                    }
                    break;
                }
            case ChangePassword:
                {
                    QJsonDocument doc = QJsonDocument::fromJson(ba);
                    if (doc["errors"].isUndefined()) {
                        Client *client;
                        unmarshal<Client>(doc.object(), &client);
                        reinterpret_cast<RessourceManager *>(parent())->setClient(client);
                        reinterpret_cast<RessourceManager *>(parent())->setToken(doc.object()["token"].toString());
                        callCallbacks(parameters, nullptr);
                    } else {
                        QVector<Error *> errors;
                        Error *error;
                        QJsonObject errorsJson = doc["errors"].toObject();
                        if (!errorsJson["password"].isNull()) {
                            unmarshal<Error>(errorsJson["password"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 0;
                            errors.append(new Error(*error));
                        }   
                        if (!errorsJson["new_password"].isNull()) {
                            unmarshal<Error>(errorsJson["new_password"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 1;
                            errors.append(new Error(*error));
                        }                         
                        callCallbacks(parameters, reinterpret_cast<void *>(&errors));
                    }
                    break;
                }
            case SendVerifyCode:
                {
                    QJsonDocument doc = QJsonDocument::fromJson(ba);
                    if (doc["errors"].isUndefined()) {
                        emailToken = doc["token"].toString();
                        callCallbacks(parameters, nullptr);
                    } else {
                        Error *error;
                        unmarshal<Error>(doc["errors"].toObject()["code"].toObject()["_errors"].toArray()[0].toObject(), &error);
                        callCallbacks(parameters, error);
                    }
                    break;
                }
            case DisableAccount:
            case DeleteAccount:
                {
                    QJsonDocument doc = QJsonDocument::fromJson(ba);
                    if (doc["message"].isUndefined()) {
                        callCallbacks(parameters, nullptr);
                    } else {
                        Error *error = new Error {
                            doc["message"].toString(),
                            doc["code"].toString(),
                            doc["code"].toInt(),
                        };
                        callCallbacks(parameters, error);
                    }
                    break;
                }
            case ChangeClient:
                {
                    QJsonDocument doc = QJsonDocument::fromJson(ba);
                    if (doc["errors"].isUndefined()) {
                        Client *client;
                        unmarshal<Client>(doc.object(), &client);
                        reinterpret_cast<RessourceManager *>(parent())->setClient(client);
                        reinterpret_cast<RessourceManager *>(parent())->setToken(doc.object()["token"].toString());
                        callCallbacks(parameters, nullptr);
                    } else {
                        QVector<Error *> errors;
                        Error *error;
                        QJsonObject errorsJson = doc["errors"].toObject();
                        if (!errorsJson["avatar"].isNull()) {
                            unmarshal<Error>(errorsJson["avatar"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 0;
                            errors.append(new Error(*error));
                        }
                        if (!errorsJson["accent_color"].isNull()) {
                            unmarshal<Error>(errorsJson["accent_color"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 1;
                            errors.append(new Error(*error));
                        }
                        if (!errorsJson["banner"].isNull()) {
                            unmarshal<Error>(errorsJson["banner"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 2;
                            errors.append(new Error(*error));
                        }
                        if (!errorsJson["bio"].isNull()) {
                            unmarshal<Error>(errorsJson["bio"].toObject()["_errors"].toArray()[0].toObject(), &error);
                            error->intCode = 3;
                            errors.append(new Error(*error));
                        }
                        callCallbacks(parameters, reinterpret_cast<void *>(&errors));
                    }
                    break;
                }
            case GetConsent:
            case GetHarvest:
                {
                    callCallbacks(parameters, reinterpret_cast<void *>(new QJsonDocument(QJsonDocument::fromJson(ba))));
                    break;
                }
            case GetAuthorizedApp:
                {
                    QVector<AuthorizedApp *> apps = unmarshalMultiple<AuthorizedApp>(QJsonDocument::fromJson(ba).array());
                    callCallbacks(parameters, reinterpret_cast<void *>(&apps));
                    break;
                }
            case GetConnections:
                {
                    QVector<Connection *> connections = unmarshalMultiple<Connection>(QJsonDocument::fromJson(ba).array());
                    callCallbacks(parameters, reinterpret_cast<void *>(&connections));
                    break;
                }
        }
    }
    finishWaiter.wakeOne();
}

void Requester::RequestLoop()
{
    while (!stopped) {
        if (requestQueue.size() > currentRequestsNumber) {
            do {
                if (requestQueue.isEmpty()) break;
                RequestParametersNoCb parameters = requestQueue.front();
                currentRequestsNumber++;

                if (rateLimitEnd > 0) {
                    // We are currently rate limited
                    QThread::msleep(rateLimitEnd);
                    rateLimitEnd = 0; // Reset rate limit
                }

                QNetworkRequest request(QUrl(parameters.url));

                /* Set the headers */
                if (parameters.type != GetFile)
                    request.setRawHeader(QByteArray("Authorization"), token.toUtf8());
                if (parameters.json)
                    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                if (!parameters.fileName.isEmpty()) {
                    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
                    QHttpPart part;

                    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"payload_json\""));
                    part.setBody(parameters.postDatas.toUtf8());
                    multiPart->append(part);

                    QString fileName = parameters.fileName;
                    part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"files[0]\"; filename=\"" + fileName.right(fileName.size() - fileName.lastIndexOf("/")) + "\""));
                    part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QMimeDatabase().mimeTypeForFile(parameters.fileName).name()));
                    QFile file(parameters.fileName);
                    file.open(QIODevice::ReadOnly);
                    part.setBody(file.readAll());
                    file.close();
                    multiPart->append(part);

                    emit requestEmit(Post, request, nullptr, multiPart);
                } else if (!parameters.customRequest.isEmpty()) {
                    if (parameters.customRequest == "POST") {
                        emit requestEmit(Post, request, new QByteArray(parameters.postDatas.toUtf8()), nullptr);
                    } else if (parameters.customRequest == "PUT") {
                        emit requestEmit(Put, request, new QByteArray(parameters.postDatas.toUtf8()), nullptr);
                    } else if (parameters.customRequest == "DELETE") {
                        emit requestEmit(Delete, request, nullptr, nullptr);
                    } else if (parameters.customRequest == "PATCH") {
                        emit requestEmit(Patch, request, new QByteArray(parameters.postDatas.toUtf8()), nullptr);
                    }
                } else {
                    emit requestEmit(Get, request, nullptr, nullptr);
                }

                finishLock.lock();
                finishWaiter.wait(&finishLock);
                finishLock.unlock();
            } while (!requestQueue.empty() && requestQueue.size() > currentRequestsNumber);
        } else {
            requestLock.lock();
            requestWaiter.wait(&requestLock);
            requestLock.unlock();
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
        case Patch:
            reply = netManager.sendCustomRequest(request, "PATCH", (postDatas == nullptr ? QByteArray() : *postDatas));
            break;
    }

    connect(reply, &QNetworkReply::finished, this, &Requester::readReply);
    if (requestQueue.front().type == GetFile)
        connect(reply, SIGNAL(readyRead()), this, SLOT(writeFile()));
}

void Requester::requestApi(const RequestParameters &parameters)
{
    RequestParametersNoCb newParams{
        parameters.url,
        parameters.postDatas,
        parameters.customRequest,
        parameters.fileName,
        parameters.outputFile,
        parameters.type,
        parameters.json,
    };
    if (!requestsCallbacks.contains(newParams)) {
        requestQueue.enqueue(newParams);
    }
    requestsCallbacks[newParams].append(parameters.callback);
    requestWaiter.wakeAll();
}

void Requester::callCallbacks(const RequestParametersNoCb& parameters, void *data)
{
    QVector<Callback> callbacks = requestsCallbacks[parameters];
    requestsCallbacks.remove(parameters);
    for (int i = 0 ; i < callbacks.size() ; i++) {
        callbacks[i](data);
    }
}

void Requester::removeRequests(RequestTypes type)
{
    if (currentRequestsNumber != 0)
        typesToCheck[type] = currentRequestsNumber;
    for (int i = requestQueue.size() ; i > currentRequestsNumber ; i--) {
        RequestParametersNoCb temp = requestQueue.dequeue();
        if (temp.type != type) requestQueue.enqueue(temp);
    }
}

void Requester::removeRequestWithUrl(const QString& url)
{
    if (currentRequestsNumber != 0)
        urlsTocheck[url] = currentRequestsNumber;
    for (int i = requestQueue.size() ; i > currentRequestsNumber ; i--) {
        RequestParametersNoCb temp = requestQueue.dequeue();
        if (temp.url != url) requestQueue.enqueue(temp);
    }
}

// Functions that request the API to retrieve data

void const Requester::getImage(Callback callback, const QString& url, const QString& fileName)
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

void const Requester::getPrivateChannels(Callback callback)
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

void const Requester::getGuilds(Callback callback)
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
void const Requester::getGuildMember(Callback callback, const Snowflake& guildId, const Snowflake& userId)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/guilds/" + guildId + "/members/" + userId,
        "",        
        "",
        "",
        "",
        GetGuildMember,
        false});
}

void const Requester::getGuildChannels(Callback callback, const Snowflake& id)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/guilds/" + id + "/channels",
        "",        
        "",
        "",
        "",
        GetGuildChannels,
        false});
}

void const Requester::getMessages(Callback callback, const Snowflake& channelId, const Snowflake& beforeId, unsigned int limit)
{
    limit = limit > 50 ? 50 : limit;

    requestApi({
        callback,
        "https://discord.com/api/v9/channels/" + channelId + "/messages?" + (beforeId != 0 ? "before=" + beforeId + "&" : "" ) + "limit=" + QString::number(limit),
        "",        
        "",
        "",
        "",
        GetMessages,
        false});
}

void const Requester::getClient(Callback callback)
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

void const Requester::getClientSettings(Callback callback)
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

void const Requester::getWsUrl(Callback callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/gateway",
        "",
        "",
        "",
        "",
        GetWsUrl,
        false
    });
}

void const Requester::getUser(Callback callback, const Snowflake& userId)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/" + userId,
        "",
        "",
        "",
        "",
        GetUser,
        false});
}

void const Requester::getAuthorizedApp(Callback callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/oauth2/tokens",
        "",
        "",
        "",
        "",
        GetAuthorizedApp,
        false});
}

void const Requester::getConsent(Callback callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/consent",
        "",
        "",
        "",
        "",
        GetConsent,
        false});
}

void const Requester::getHarvest(Callback callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/harvest",
        "",
        "",
        "",
        "",
        GetHarvest,
        false});
}

void const Requester::getConnections(Callback callback)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/connections",
        "",
        "",
        "",
        "",
        GetConnections,
        false});
}

// Functions that request the API to send data

void const Requester::setStatus(Callback callback, const QString& status)
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

void const Requester::sendTyping(Callback callback, const Snowflake& channelId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/typing",
        "",
        "POST",
        "",
        "",
        SendTyping,
        false});
}

void const Requester::sendMessage(Callback callback, const QString& content, const Snowflake& channelId)
{
    const_cast<QString&>(content).replace('\n', "\\n").replace('\t', "\\t").replace('\r', "\\r")
        .replace('\v', "\\v").replace('\f', "\\f").replace('\b', "\\b").replace('\a', "\\a");
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/messages",
        "{\"content\":\"" + content + "\"}",
        "POST",
        "",
        "",
        SendMessage,
        true});
}

void const Requester::sendMessageWithFile(Callback callback, const QString& content, const Snowflake& channelId, const QString& filePath)
{
    const_cast<QString&>(content).replace('\n', "\\n").replace('\t', "\\t").replace('\r', "\\r")
        .replace('\v', "\\v").replace('\f', "\\f").replace('\b', "\\b").replace('\a', "\\a");
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/messages",
        "{\"content\":\"" + content + "\"}",
        "POST",
        filePath,
        "",
        SendMessageWithFile,
        false});
}

void const Requester::deleteMessage(Callback callback, const Snowflake& channelId, const Snowflake& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/messages/" + messageId,
        "",
        "DELETE",
        "",
        "",
        DeleteMessage,
        false});
}

void const Requester::pinMessage(Callback callback, const Snowflake& channelId, const Snowflake& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/pins/" + messageId,
        "",
        "PUT",
        "",
        "",
        PinMessage,
        false});
}

void const Requester::unpinMessage(Callback callback, const Snowflake& channelId, const Snowflake& messageId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/channels/" + channelId + "/pins/" + messageId,
        "",
        "PUT",
        "",
        "",
        UnpinMessage,
        false});
}

void const Requester::getFile(Callback callback, const QString& url, const QString& filename)
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

void const Requester::changeUsername(Callback callback, const QString& username, const QString& discriminator, const QString& password)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me",
        "{\"username\":\"" + username + "\",\"password\":\"" + password + "\",\"discriminator\":\"" + discriminator + "\"}",
        "PATCH",
        "",
        "",
        ChangeUsername,
        true});
}

void const Requester::changeEmail(Callback callback, QString email, QString password)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me",
        "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"email_token\":\"" + emailToken + "\"}",
        "PATCH",
        "",
        "",
        ChangeEmail,
        true});
    emailToken.clear();
}

void const Requester::changePassword(Callback callback, QString oldPassword, QString newPassword)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me",
        "{\"new_password\":\"" + newPassword + "\",\"password\":\"" + oldPassword + "\"}",
        "PATCH",
        "",
        "",
        ChangePassword,
        true});
}

void const Requester::removePhone(Callback callback, QString password)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/phone",
        "{\"change_phone_reason\":\"user_settings_update\",\"password\":\"" + password + "\",\"email_token\":\"" + emailToken + "\"}",
        "DELETE",
        "",
        "",
        RemovePhone,
        true});
}

void const Requester::sendVerificationEmail(Callback callback)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/users/@me/email",
        "",
        "PUT",
        "",
        "",
        SendVerificationEmail,
        false});
}

void const Requester::sendVerifyCode(Callback callback, QString verifyCode)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/email/verify-code",
        "{\"code\":\"" + verifyCode + "\"}",
        "POST",
        "",
        "",
        SendVerifyCode,
        true});
}

void const Requester::disableAccount(Callback callback, QString password)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/disable",
        "{\"password\":\"" + password + "\"}",
        "POST",
        "",
        "",
        DisableAccount,
        true});
}

void const Requester::deleteAccount(Callback callback, QString password)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/delete",
        "{\"password\":\"" + password + "\"}",
        "POST",
        "",
        "",
        DeleteAccount,
        true});
}

void const Requester::deleteAuthorizedApp(Callback callback, const Snowflake& appId)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/oauth2/tokens/" + appId,
        "",
        "DELETE",
        "",
        "",
        DeleteAuthorizedApp,
        false});
}

void const Requester::changeClient(Callback callback, QString json)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me",
        json,
        "PATCH",
        "",
        "",
        ChangeClient,
        true});
}

void const Requester::harvestData(Callback callback)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/users/@me/harvest",
        "POST",
        "",
        "",
        "",
        HarvestData,
        false});
}

void const Requester::setSettings(Callback callback, QString settings)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/settings",
        settings,
        "PATCH",
        "",
        "",
        SetSettings,
        true});
}

void const Requester::setSettingsProto(Callback callback, QString settings)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/settings-proto/1",
        "{\"settings\":\"" + settings + "\"}",
        "PATCH",
        "",
        "",
        SetSettingsProto,
        true});
}

void const Requester::setConsent(Callback callback, QString grant, QString revoke)
{
    requestApi({
        callback,
        "https://discord.com/api/v9/users/@me/consent",
        "{\"grant\":[" + grant + "],\"revoke\":[" + revoke + "]}",
        "POST",
        "",
        "",
        SetConsent,
        true});
}

void const Requester::setConnection(Callback callback, QString type, QString id, int visibility)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/users/@me/connections/" + type + "/" + id,
        "{\"visibility\":" + QString(visibility ? "true" : "false") + "}",
        "PATCH",
        "",
        "",
        SetConnection,
        true});
}

void const Requester::removeConnection(Callback callback, QString type, QString id)
{
    requestApi({
        nullptr,
        "https://discord.com/api/v9/users/@me/connections/" + type + "/" + id,
        "",
        "DELETE",
        "",
        "",
        RemoveConnection,
        false});
}

void const Requester::addReaction(Callback callback, const Snowflake& channelId, const Snowflake& messageId, const Snowflake& reactionId, const QString& reactionName)
{
    qDebug() << ("https://discord.com/api/v9/channels/" + channelId + "/messages/" + messageId + "/reactions/" + QUrl::toPercentEncoding(reactionId.value == 0 ? reactionName : (reactionName + ":" + reactionId)) + "/%40me");
    requestApi({
        callback,
        "https://discord.com/api/v9/channels/" + channelId + "/messages/" + messageId + "/reactions/" + QUrl::toPercentEncoding(reactionId.value == 0 ? reactionName : (reactionName + ":" + reactionId)) + "/%40me",
        "",
        "PUT",
        "",
        "",
        AddReaction,
        false});
}

void const Requester::deleteReaction(Callback callback, const Snowflake& channelId, const Snowflake& messageId, const Snowflake& reactionId, const QString& reactionName)
{
    qDebug() << ("https://discord.com/api/v9/channels/" + channelId + "/messages/" + messageId + "/reactions/" + QUrl::toPercentEncoding(reactionId.value == 0 ? reactionName : (reactionName + ":" + reactionId)) + "/%40me");
    requestApi({
        callback,
        "https://discord.com/api/v9/channels/" + channelId + "/messages/" + messageId + "/reactions/" + QUrl::toPercentEncoding(reactionId.value == 0 ? reactionName : (reactionName + ":" + reactionId)) + "/%40me",
        "",
        "DELETE",
        "",
        "",
        DeleteReaction,
        false});
}

} // namespace Api
