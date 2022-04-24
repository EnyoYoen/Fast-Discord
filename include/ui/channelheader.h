#pragma once

#include "api/ressourcemanager.h"

#include <QWidget>
#include <QHBoxLayout>

namespace Ui {

class ChannelHeader : public QWidget
{
    Q_OBJECT
public:
    ChannelHeader(Api::RessourceManager *rm, QWidget *parent);
    void openChannel(const QString& channelName, int channelType);
    void close();

private:
    Api::RessourceManager *rm;

    QWidget *container;
    QHBoxLayout *layout;
};

}
