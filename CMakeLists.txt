cmake_minimum_required(VERSION 3.9.0)
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

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
find_package(ZLIB REQUIRED)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(cpprestsdk REQUIRED)
find_package(Threads REQUIRED)

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
    "src/api/role.cpp"
    "src/api/sticker.cpp"
    "src/api/team.cpp"
    "src/api/thread.cpp"
    "src/api/user.cpp"
    "src/api/voice.cpp"
    "src/main.cpp"
    "src/ui/guildchannelwidget.cpp"
    "src/ui/guildwidget.cpp"
    "src/ui/homebutton.cpp"
    "src/ui/mainwindow.cpp"
    "src/ui/messagearea.cpp"
    "src/ui/messageseparator.cpp"
    "src/ui/messagetextinput.cpp"
    "src/ui/messagewidget.cpp"
    "src/ui/privatechannel.cpp"
    "src/ui/roundedimage.cpp"
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
    "include/api/role.h"
    "include/api/sticker.h"
    "include/api/team.h"
    "include/api/thread.h"
    "include/api/user.h"
    "include/api/voice.h"
    "include/ui/guildchannelwidget.h"
    "include/ui/guildwidget.h"
    "include/ui/homebutton.h"
    "include/ui/mainwindow.h"
    "include/ui/messagearea.h"
    "include/ui/messageseparator.h"
    "include/ui/messagetextinput.h"
    "include/ui/messagewidget.h"
    "include/ui/privatechannel.h"
    "include/ui/roundedimage.h"
    "lib/nlohmann/adl_serializer.hpp"
    "lib/nlohmann/byte_container_with_subtype.hpp"
    "lib/nlohmann/detail/conversions/from_json.hpp"
    "lib/nlohmann/detail/conversions/to_chars.hpp"
    "lib/nlohmann/detail/conversions/to_json.hpp"
    "lib/nlohmann/detail/exceptions.hpp"
    "lib/nlohmann/detail/hash.hpp"
    "lib/nlohmann/detail/input/binary_reader.hpp"
    "lib/nlohmann/detail/input/input_adapters.hpp"
    "lib/nlohmann/detail/input/json_sax.hpp"
    "lib/nlohmann/detail/input/lexer.hpp"
    "lib/nlohmann/detail/input/parser.hpp"
    "lib/nlohmann/detail/input/position_t.hpp"
    "lib/nlohmann/detail/iterators/internal_iterator.hpp"
    "lib/nlohmann/detail/iterators/iter_impl.hpp"
    "lib/nlohmann/detail/iterators/iteration_proxy.hpp"
    "lib/nlohmann/detail/iterators/iterator_traits.hpp"
    "lib/nlohmann/detail/iterators/json_reverse_iterator.hpp"
    "lib/nlohmann/detail/iterators/primitive_iterator.hpp"
    "lib/nlohmann/detail/json_pointer.hpp"
    "lib/nlohmann/detail/json_ref.hpp"
    "lib/nlohmann/detail/macro_scope.hpp"
    "lib/nlohmann/detail/macro_unscope.hpp"
    "lib/nlohmann/detail/meta/cpp_future.hpp"
    "lib/nlohmann/detail/meta/detected.hpp"
    "lib/nlohmann/detail/meta/identity_tag.hpp"
    "lib/nlohmann/detail/meta/is_sax.hpp"
    "lib/nlohmann/detail/meta/type_traits.hpp"
    "lib/nlohmann/detail/meta/void_t.hpp"
    "lib/nlohmann/detail/output/binary_writer.hpp"
    "lib/nlohmann/detail/output/output_adapters.hpp"
    "lib/nlohmann/detail/output/serializer.hpp"
    "lib/nlohmann/detail/string_escape.hpp"
    "lib/nlohmann/detail/value_t.hpp"
    "lib/nlohmann/json.hpp"
    "lib/nlohmann/json_fwd.hpp"
    "lib/nlohmann/ordered_map.hpp"
    "lib/nlohmann/thirdparty/hedley/hedley.hpp"
    "lib/nlohmann/thirdparty/hedley/hedley_undef.hpp"
)

add_executable(Fast-Discord ${SOURCES} ${HEADERS})

target_link_libraries(Fast-Discord Qt5::Widgets)
target_link_libraries(Fast-Discord Boost::filesystem)
target_link_libraries(Fast-Discord ZLIB::ZLIB)
target_link_libraries(Fast-Discord CURL::libcurl)
target_link_libraries(Fast-Discord OpenSSL::Crypto)
target_link_libraries(Fast-Discord cpprest)
target_link_libraries(Fast-Discord pthread)
