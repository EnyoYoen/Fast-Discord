#include "ui/markdownlabel.h"

#include <QFontMetrics>

namespace Ui {

MarkdownLabel::MarkdownLabel(const QString& content, Api::RessourceManager *rm, QWidget *parent)
    : QLabel(parent)
{
    QFont font;
    font.setPixelSize(16);
    font.setFamily("whitney");
    setFont(font);
    if (!content.isNull() && !content.isEmpty()) {
        QString html = "<html>";
        bool block = false, firstCharFound = false;
        int boldEnd = 0, italicStarEnd = 0, italicUnderscoreEnd = 0, underlineEnd = 0
            , barredEnd = 0, simpleCodeBlockEnd = 0, multiCodeBlockEnd = 0;

        for (int i = 0 ; i < content.size() ; i++) {
            if (i > content.size())
                break;
            bool special = false;
            if (simpleCodeBlockEnd == 0 && multiCodeBlockEnd == 0) {
                if (boldEnd != 0 && boldEnd == i) {
                    html += "</b>";
                    boldEnd = 0;
                    i++;
                    special = true;
                } else if (italicUnderscoreEnd != 0 && italicUnderscoreEnd == i) {
                    html += "</i>";
                    italicUnderscoreEnd = 0;
                    special = true;
                } else if (italicStarEnd != 0 && italicStarEnd == i) {
                    html += "</i>";
                    italicStarEnd = 0;
                    special = true;
                } else if (underlineEnd != 0 && underlineEnd == i) {
                    html += "</u>";
                    underlineEnd = 0;
                    i++;
                    special = true;
                } else if (barredEnd != 0 && barredEnd == i) {
                    html += "</strike>";
                    barredEnd = 0;
                    i++;
                    special = true;
                } else if (content[i] == '`') {
                    if (i + 2 < content.size() && content[i + 1] == '`' && content[i + 2] == '`') {
                        if ((multiCodeBlockEnd = content.indexOf("```", i + 1)) != -1) {
                            html += "<div>";
                            i += 2;
                            block = true;
                            special = true;
                        } else {
                            multiCodeBlockEnd = 0;
                        }
                    } else {
                        if ((simpleCodeBlockEnd = content.indexOf('`', i + 1)) != -1 && simpleCodeBlockEnd < content.indexOf('\n', i + 1)) {
                            html += "<code>";
                            block = true;
                            special = true;
                        } else {
                            simpleCodeBlockEnd = 0;
                        }
                    }
                } else if (content[i] == '_') {
                    if (i + 1 < content.size() && content[i + 1] == '_') {
                        if (underlineEnd == 0 && (underlineEnd = content.indexOf("__", i + 2)) != -1 && underlineEnd < content.indexOf('\n', i + 2)) {
                            html += "<u>";
                            i++;
                            special = true;
                        } else {
                            underlineEnd = 0;
                        }
                    } else {
                        if (italicUnderscoreEnd == 0 && italicStarEnd == 0
                            && (italicUnderscoreEnd = content.indexOf('_', i + 1)) != -1 && italicUnderscoreEnd < content.indexOf('\n', i + 1)
                            && (content[italicUnderscoreEnd + 1] < '0' || (content[italicUnderscoreEnd + 1] > '9' && content[italicUnderscoreEnd + 1] < 'A')
                            || (content[italicUnderscoreEnd + 1] > 'Z' && content[italicUnderscoreEnd + 1] < 'a') || content[italicUnderscoreEnd + 1] > 'z')) {
                                html += "<i>";
                                special = true;
                        } else {
                            italicUnderscoreEnd = 0;
                        }
                    }
                } else if (content[i] == '*') {
                    if (i + 1 < content.size() && content[i + 1] == '*') {
                        if (i + 2 < content.size() && content[i + 2] == '*') {
                            unsigned int end = 0;
                            if (boldEnd == 0 && italicStarEnd == 0 && italicUnderscoreEnd == 0
                                    && (end = content.indexOf("***", i + 3)) != -1
                                    && end < content.indexOf('\n', i + 3)) {
                                boldEnd = end;
                                italicStarEnd = end + 2;
                                html += "<i><b>";
                                i += 2;
                                special = true;
                            }
                        } else {
                            if (boldEnd == 0 && (boldEnd = content.indexOf("**", i + 2)) != -1 && boldEnd < content.indexOf('\n', i + 2)) {
                                html += "<b>";
                                i++;
                                special = true;
                            } else {
                                boldEnd = 0;
                            }
                        }
                    } else {
                        if (italicStarEnd == 0 && (italicStarEnd = content.indexOf('*', i + 1)) != -1 && italicStarEnd < content.indexOf('\n', i + 1)) {
                            html += "<i>";
                            special = true;
                        } else {
                            italicStarEnd = 0;
                        }
                    }
                } else if (i + 1 < content.size() && content[i] == '~' && content[i + 1] == '~') {
                    if (barredEnd == 0 && (barredEnd = content.indexOf("~~", i + 2)) != -1 && barredEnd < content.indexOf('\n', i + 2)) {
                        html += "<strike>";
                        i++;
                        special = true;
                    } else {
                        barredEnd = 0;
                    }
                }
            } else if (simpleCodeBlockEnd == i) {
                html += "</code>";
                simpleCodeBlockEnd = 0;
                special = true;
            } else if (multiCodeBlockEnd == i) {
                html += "</div>";
                multiCodeBlockEnd = 0;
                i += 2;
                special = true;
            }

            if (!special) {
                if (content[i] == '\n') {
                    html += "<br>";
                    firstCharFound = false;
                    simpleCodeBlockEnd = 0;
                } else if (content[i] == '\t') {
                    html += "&nbsp;&nbsp;&nbsp;&nbsp;";
                } else if (content[i] == ' ' && !firstCharFound) {
                    html += "&nbsp;";
                } else if (content[i] == '<') {
                    unsigned int emojiNameEnd = content.indexOf(':', i + 2);
                    unsigned int emojiIdEnd = content.indexOf('>', i + 1);
                    if (content[i + 1] == ':' && emojiNameEnd != -1 && emojiIdEnd != -1 && emojiIdEnd < content.indexOf("\n") && emojiNameEnd < emojiIdEnd) {
                        QString fileName = content.mid(emojiNameEnd + 1, emojiIdEnd - emojiNameEnd - 1) + ".webp";
                        //html += "<img src=\"cache/" + fileName + "\">";
                        //rm->getImage([this](void *){}, "https://cdn.discordapp.com/emojis/" + fileName, fileName);
                        i += emojiIdEnd - i;
                    } else {
                        html += "&lt;";
                        firstCharFound = true;
                    }
                } else if (content[i] == '>') {
                    if (i + 1 < content.size() && !firstCharFound && content[i + 1] == ' ') {
                        html += "<span>&nbsp;</span>&nbsp;&nbsp;&nbsp;&nbsp;";
                        block = true;
                        i++;
                    } else {
                        html += "&gt;";
                        firstCharFound = true;
                    }
                } else {
                    firstCharFound = true;
                    html += content[i];
                }
            }
        }
        if (block) {
            html.insert(6, "<style>"
                    "*{color:#DCDDDE;}"
                    "span{"
                      "background-color:#4F545C;"
                    "}"
                    "code,div{"
                      "background-color:#2F3136;"
                      "margin-bottom:0px;"
                    "}"
                    "</style>");
        }
        html += "</html>";

        this->setText(html);
        this->setTextInteractionFlags(Qt::TextSelectableByMouse);
        this->setCursor(QCursor(Qt::IBeamCursor));
        this->setWordWrap(true);
        this->setStyleSheet("color: #DCDDDE;"
                            "background-color: #36393F;");
    }
}

} // namespace Ui
