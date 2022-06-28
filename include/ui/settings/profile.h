#pragma once

#include "ui/common/roundedimage.h"
#include "api/ressourcemanager.h"
#include "api/objects/client.h"

#include <QWidget>
#include <QLabel>
#include <QTimer>

namespace Ui {

class Profile : public Widget
{
    Q_OBJECT
public:
    Profile(Api::RessourceManager *rm, Api::Client client, QWidget *parent);
    ~Profile();

    void setAvatar(QString file, QColor color);
    void setBanner(QString file, QColor color);
    void setBio(QString bio);

signals:
    void bannerChanged(QString bannerFile);
    void avatarChanged(QString avatarFile);

private:
    Label  *bio;
    Widget *banner;
    Widget *avatarBorders;
    Widget *imageUpload;
    RoundedImage *avatar;
    Widget *bioSection;
    QVBoxLayout *layout;
    QTimer *timer;
    qint64 counter;
    Api::RessourceManager *rm;
};

} // namespace Ui

#include "ui/settings/userprofile.h"