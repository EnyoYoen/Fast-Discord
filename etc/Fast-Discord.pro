QT += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 static
DESTDIR = ../bin/
OBJECTS_DIR = $${DESTDIR}
MOC_DIR = $${DESTDIR}
RCC_DIR = $${DESTDIR}
UI_DIR = $${DESTDIR}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    ../src/api/application.cpp \
    ../src/api/attachment.cpp \
    ../src/api/channel.cpp \
    ../src/api/client.cpp \
    ../src/api/embed.cpp \
    ../src/api/emoji.cpp \
    ../src/api/gateway.cpp \
    ../src/api/guild.cpp \
    ../src/api/guildmember.cpp \
    ../src/api/jsonutils.cpp \
    ../src/api/message.cpp \
    ../src/api/overwrite.cpp \
    ../src/api/presence.cpp \
    ../src/api/request.cpp \
    ../src/api/ressourcemanager.cpp \
    ../src/api/role.cpp \
    ../src/api/sticker.cpp \
    ../src/api/team.cpp \
    ../src/api/thread.cpp \
    ../src/api/user.cpp \
    ../src/api/voice.cpp \
    ../src/main.cpp \
    ../src/ui/attachmentfile.cpp \
    ../src/ui/closechannelbutton.cpp \
    ../src/ui/guildchannelwidget.cpp \
    ../src/ui/guildpill.cpp \
    ../src/ui/guildwidget.cpp \
    ../src/ui/homebutton.cpp \
    ../src/ui/leftcolumn.cpp \
    ../src/ui/mainwindow.cpp \
    ../src/ui/markdownlabel.cpp \
    ../src/ui/messagearea.cpp \
    ../src/ui/messageseparator.cpp \
    ../src/ui/messagetextinput.cpp \
    ../src/ui/messagewidget.cpp \
    ../src/ui/middlecolumn.cpp \
    ../src/ui/privatechannel.cpp \
    ../src/ui/rightcolumn.cpp \
    ../src/ui/roundedimage.cpp \
    ../src/ui/statusicon.cpp \
    ../src/ui/usermenu.cpp \
    ../src/ui/usermenubutton.cpp

HEADERS += \
    ../include/api/application.h \
    ../include/api/attachment.h \
    ../include/api/channel.h \
    ../include/api/client.h \
    ../include/api/embed.h \
    ../include/api/emoji.h \
    ../include/api/gateway.h \
    ../include/api/guild.h \
    ../include/api/guildmember.h \
    ../include/api/jsonutils.h \
    ../include/api/message.h \
    ../include/api/overwrite.h \
    ../include/api/presence.h \
    ../include/api/request.h \
    ../include/api/ressourcemanager.h \
    ../include/api/role.h \
    ../include/api/sticker.h \
    ../include/api/team.h \
    ../include/api/thread.h \
    ../include/api/user.h \
    ../include/api/voice.h \
    ../include/ui/attachmentfile.h \
    ../include/ui/closechannelbutton.h \
    ../include/ui/guildchannelwidget.h \
    ../include/ui/guildpill.h \
    ../include/ui/guildwidget.h \
    ../include/ui/homebutton.h \
    ../include/ui/leftcolumn.h \
    ../include/ui/mainwindow.h \
    ../include/ui/markdownlabel.h \
    ../include/ui/messagearea.h \
    ../include/ui/messageseparator.h \
    ../include/ui/messagetextinput.h \
    ../include/ui/messagewidget.h \
    ../include/ui/middlecolumn.h \
    ../include/ui/privatechannel.h \
    ../include/ui/rightcolumn.h \
    ../include/ui/roundedimage.h \
    ../include/ui/statusicon.h \
    ../include/ui/usermenu.h \
    ../include/ui/usermenubutton.h

INCLUDEPATH += "../include/"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
