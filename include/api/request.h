#pragma once

#include "channel.h"
#include "guild.h"
#include "message.h"
#include "client.h"
#include "ui/roundedimage.h"

#include <QThread>
#include <QBoxLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <string>
#include <vector>
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
    const std::string url;
    const std::string postDatas;
    const std::string customRequest;
    const std::string fileName;
    const std::string outputFile;
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
    Requester(const std::string& token);
    ~Requester();

    // Function that request the API
    void requestApi(const RequestParameters& parameters);
    void removeImageRequestCallbacks();

    // Functions that request the API to retrieve data
    void getGuilds(std::function<void(void *)> callback);
    void getGuildChannels(std::function<void(void *)> callback, const std::string& id);
    void getPrivateChannels(std::function<void(void *)> callback);
    void getMessages(std::function<void(void *)> callback, const std::string& channelId, const std::string& beforeId, unsigned int limit);
    void getClient(std::function<void(void *)> callback);
    void getClientSettings(std::function<void(void *)> callback);
    void getImage(std::function<void(void *)> callback, const std::string& url, const std::string& fileName);
    void getUser(std::function<void(void *)> callback, const std::string& userId);

    // Functions that request the API to send data
    void setStatus(const std::string& status);
    void sendTyping(const std::string& channelId);
    void sendMessage(const std::string& content, const std::string& channelId);
    void sendMessageWithFile(const std::string& content, const std::string& channelId, const std::string& filePath);
    void deleteMessage(const std::string& channelId, const std::string& messageId);
    void pinMessage(const std::string& channelId, const std::string& messageId);
    void unpinMessage(const std::string& channelId, const std::string& messageId);

signals:
    void requestEmit(int requestType, QNetworkRequest request, QByteArray *query, QHttpMultiPart *multiPart);

private slots:
    void doRequest(int requestType, QNetworkRequest request, QByteArray *query, QHttpMultiPart *multiPart);
    void readReply();

private:
    QNetworkAccessManager netManager;
    QNetworkReply *reply;
    std::queue<RequestParameters> requestQueue; // Queue of request parameters
    std::mutex lock;
    std::condition_variable requestWaiter;      // The loop waits when there is no request
    std::condition_variable finishWaiter;       // The loop waits when there is no request
    QThread *loop;                              // Request loop
    std::string token;                          // Authorization token
    double rateLimitEnd;                        // Unix time that represents the moment of the end of the rate limit
    unsigned int requestsToCheck;               // The number of requests that we have to check when we need to remove
                                                // callbacks for image requests
    unsigned int currentRequestsNumber;         // The number of requests that are processed at the moment
    bool stopped;                               // Used to stop the request loop

    // The function that contains the request loop
    void RequestLoop();
};

} // namespace Api
