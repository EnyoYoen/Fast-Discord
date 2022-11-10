#pragma once

#include "api/objects/channel.h"
#include "api/objects/guild.h"
#include "api/objects/message.h"
#include "api/objects/client.h"
#include "api/objects/error.h"

#include <QString>
#include <QVector>
#include <QQueue>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

namespace Api {

enum RequestTypes {
    // We need the response data
    GetGuilds,
    GetGuildMember,
    GetGuildChannels,
    GetPrivateChannels,
    GetMessages,
    GetClient,
    GetClientSettings,
    GetImage,
    GetWsUrl,
    GetUser,
    GetFile,
    GetConsent,
    GetHarvest,
    GetAuthorizedApp,
    GetConnections,
    ChangeUsername,
    ChangeEmail,
    ChangePassword,
    RemovePhone,
    DisableAccount,
    DeleteAccount,
    ChangeClient,
    SetConsent,
    SetSettings,
    SetSettingsProto,

    // We don't care about the response
    SetStatus,
    SendVerificationEmail,
    SendVerifyCode,
    SendTyping,
    SendMessage,
    SendMessageWithFile,
    DeleteMessage,
    PinMessage,
    UnpinMessage,
    HarvestData,
    DeleteAuthorizedApp,
    SetConnection,
    RemoveConnection,
    AddReaction,
    DeleteReaction
};

struct RequestError
{
    // Type flags
    enum ErrorType {
        None    = 0,
        Http    = 1,
        Discord = 2
    };

    // Should always verify the type of the error before accessing data
    QString message;
    qint32 httpCode; 
    qint32 apiCode;
    qint32 type;

    RequestError() : type(None) {}
    RequestError(QVariant statusCode, QByteArray rawData) {
        type = None;

        int http = statusCode.toInt();
        if (http != 200 && http != 201 && http != 204) {
            type |= Http;
            httpCode = http;
        }

        QJsonObject jsonObj = QJsonDocument::fromJson(rawData).object();
        if (jsonObj.contains("code") && jsonObj.contains("message")) {
            type |= Discord;
            apiCode = jsonObj["code"].toInt();
            message = jsonObj["message"].toString();
        }
    }

    QString str() {
        return QString("RequestError: ")
         + ((type & ErrorType::Http) ? QString::number(httpCode) + " " : QString(" "))
         + ((type & ErrorType::Discord) ? QString::number(apiCode) + " " + message : QString());
    }
};

struct CallbackStruct
{
    RequestError error;
    void *data;

    CallbackStruct(void *datap) : data(datap) {}
    CallbackStruct(RequestError errorp, void *datap) : error(errorp), data(datap) {}
};

typedef std::function<void(CallbackStruct)> Callback;

struct RequestParameters
{
    Callback callback;
    const QString url;
    const QString postDatas;
    const QString customRequest;
    const QString fileName;
    const QString outputFile;
    int type;
    bool json;
};

struct RequestParametersNoCb
{
    RequestParametersNoCb operator=(RequestParametersNoCb other)
    {
        return RequestParametersNoCb {
            other.url,
            other.postDatas,
            other.customRequest,
            other.fileName,
            other.outputFile,
            other.type,
            other.json
        };
    }

    bool operator==(const RequestParametersNoCb& other) const {
        return (url == other.url
            && postDatas == other.postDatas
            && customRequest == other.customRequest
            && fileName == other.fileName
            && outputFile == other.outputFile
            && type == other.type
            && json == other.json);
    }

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
    Delete,
    Patch
};

template <typename K, typename V>
class Map {
public:
    Map() {

    }

    V& operator[](const K& key)
    {
        for (int i = 0 ; i < keys.size() ; i++) {
            if (key == keys[i])
                return values[i];
        }

        keys.append(key);
        values.append(V());
        return values[values.size() - 1];
    }

    int size()
    {
        return keys.size();
    }

    bool contains(const K& key)
    {
        for (int i = 0 ; i < keys.size() ; i++) {
            if (key == keys[i]) 
                return true;
        }
        return false;
    }

    void remove(const K& key)
    {
        for (int i = 0 ; i < keys.size() ; i++) {
            if (key == keys[i]) {
                keys.remove(i);
                values.remove(i);
                break;
            }
        }
    }

