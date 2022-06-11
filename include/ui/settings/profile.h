#pragma once

#include "api/ressourcemanager.h"
#include "api/objects/client.h"

#include <QWidget>
#include <QTimer>

namespace Ui {

class Profile : public QWidget
{
    Q_OBJECT
public:
    Profile(Api::RessourceManager *rm, Api::Client client, QWidget *parent);
    ~Profile();

    void setAvatar(QString file, QString color);
    void setBanner(QString file, QString color);
    void setBio(QString bio);

signals:
    void bannerChanged(QString bannerFile);
    void avatarChanged(QString avatarFile);

private:
    QLabel *bio;
    QLabel *banner;
    QLabel *avatarBorders;
    QLabel *imageUpload;
    RoundedImage *avatar;
    QWidget *bioSection;
    QVBoxLayout *layout;
    QTimer *timer;
    qint64 counter;
    Api::RessourceManager *rm;
};

} // namespace Ui

#include "ui/settings/userprofile.h"