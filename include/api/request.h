#pragma once

#include "api/objects/channel.h"
#include "api/objects/guild.h"
#include "api/objects/message.h"
#include "api/objects/client.h"
#include "ui/common/roundedimage.h"

#include <QString>
#include <QVector>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QBoxLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

namespace Api {

typedef std::function<void(void *)> Callback;

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
    RequestParameters operator=(RequestParameters other)
    {
        return RequestParameters {
            other.callback,
            other.url,
            other.postDatas,
            other.customRequest,
            other.fileName,
            other.outputFile,
            other.type,
            other.json
        };
    }

    Callback callback;
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
    void const getGuilds(Callback callback);
    void const getGuildChannels(Callback callback, const Snowflake& id);
    void const getPrivateChannels(Callback callback);
    void const getMessages(Callback callback, const Snowflake& channelId, const Snowflake& beforeId, unsigned int limit);
    void const getClient(Callback callback);
    void const getClientSettings(Callback callback);
    void const getImage(Callback callback, const QString& url, const QString& fileName);
    void const getUser(Callback callback, const Snowflake& userId);
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
    void requestEmit(int requestType, QNetworkRequest request, QByteArray *query, QHttpMultiPart *multiPart);

private slots:
    void doRequest(int requestType, QNetworkRequest request, QByteArray *query, QHttpMultiPart *multiPart);
    void readReply();
    void const writeFile();

private:
    QNetworkAccessManager netManager;
    QNetworkReply *reply;
    QQueue<RequestParameters> requestQueue;     // Queue of request parameters
    QMutex lock;
    QWaitCondition requestWaiter;               // The loop waits when there is no request
    QWaitCondition finishWaiter;                // The loop waits when there is no request
    QThread *loop;                              // Request loop
    QString token;                              // Authorization token
    double rateLimitEnd;                        // Unix time that represents the moment of the end of the rate limit
    unsigned int requestsToCheck;               // The number of requests that we have to check when we need to remove
                                                // callbacks for image requests
    unsigned int currentRequestsNumber;         // The number of requests that are processed at the moment
    bool stopped;                               // Used to stop the request loop

    // The function that contains the request loop
    void RequestLoop();
};

} // namespace Api
