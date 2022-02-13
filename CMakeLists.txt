cmake_minimum_required(VERSION 3.0.0)
project(
    Fast-Discord
    VERSION 0.0.1
    DESCRIPTION "A new Discord client made in C++ and Qt"
    LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(lib/)
include_directories(include/)

find_package(Qt5 COMPONENTS Widgets Network WebSockets REQUIRED)
if (NOT WIN32)
    find_package(Threads REQUIRED)
endif (NOT WIN32)

set(SOURCES
    "src/api/application.cpp"
    "src/api/attachment.cpp"
    "src/api/channel.cpp"
    "src/api/embed.cpp"
    "src/api/emoji.cpp"
    "src/api/gateway.cpp"
    "src/api/guild.cpp"
    "src/api/guildmember.cpp"
    "src/api/jsonutils.cpp"
    "src/api/message.cpp"
    "src/api/overwrite.cpp"
    "src/api/request.cpp"
    "src/api/ressourcemanager.cpp"
    "src/api/role.cpp"
    "src/api/sticker.cpp"
    "src/api/team.cpp"
    "src/api/thread.cpp"
    "src/api/user.cpp"
    "src/api/voice.cpp"
    "src/main.cpp"
    "src/ui/attachmentfile.cpp"
    "src/ui/closechannelbutton.cpp"
    "src/ui/downloadbutton.cpp"
    "src/ui/downloadlink.cpp"
    "src/ui/guildchannelwidget.cpp"
    "src/ui/guildpill.cpp"
    "src/ui/guildwidget.cpp"
    "src/ui/homebutton.cpp"
    "src/ui/leftcolumn.cpp"
    "src/ui/mainwindow.cpp"
    "src/ui/markdownlabel.cpp"
    "src/ui/messagearea.cpp"
    "src/ui/messageseparator.cpp"
    "src/ui/messagetextinput.cpp"
    "src/ui/messagewidget.cpp"
    "src/ui/middlecolumn.cpp"
    "src/ui/privatechannel.cpp"
    "src/ui/rightcolumn.cpp"
    "src/ui/roundedimage.cpp"
    "src/ui/statusicon.cpp"
    "src/ui/usermenu.cpp"
    "src/ui/usermenubutton.cpp"
)

set(HEADERS
    "include/api/application.h"
    "include/api/attachment.h"
    "include/api/channel.h"
    "include/api/embed.h"
    "include/api/emoji.h"
    "include/api/gateway.h"
    "include/api/guild.h"
    "include/api/guildmember.h"
    "include/api/jsonutils.h"
    "include/api/message.h"
    "include/api/overwrite.h"
    "include/api/request.h"
    "include/api/ressourcemanager.h"
    "include/api/role.h"
    "include/api/sticker.h"
    "include/api/team.h"
    "include/api/thread.h"
    "include/api/user.h"
    "include/api/voice.h"
    "include/ui/attachmentfile.h"
    "include/ui/closechannelbutton.h"
    "include/ui/downloadbutton.h"
    "include/ui/downloadlink.h"
    "include/ui/guildchannelwidget.h"
    "include/ui/guildpill.h"
    "include/ui/guildwidget.h"
    "include/ui/homebutton.h"
    "include/ui/leftcolumn.h"
    "include/ui/middlecolumn.h"
    "include/ui/mainwindow.h"
    "include/ui/markdownlabel.h"
    "include/ui/messagearea.h"
    "include/ui/messageseparator.h"
    "include/ui/messagetextinput.h"
    "include/ui/messagewidget.h"
    "include/ui/privatechannel.h"
    "include/ui/rightcolumn.h"
    "include/ui/roundedimage.h"
    "include/ui/statusicon.h"
    "include/ui/usermenu.h"
    "include/ui/usermenubutton.h"
)

add_executable(Fast-Discord ${SOURCES} ${HEADERS})

target_link_libraries(Fast-Discord Qt5::Widgets)
target_link_libraries(Fast-Discord Qt5::Network)
target_link_libraries(Fast-Discord Qt5::WebSockets)
if (NOT WIN32)
    target_link_libraries(Fast-Discord pthread)
endif (NOT WIN32)
