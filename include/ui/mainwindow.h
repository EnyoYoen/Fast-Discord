#pragma once

#include "leftcolumn.h"
#include "middlecolumn.h"
#include "rightcolumn.h"
#include "api/gateway.h"
#include "api/client.h"
#include "api/jsonutils.h"

#include <QWidget>
#include <QHBoxLayout>

#include <string>
#include <vector>

namespace Ui {

// The main window of the app
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();

signals:
    void messageReceived(Api::Message);
    void clientSettingsReceived();
    void unreadUpdateReceived(const std::string&);

private slots:
    void setup();

private:
    void setupInterface(); // Create the interface
    void gatewayDispatchHandler(std::string& eventName, json& data);
        // Event handler for the gateway

    //All the widget that need to be accessed from different functions
    QHBoxLayout  *mainLayout;
    LeftColumn   *leftColumn;
    MiddleColumn *middleColumn;
    RightColumn  *rightColumn;

    Api::Gateway   *gw;                  // For the websocket connection
    Api::Requester *requester;           // To request the API

    Api::Client *client;                 // The actual client
    Api::ClientSettings *clientSettings; // and the settings

    json gatewayData;
};

} // namespace Ui
