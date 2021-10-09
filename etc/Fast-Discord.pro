QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
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

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../src/api/application.cpp \
    ../src/api/attachment.cpp \
    ../src/api/channel.cpp \
    ../src/api/embed.cpp \
    ../src/api/emoji.cpp \
    ../src/api/gateway.cpp \
    ../src/api/guild.cpp \
    ../src/api/guildmember.cpp \
    ../src/api/jsonutils.cpp \
    ../src/api/message.cpp \
    ../src/api/overwrite.cpp \
    ../src/api/request.cpp \
    ../src/api/role.cpp \
    ../src/api/status.cpp \
    ../src/api/sticker.cpp \
    ../src/api/team.cpp \
    ../src/api/thread.cpp \
    ../src/api/user.cpp \
    ../src/api/voice.cpp \
    ../src/main.cpp \
    ../src/ui/mainwindow.cpp \
    ../src/ui/messagearea.cpp \
    ../src/ui/messagetextinput.cpp \
    ../src/ui/messagewidget.cpp \
    ../src/ui/privatechannel.cpp \
    ../src/ui/roundedimage.cpp

HEADERS += \
    ../include/api/application.h \
    ../include/api/attachment.h \
    ../include/api/channel.h \
    ../include/api/embed.h \
    ../include/api/emoji.h \
    ../include/api/gateway.h \
    ../include/api/guild.h \
    ../include/api/guildmember.h \
    ../include/api/jsonutils.h \
    ../include/api/message.h \
    ../include/api/overwrite.h \
    ../include/api/request.h \
    ../include/api/role.h \
    ../include/api/status.h \
    ../include/api/sticker.h \
    ../include/api/team.h \
    ../include/api/thread.h \
    ../include/api/user.h \
    ../include/api/voice.h \
    ../include/token.h \
    ../include/ui/mainwindow.h \
    ../include/ui/messagearea.h \
    ../include/ui/messagetextinput.h \
    ../include/ui/messagewidget.h \
    ../include/ui/privatechannel.h \
    ../include/ui/roundedimage.h \
    ../lib/nlohmann/adl_serializer.hpp \
    ../lib/nlohmann/byte_container_with_subtype.hpp \
    ../lib/nlohmann/detail/conversions/from_json.hpp \
    ../lib/nlohmann/detail/conversions/to_chars.hpp \
    ../lib/nlohmann/detail/conversions/to_json.hpp \
    ../lib/nlohmann/detail/exceptions.hpp \
    ../lib/nlohmann/detail/hash.hpp \
    ../lib/nlohmann/detail/input/binary_reader.hpp \
    ../lib/nlohmann/detail/input/input_adapters.hpp \
    ../lib/nlohmann/detail/input/json_sax.hpp \
    ../lib/nlohmann/detail/input/lexer.hpp \
    ../lib/nlohmann/detail/input/parser.hpp \
    ../lib/nlohmann/detail/input/position_t.hpp \
    ../lib/nlohmann/detail/iterators/internal_iterator.hpp \
    ../lib/nlohmann/detail/iterators/iter_impl.hpp \
    ../lib/nlohmann/detail/iterators/iteration_proxy.hpp \
    ../lib/nlohmann/detail/iterators/iterator_traits.hpp \
    ../lib/nlohmann/detail/iterators/json_reverse_iterator.hpp \
    ../lib/nlohmann/detail/iterators/primitive_iterator.hpp \
    ../lib/nlohmann/detail/json_pointer.hpp \
    ../lib/nlohmann/detail/json_ref.hpp \
    ../lib/nlohmann/detail/macro_scope.hpp \
    ../lib/nlohmann/detail/macro_unscope.hpp \
    ../lib/nlohmann/detail/meta/cpp_future.hpp \
    ../lib/nlohmann/detail/meta/detected.hpp \
    ../lib/nlohmann/detail/meta/identity_tag.hpp \
    ../lib/nlohmann/detail/meta/is_sax.hpp \
    ../lib/nlohmann/detail/meta/type_traits.hpp \
    ../lib/nlohmann/detail/meta/void_t.hpp \
    ../lib/nlohmann/detail/output/binary_writer.hpp \
    ../lib/nlohmann/detail/output/output_adapters.hpp \
    ../lib/nlohmann/detail/output/serializer.hpp \
    ../lib/nlohmann/detail/string_escape.hpp \
    ../lib/nlohmann/detail/value_t.hpp \
    ../lib/nlohmann/json.hpp \
    ../lib/nlohmann/json_fwd.hpp \
    ../lib/nlohmann/ordered_map.hpp \
    ../lib/nlohmann/thirdparty/hedley/hedley.hpp \
    ../lib/nlohmann/thirdparty/hedley/hedley_undef.hpp

LIBS += \
    -lcurl -lssl -lcrypto -lpthread -lcpprest -lz

INCLUDEPATH += "../lib/"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