    QVector<K> keys;
    QVector<V> values;
};

// Class to request the API
class Requester : public QObject
{
    Q_OBJECT
public:
    Requester(const QString& token, QObject *parent);
    ~Requester();

    // Function that request the API
    void removeRequests(RequestTypes type);
    void removeRequestWithUrl(const QString& url);

    void const getGuilds(Callback callback);
    void const getGuildMember(Callback callback, const Snowflake& guildId, const Snowflake& userId);
    void const getGuildChannels(Callback callback, const Snowflake& id);
    void const getPrivateChannels(Callback callback);
    void const getMessages(Callback callback, const Snowflake& channelId, const Snowflake& beforeId, unsigned int limit);
    void const getClient(Callback callback);
    void const getClientSettings(Callback callback);
    void const getWsUrl(Callback callback);
    void const getImage(Callback callback, const QString& url, const QString& fileName);
    void const getUser(Callback callback, const Snowflake& userId);
    void const getFile(Callback callback, const QString& url, const QString& filename);
    void const getConsent(Callback callback);
    void const getHarvest(Callback callback);
    void const getAuthorizedApp(Callback callback);
    void const getConnections(Callback callback);
    
    void const changeUsername(Callback callback, const QString& username, const QString& discriminator, const QString& password);
    void const changeEmail(Callback callback, QString email, QString password);
    void const changePassword(Callback callback, QString oldPassword, QString newPassword);
    void const removePhone(Callback callback, QString password);
    void const disableAccount(Callback callback, QString password);
    void const deleteAccount(Callback callback, QString password);
    void const changeClient(Callback callback, QString json);
    void const setSettings(Callback callback, QString settings);
    void const setSettingsProto(Callback callback, QString settings);
    void const setConsent(Callback callback, QString grant, QString revoke);

    void const setStatus(Callback callback, const QString& status);
    void const sendTyping(Callback callback, const Snowflake& channelId);
    void const sendVerifyCode(Callback callback, QString verifyCode);
    void const sendVerificationEmail(Callback callback);
    void const harvestData(Callback callback);
    void const sendMessage(Callback callback, const QString& content, const Snowflake& channelId);
    void const sendMessageWithFile(Callback callback, const QString& content, const Snowflake& channelId, const QString& filePath);
    void const deleteMessage(Callback callback, const Snowflake& channelId, const Snowflake& messageId);
    void const pinMessage(Callback callback, const Snowflake& channelId, const Snowflake& messageId);
    void const unpinMessage(Callback callback, const Snowflake& channelId, const Snowflake& messageId);
    void const deleteAuthorizedApp(Callback callback, const Snowflake& appId);
    void const setConnection(Callback callback, QString type, QString id, int visibility);
    void const removeConnection(Callback callback, QString type, QString id);
    void const addReaction(Callback callback, const Snowflake& channelId, const Snowflake& messageId, const Snowflake& reactionId, const QString& reactionName);
    void const deleteReaction(Callback callback, const Snowflake& channelId, const Snowflake& messageId, const Snowflake& reactionId, const QString& reactionName);

signals:
    void requestEmit(int requestType, QNetworkRequest request, QByteArray *query, QHttpMultiPart *multiPart);
    void invalidToken();

private slots:
    void doRequest(int requestType, QNetworkRequest request, QByteArray *query, QHttpMultiPart *multiPart);
    void readReply();
    void const writeFile();

private:
    QNetworkAccessManager netManager;
    QNetworkReply *reply;
    QQueue<RequestParametersNoCb> requestQueue;     // Queue of request parameters
    QMutex finishLock;
    QMutex requestLock;
    QWaitCondition requestWaiter;               // The loop waits when there is no request
    QWaitCondition finishWaiter;                // The loop waits when there is no request
    QThread *loop;                              // Request loop
    QMap<QString, int> urlsTocheck;
    QMap<RequestTypes, int> typesToCheck;
    Map<RequestParametersNoCb, QVector<Callback>> requestsCallbacks;
    QString token;                              // Authorization token
    QString emailToken;
    double rateLimitEnd;                        // Unix time that represents the moment of the end of the rate limit
    int currentRequestsNumber;                  // The number of requests that are processed at the moment
    bool stopped;                               // Used to stop the request loop

    void requestApi(const RequestParameters& parameters);
    void callCallbacks(const RequestParametersNoCb& parameters, CallbackStruct cb);
    void RequestLoop();
};

} // namespace Api
