#include "ui/rightcolumn.h"

#include "ui/messagetextinput.h"
#include "api/request.h"
#include "api/user.h"
#include "api/jsonutils.h"

#include <QGroupBox>
#include <QDateTime>

#include <thread>

namespace Ui {

RightColumn::RightColumn(Api::Client *clientp, QWidget *parent)
    : QWidget(parent)
{
    // Attribute initialization
    client = clientp;

    // Create and style the layout
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create a placeholder
    QWidget *placeholder = new QWidget(this);
    placeholder->setStyleSheet("background-color: #36393F;");
    layout->addWidget(placeholder);

    // Style the column
    this->setStyleSheet("background-color: #36393F;");
}

void RightColumn::clean()
{
    // Remove all items from the layout
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // And add a placeholder
    QWidget *placeholder = new QWidget(this);
    placeholder->setStyleSheet("background-color: #36393F;");
    layout->addWidget(placeholder);
}

void RightColumn::openChannel(Api::Channel& channel)
{
    // Do nothing if the channel is a guild voice (not implemented)
    if (channel.type != Api::GuildVoice) {
        this->clean(); // Clean the column

        // Get the messages of the channel
        std::string channelId = *channel.id;
        std::vector<Api::Message *> *messages;

        std::map<std::string, std::vector<Api::Message *> *>::iterator currentMessages = channelsMessages.find(channelId);
        if (currentMessages == channelsMessages.end()) {
            messages = Api::Request::getMessages(channelId, 50 );
            channelsMessages[channelId] = messages;
        } else {
            messages = channelsMessages[channelId];
        }

        // Change the current opened channel ID
        currentOpenedChannel = channelId;

        // Create all the widgets
        messageArea = new MessageArea(*messages, this);
        QWidget *messagesContainer = new QWidget(this);
        QVBoxLayout *messagesLayout = new QVBoxLayout(messagesContainer);
        QGroupBox *inputBox = new QGroupBox(messagesContainer);
        QHBoxLayout *inputLayout = new QHBoxLayout(inputBox);
        MessageTextInput *textInput = new MessageTextInput(inputBox);

        // Add widget to the layout and style it
        inputLayout->addWidget(textInput);
        inputLayout->setSpacing(0);

        // Style the input box
        inputBox->setStyleSheet("background-color: #40444B;"
                                "height: 44px;"
                                "border-radius: 8px;");

        // Create and style the typing label
        typingLabel = new QLabel(messagesContainer);
        typingLabel->setText("");
        typingLabel->setStyleSheet("height: 24px;"
                                   "color: #DCDDDE");

        // Add widgets to the message layout and style it
        messagesLayout->addWidget(messageArea);
        messagesLayout->addWidget(inputBox);
        messagesLayout->addWidget(typingLabel);
        messagesLayout->setSpacing(0);
        messagesLayout->setContentsMargins(0, 0, 0, 0);

        // Add a widget to the layout and style it
        layout->addWidget(messagesContainer);
        layout->setContentsMargins(0, 0, 0, 0);

        if (channel.type != Api::DM && channel.type != Api::GroupDM) {
            // Guild channel

            // Create the widgets of the user list
            QScrollArea *userList = new QScrollArea(this);
            //QVBoxLayout *userListLayout = new QVBoxLayout(userList);

            // Style the user list
            userList->setFixedWidth(240);
            userList->setStyleSheet("border: none;"
                                    "background-color: #2F3136");

            // Add it to the layout
            layout->addWidget(userList);
        }

        // Connect signals to slots
        QObject::connect(textInput, SIGNAL(returnPressed(std::string)), this, SLOT(sendMessage(const std::string&)));
        QObject::connect(textInput, SIGNAL(typing()), this, SLOT(sendTyping()));
    }
}

void RightColumn::addMessage(Api::Message message)
{
    // Add the message if it belongs to this channels
    if (*message.channelId == currentOpenedChannel) {
        std::vector<Api::Message *> channelMessages = *channelsMessages[currentOpenedChannel];
        messageArea->addMessage(message, *(*channelsMessages[currentOpenedChannel])[0]);
    }
}

void RightColumn::userTyping(const json& data)
{
    // TODO : it's showing typing only for the current opened channel, other typing events are ignored
    std::string channelId = data.value("channel_id", "");
    if (currentOpenedChannel == channelId) {
        // A user is typing in this channel

        // Get the actual timestamp
        time_t currentTimestamp = std::time(nullptr);

        // Get the user that is typing
        Api::MemoryStruct response;
        std::string url = std::string("https://discord.com/api/v9/users/") + data.value("user_id", "");
        Api::Request::requestJson(url, "", &response, "", "");
        Api::User *user;
        Api::unmarshal<Api::User>(json::parse(response.memory), "", &user);

        // Get the user name and the text of the typing label
        std::string username = *(*user).username;
        long typingTimestamp = data.value("timestamp", -1);
        std::string text = typingLabel->text().toUtf8().constData();

        // Change the text of the typing label
        if (text != "") {
            // Somemone is already typing, so we change with the new user
            size_t index = (text.find(" is typing") != std::string::npos) ? text.find(" is typing") : text.find(" are typing");
            text.resize(text.size() - index);
            text += " " + username + " are typing";
        } else {
            // The user is to only one to type
            text = username + " is typing";
        }

        // Change the text on the label and wait 8 seconds
        typingLabel->setText(QString::fromUtf8(text.c_str()));
        std::this_thread::sleep_for(std::chrono::seconds(typingTimestamp + 8 - currentTimestamp));
        typingLabel->setText("");
    }
}

void RightColumn::sendTyping()
{
    // Send typing to the API
    Api::Request::sendTyping(currentOpenedChannel);
}

void RightColumn::sendMessage(const std::string& content)
{
    // Send a new message to the API and add it to the opened channel
    Api::Request::sendMessage(content, currentOpenedChannel);
    std::string messageTimestamp = QDateTime::currentDateTime().toString(Qt::ISODateWithMs).toUtf8().constData();
    Api::Message newMessage = Api::Message {nullptr, new Api::User{client->username, nullptr, client->avatar, nullptr, nullptr, client->id, -1, -1, -1, false, false, false, false}, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, new std::string(content), new std::string(messageTimestamp), nullptr, nullptr, nullptr, nullptr, -1, -1, -1, -1, false, false, false};
    messageArea->addMessage(newMessage, *(*channelsMessages[currentOpenedChannel])[0]);
}

} // namespace Ui

