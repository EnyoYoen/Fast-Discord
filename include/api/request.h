#pragma once

#include "channel.h"
#include "guild.h"
#include "message.h"
#include "client.h"
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
    std::function<void(void *)> callback;
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
    void getGuilds(std::function<void(void *)> callback);
    void getGuildChannels(std::function<void(void *)> callback, const Snowflake& id);
    void getPrivateChannels(std::function<void(void *)> callback);
    void getMessages(std::function<void(void *)> callback, const Snowflake& channelId, const Snowflake& beforeId, unsigned int limit);
    void getClient(std::function<void(void *)> callback);
    void getClientSettings(std::function<void(void *)> callback);
    void getImage(std::function<void(void *)> callback, const QString& url, const QString& fileName);
    void getUser(std::function<void(void *)> callback, const Snowflake& userId);
    void getFile(const QString& url, const QString& filename);

    // Functions that request the API to send data
    void setStatus(const QString& status);
    void sendTyping(const Snowflake& channelId);
    void sendMessage(const QString& content, const Snowflake& channelId);
    void sendMessageWithFile(const QString& content, const Snowflake& channelId, const QString& filePath);
    void deleteMessage(const Snowflake& channelId, const Snowflake& messageId);
    void pinMessage(const Snowflake& channelId, const Snowflake& messageId);
    void unpinMessage(const Snowflake& channelId, const Snowflake& messageId);

signals:
    void requestEmit(int requestType, QNetworkRequest request, QByteArray *query, QHttpMultiPart *multiPart);

private slots:
    void doRequest(int requestType, QNetworkRequest request, QByteArray *query, QHttpMultiPart *multiPart);
    void readReply();
    void writeFile();

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
