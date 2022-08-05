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

find_package(Qt5 COMPONENTS Widgets Network Multimedia WebSockets REQUIRED)
if (NOT WIN32)
    find_package(Threads REQUIRED)
endif (NOT WIN32)

set(SOURCES
    "src/api/gateway.cpp"
    "src/api/jsonutils.cpp"
    "src/api/request.cpp"
    "src/api/ressourcemanager.cpp"
    "src/api/voicesocket.cpp"
    "src/main.cpp"
    "src/settings/settings.cpp"
    "src/ui/common/basicwidgets.cpp"
    "src/ui/common/roundedimage.cpp"
    "src/ui/common/statusicon.cpp"
    "src/ui/leftcolumn/guildfolder.cpp"
    "src/ui/leftcolumn/guildicon.cpp"
    "src/ui/leftcolumn/guildpill.cpp"
    "src/ui/leftcolumn/guildwidget.cpp"
    "src/ui/leftcolumn/homebutton.cpp"
    "src/ui/leftcolumn/leftcolumn.cpp"
    "src/ui/mainwindow.cpp"
    "src/ui/middlecolumn/callwidget.cpp"
    "src/ui/middlecolumn/closecallbutton.cpp"
    "src/ui/middlecolumn/closechannelbutton.cpp"
    "src/ui/middlecolumn/guildchannelwidget.cpp"
    "src/ui/middlecolumn/middlecolumn.cpp"
    "src/ui/middlecolumn/privatechannel.cpp"
    "src/ui/middlecolumn/usermenu.cpp"
    "src/ui/middlecolumn/usermenubutton.cpp"
    "src/ui/settings/components/closebutton.cpp"
    "src/ui/settings/components/divider.cpp"
    "src/ui/settings/components/menubutton.cpp"
    "src/ui/settings/components/popup.cpp"
    "src/ui/settings/components/profile.cpp"
    "src/ui/settings/components/radiobutton.cpp"
    "src/ui/settings/components/scrollmenu.cpp"
    "src/ui/settings/components/selectimagebutton.cpp"
    "src/ui/settings/components/settingsbutton.cpp"
    "src/ui/settings/components/settingsinput.cpp"
    "src/ui/settings/components/slider.cpp"
    "src/ui/settings/components/switchbutton.cpp"
    "src/ui/settings/menus/accessibility.cpp"
    "src/ui/settings/menus/appearance.cpp"
    "src/ui/settings/menus/authorizedapps.cpp"
    "src/ui/settings/menus/connections.cpp"
    "src/ui/settings/menus/myaccount.cpp"
    "src/ui/settings/menus/privacysafety.cpp"
    "src/ui/settings/menus/userprofile.cpp"
    "src/ui/settings/settingsmenu.cpp"
    "src/ui/rightcolumn/attachmentfile.cpp"
    "src/ui/rightcolumn/channelheader.cpp"
    "src/ui/rightcolumn/downloadbutton.cpp"
    "src/ui/rightcolumn/downloadlink.cpp"
    "src/ui/rightcolumn/fileuploadbutton.cpp"
    "src/ui/rightcolumn/rightcolumn.cpp"
    "src/ui/rightcolumn/markdownlabel.cpp"
    "src/ui/rightcolumn/memberlist.cpp"
    "src/ui/rightcolumn/memberwidget.cpp"
    "src/ui/rightcolumn/messagearea.cpp"
    "src/ui/rightcolumn/messageseparator.cpp"
    "src/ui/rightcolumn/messagetextinput.cpp"
    "src/ui/rightcolumn/messagewidget.cpp"
)

