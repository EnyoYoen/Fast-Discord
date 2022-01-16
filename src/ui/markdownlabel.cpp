#include "ui/markdownlabel.h"

#include <QFontMetrics>
#include <QDebug>

namespace Ui {

MarkdownLabel::MarkdownLabel(const std::string& content, QWidget *parent)
    : QLabel(parent)
{
    if (content != "") {
        std::string html = "<html>";
        bool block = false, firstCharFound = false;
        unsigned int boldEnd = 0, italicStarEnd = 0, italicUnderscoreEnd = 0, underlineEnd = 0
                   , barredEnd = 0, simpleCodeBlockEnd = 0, multiCodeBlockEnd = 0;

        for (unsigned int i = 0 ; i < content.size() ; i++) {
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
                    if (content[i + 1] == '`' && content[i + 2] == '`') {
                        if ((multiCodeBlockEnd = content.find("```", i + 1)) != std::string::npos) {
                            html += "<div>";
                            i += 2;
                            block = true;
                            special = true;
                        } else {
                            multiCodeBlockEnd = 0;
                        }
                    } else {
                        if ((simpleCodeBlockEnd = content.find("`", i + 1)) != std::string::npos && simpleCodeBlockEnd < content.find('\n', i + 1)) {
                            html += "<code>";
                            block = true;
                            special = true;
                        } else {
                            simpleCodeBlockEnd = 0;
                        }
                    }
                } else if (content[i] == '_') {
                    if (content[i + 1] == '_') {
                        if (underlineEnd == 0 && (underlineEnd = content.find("__", i + 2)) != std::string::npos && underlineEnd < content.find('\n', i + 2)) {
                            html += "<u>";
                            i++;
                            special = true;
                        } else {
                            underlineEnd = 0;
                        }
                    } else {
                        if (italicUnderscoreEnd == 0 && italicStarEnd == 0
                            && (italicUnderscoreEnd = content.find("_", i + 1)) != std::string::npos && italicUnderscoreEnd < content.find('\n', i + 1)
                            && (content[italicUnderscoreEnd + 1] < '0' || (content[italicUnderscoreEnd + 1] > '9' && content[italicUnderscoreEnd + 1] < 'A')
                            || (content[italicUnderscoreEnd + 1] > 'Z' && content[italicUnderscoreEnd + 1] < 'a') || content[italicUnderscoreEnd + 1] > 'z')) {
                                html += "<i>";
                                special = true;
                        } else {
                            italicUnderscoreEnd = 0;
                        }
                    }
                } else if (content[i] == '*') {
                    if (content[i + 1] == '*') {
                        if (content[i + 2] == '*') {
                            unsigned int end = 0;
                            if (boldEnd == 0 && italicStarEnd == 0 && italicUnderscoreEnd == 0
                                    && (end = content.find("***", i + 3)) != std::string::npos
                                    && end < content.find('\n', i + 3)) {
                                boldEnd = end;
                                italicStarEnd = end + 2;
                                html += "<i><b>";
                                i += 2;
                                special = true;
                            }
                        } else {
                            if (boldEnd == 0 && (boldEnd = content.find("**", i + 2)) != std::string::npos && boldEnd < content.find('\n', i + 2)) {
                                html += "<b>";
                                i++;
                                special = true;
                            } else {
                                boldEnd = 0;
                            }
                        }
                    } else {
                        if (italicStarEnd == 0 && (italicStarEnd = content.find("*", i + 1)) != std::string::npos && italicStarEnd < content.find('\n', i + 1)) {
                            html += "<i>";
                            special = true;
                        } else {
                            italicStarEnd = 0;
                        }
                    }
                } else if (content[i] == '~' && content[i + 1] == '~') {
                    if (barredEnd == 0 && (barredEnd = content.find("~~", i + 2)) != std::string::npos && barredEnd < content.find('\n', i + 2)) {
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
                    html += "&lt;";
                } else if (content[i] == '>') {
                    if (!firstCharFound && content[i + 1] == ' ') {
                        html += "<span>&nbsp;</span>&nbsp;&nbsp;&nbsp;&nbsp;";
                        block = true;
                        i++;
                    } else {
                        html += "&gt;";
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

        this->setText(html.c_str());
        this->setTextInteractionFlags(Qt::TextSelectableByMouse);
        this->setCursor(QCursor(Qt::IBeamCursor));
        this->setWordWrap(true);
        this->setStyleSheet("color: #DCDDDE;"
                            "background-color: #36393F;");
    }
}

} // namespace Ui
