#pragma once

#include "api/objects/channel.h"
#include "api/objects/guild.h"
#include "api/objects/message.h"
#include "api/objects/client.h"
#include "ui/roundedimage.h"

#include <QString>
#include <QVector>
#include <QThread>
#include <QBoxLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <queue>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <cstddef>
#include <ostream>

namespace Api {

enum RequestTypes {
    // We need the response data
    GetGuilds,
    GetGuildChannels,
    GetPrivateChannels,
    GetMessages,
    GetClient,
    GetClientSettings,
    GetImage,
    GetWsUrl,
    GetUser,
    GetFile,

    // We don't care about the response
    SetStatus,
    SendTyping,
    SendMessage,
    SendMessageWithFile,
    DeleteMessage,
    PinMessage,
    UnpinMessage
};

struct RequestParameters
{
    const std::function<void(void *)>& callback;
    const QString url;
    const QString postDatas;
    const QString customRequest;
    const QString fileName;
    const QString outputFile;
    int type;
    bool json;
};

enum RequestMethods {
    Get,
    Post,
    Put,
    Delete
};

// Class to request the API
class Requester : public QObject
{
    Q_OBJECT
public:
    Requester(const QString& token);
    ~Requester();

    // Function that request the API
    void requestApi(const RequestParameters& parameters);
    void removeImageRequests();

    // Functions that request the API to retrieve data
    void const getGuilds(const std::function<void(void *)>& callback);
    void const getGuildChannels(const std::function<void(void *)>& callback, const Snowflake& id);
    void const getPrivateChannels(const std::function<void(void *)>& callback);
    void const getMessages(const std::function<void(void *)>& callback, const Snowflake& channelId, const Snowflake& beforeId, unsigned int limit);
    void const getClient(const std::function<void(void *)>& callback);
    void const getClientSettings(const std::function<void(void *)>& callback);
    void const getImage(const std::function<void(void *)>& callback, const QString& url, const QString& fileName);
    void const getUser(const std::function<void(void *)>& callback, const Snowflake& userId);
    void const getFile(const QString& url, const QString& filename);

    // Functions that request the API to send data
    void const setStatus(const QString& status);
    void const sendTyping(const Snowflake& channelId);
    void const sendMessage(const QString& content, const Snowflake& channelId);
    void const sendMessageWithFile(const QString& content, const Snowflake& channelId, const QString& filePath);
    void const deleteMessage(const Snowflake& channelId, const Snowflake& messageId);
    void const pinMessage(const Snowflake& channelId, const Snowflake& messageId);
    void const unpinMessage(const Snowflake& channelId, const Snowflake& messageId);

signals:
    void requestEmit(int requestType, const QNetworkRequest& request, QByteArray *query, QHttpMultiPart *multiPart);

private slots:
    void doRequest(int requestType, const QNetworkRequest& request, const QByteArray *query, QHttpMultiPart *multiPart);
    void readReply();
    void const writeFile();

private:
    QNetworkAccessManager netManager;
    QNetworkReply *reply;
    std::queue<RequestParameters> requestQueue; // Queue of request parameters
    std::mutex lock;
    std::condition_variable requestWaiter;      // The loop waits when there is no request
    std::condition_variable finishWaiter;       // The loop waits when there is no request
    QThread *loop;                              // Request loop
    QString token;                          // Authorization token
    double rateLimitEnd;                        // Unix time that represents the moment of the end of the rate limit
    unsigned int requestsToCheck;               // The number of requests that we have to check when we need to remove
                                                // callbacks for image requests
    unsigned int currentRequestsNumber;         // The number of requests that are processed at the moment
    bool stopped;                               // Used to stop the request loop

    // The function that contains the request loop
    void RequestLoop();
};

} // namespace Api