set(HEADERS
    "include/api/objects/application.h"
    "include/api/objects/attachment.h"
    "include/api/objects/channel.h"
    "include/api/objects/client.h"
    "include/api/objects/connection.h"
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
    "include/api/voicesocket.h"
    "include/settings/settings.h"
    "include/ui/common/basicwidgets.h"
    "include/ui/common/roundedimage.h"
    "include/ui/common/statusicon.h"
    "include/ui/leftcolumn/guildfolder.h"
    "include/ui/leftcolumn/guildicon.h"
    "include/ui/leftcolumn/guildpill.h"
    "include/ui/leftcolumn/guildwidget.h"
    "include/ui/leftcolumn/homebutton.h"
    "include/ui/leftcolumn/leftcolumn.h"
    "include/ui/mainwindow.h"
    "include/ui/middlecolumn/callwidget.h"
    "include/ui/middlecolumn/closecallbutton.h"
    "include/ui/middlecolumn/closechannelbutton.h"
    "include/ui/middlecolumn/guildchannelwidget.h"
    "include/ui/middlecolumn/middlecolumn.h"
    "include/ui/middlecolumn/privatechannel.h"
    "include/ui/middlecolumn/usermenu.h"
    "include/ui/middlecolumn/usermenubutton.h"
    "include/ui/settings/components/closebutton.h"
    "include/ui/settings/components/divider.h"
    "include/ui/settings/components/menubutton.h"
    "include/ui/settings/components/popup.h"
    "include/ui/settings/components/profile.h"
    "include/ui/settings/components/radiobutton.h"
    "include/ui/settings/components/scrollmenu.h"
    "include/ui/settings/components/selectimagebutton.h"
    "include/ui/settings/components/settingsbutton.h"
    "include/ui/settings/components/settingsinput.h"
    "include/ui/settings/components/slider.h"
    "include/ui/settings/components/switchbutton.h"
    "include/ui/settings/menus/accessibility.h"
    "include/ui/settings/menus/appearance.h"
    "include/ui/settings/menus/authorizedapps.h"
    "include/ui/settings/menus/connections.h"
    "include/ui/settings/menus/myaccount.h"
    "include/ui/settings/menus/privacysafety.h"
    "include/ui/settings/menus/userprofile.h"
    "include/ui/settings/settingsmenu.h"
    "include/ui/rightcolumn/attachmentfile.h"
    "include/ui/rightcolumn/channelheader.h"
    "include/ui/rightcolumn/downloadbutton.h"
    "include/ui/rightcolumn/downloadlink.h"
    "include/ui/rightcolumn/fileuploadbutton.h"
    "include/ui/rightcolumn/rightcolumn.h"
    "include/ui/rightcolumn/markdownlabel.h"
    "include/ui/rightcolumn/memberlist.h"
    "include/ui/rightcolumn/memberwidget.h"
    "include/ui/rightcolumn/messagearea.h"
    "include/ui/rightcolumn/messageseparator.h"
    "include/ui/rightcolumn/messagetextinput.h"
    "include/ui/rightcolumn/messagewidget.h"
)

add_executable(Fast-Discord ${SOURCES} ${HEADERS})

if (WIN32)
    set(OPUS_INCLUDE_DIRS "libs/include")
    set(sodium_INCLUDE_DIR "libs/include")

    if (WINDOWS_32_BIT)
        set(OPUS_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/libs/32/lib/opus.lib)
        set(sodium_LIBRARY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/libs/32/lib/libsodium.lib)

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/32/bin/libsodium.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/32/bin/opus.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/64/bin/libcrypto-1_1.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/64/bin/libssl-1_1.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
    else()
        set(OPUS_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/libs/64/lib/opus.lib)
        set(sodium_LIBRARY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/libs/64/lib/libsodium.lib)

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/64/bin/libsodium.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/64/bin/opus.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/64/bin/libcrypto-1_1-x64.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libs/64/bin/libssl-1_1-x64.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} COPYONLY)
    endif()
else()
    include("FindOpus.cmake")
    include("FindSodium.cmake")

    if(NOT DEFINED OPUS_FOUND)
        message(FATAL_ERROR, "Opus not found, can't compile.")
    endif()
    if(NOT DEFINED sodium_VERSION_STRING)
        message(FATAL_ERROR, "Sodium not found, can't compile.")
    endif()

    target_link_libraries(Fast-Discord pthread)
endif()

target_link_libraries(Fast-Discord Qt5::Widgets)
target_link_libraries(Fast-Discord Qt5::Network)
target_link_libraries(Fast-Discord Qt5::Multimedia)
target_link_libraries(Fast-Discord Qt5::WebSockets)
target_link_libraries(Fast-Discord ${OPUS_LIBRARIES} ${sodium_LIBRARY_RELEASE})

include_directories(${OPUS_INCLUDE_DIRS} ${sodium_INCLUDE_DIR})
include_directories(include/)