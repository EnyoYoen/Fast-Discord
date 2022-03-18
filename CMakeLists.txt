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

include_directories(include/)

find_package(Qt5 COMPONENTS Widgets Network WebSockets REQUIRED)
if (NOT WIN32)
    find_package(Threads REQUIRED)
endif (NOT WIN32)

set(SOURCES
    "src/api/gateway.cpp"
    "src/api/jsonutils.cpp"
    "src/api/request.cpp"
    "src/api/ressourcemanager.cpp"
    "src/main.cpp"
    "src/ui/attachmentfile.cpp"
    "src/ui/closechannelbutton.cpp"
    "src/ui/downloadbutton.cpp"
    "src/ui/downloadlink.cpp"
    "src/ui/guildchannelwidget.cpp"
    "src/ui/guildfolder.cpp"
    "src/ui/guildicon.cpp"
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
    "include/api/objects/application.h"
    "include/api/objects/attachment.h"
    "include/api/objects/channel.h"
    "include/api/objects/embed.h"
    "include/api/objects/emoji.h"
    "include/api/objects/guild.h"
    "include/api/objects/guildmember.h"
    "include/api/objects/message.h"
    "include/api/objects/overwrite.h"
    "include/api/objects/role.h"
    "include/api/objects/snowflake.h"
    "include/api/objects/sticker.h"
    "include/api/objects/team.h"
    "include/api/objects/thread.h"
    "include/api/objects/user.h"
    "include/api/objects/voice.h"
    "include/api/gateway.h"
    "include/api/jsonutils.h"
    "include/api/request.h"
    "include/api/ressourcemanager.h"
    "include/ui/attachmentfile.h"
    "include/ui/closechannelbutton.h"
    "include/ui/downloadbutton.h"
    "include/ui/downloadlink.h"
    "include/ui/guildchannelwidget.h"
    "include/ui/guildfolder.h"
    "include/ui/guildicon.h"
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
