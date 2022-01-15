#include "ui/markdownlabel.h"

#include <QDebug>

namespace Ui {

MarkdownLabel::MarkdownLabel(const std::string& content, QWidget *parent)
    : QLabel(parent)
{
    std::string html = "<html>";
    unsigned int boldEnd = 0, italicStarEnd = 0, italicUnderscoreEnd = 0, underlineEnd = 0, barredEnd = 0;
    for (unsigned int i = 0 ; i < content.size() ; i++) {
        if (boldEnd != 0 && boldEnd == i) {
            html += "</b>";
            boldEnd = 0;
            i++;
        } else if (italicUnderscoreEnd != 0 && italicUnderscoreEnd == i) {
            html += "</i>";
            italicUnderscoreEnd = 0;
        } else if (italicStarEnd != 0 && italicStarEnd == i) {
            html += "</i>";
            italicStarEnd = 0;
        } else if (underlineEnd != 0 && underlineEnd == i) {
            html += "</u>";
            underlineEnd = 0;
            i++;
        } else if (barredEnd != 0 && barredEnd == i) {
            html += "</strike>";
            barredEnd = 0;
            i++;
        } else if (content[i] == '_') {
            if (content[i + 1] == '_') {
                if (underlineEnd == 0 && (underlineEnd = content.find("__", i + 2)) != std::string::npos && underlineEnd < content.find('\n', i + 2)) {
                    html += "<u>";
                    i++;
                } else {
                    underlineEnd = 0;
                }
            } else {
                if (italicUnderscoreEnd == 0 && italicStarEnd == 0
                    && (italicUnderscoreEnd = content.find("_", i + 1)) != std::string::npos && italicUnderscoreEnd < content.find('\n', i + 1)
                    && content[italicUnderscoreEnd + 1] < 'a' && content[italicUnderscoreEnd + 1] > 'z' && content[italicUnderscoreEnd + 1] < 'A'
                    && content[italicUnderscoreEnd + 1] > 'Z' && content[italicUnderscoreEnd + 1] < 'a' && content[italicUnderscoreEnd + 1] != '_') {
                        html += "<i>";
                } else {
                    html += "_";
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
                    }
                } else {
                    if (boldEnd == 0 && (boldEnd = content.find("**", i + 2)) != std::string::npos && boldEnd < content.find('\n', i + 2)) {
                        html += "<b>";
                        i++;
                    } else {
                        boldEnd = 0;
                    }
                }
            } else {
                if (italicStarEnd == 0 && (italicStarEnd = content.find("*", i + 1)) != std::string::npos && italicStarEnd < content.find('\n', i + 1)) {
                    html += "<i>";
                } else {
                    italicStarEnd = 0;
                }
            }
        } else if (content[i] == '~' && content[i + 1] == '~') {
            if (barredEnd == 0 && (barredEnd = content.find("~~", i + 2)) != std::string::npos && barredEnd < content.find('\n', i + 2)) {
                html += "<strike>";
                i++;
            } else {
                barredEnd = 0;
            }
        } else if (content[i] == '\n') {
            html += "<br>";
        } else if (content[i] == '\t') {
            html += "&nbsp;&nbsp;&nbsp;&nbsp;";
        }  else if (content[i] == ' ') {
            html += "&nbsp;";
        } else if (content[i] == '<') {
            html += "&lt;";
        } else if (content[i] == '>') {
            html += "&gt;";
        } else {
            html += content[i];
        }
    }
    html += "</html>";

    this->setText(html.c_str());
    this->setTextInteractionFlags(Qt::TextSelectableByMouse);
    this->setCursor(QCursor(Qt::IBeamCursor));
    this->setWordWrap(true);
    this->setStyleSheet("color: #DCDDDE;"
                        "background-color: #36393F;");
}

} // namespace Ui
