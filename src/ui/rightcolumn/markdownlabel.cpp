#include "ui/rightcolumn/markdownlabel.h"

#include <QFontMetrics>

namespace Ui {

QString monthFromNumber(int num)
{
    switch (num) {
        case 1:
            return "January";
        case 2:
            return "February";
        case 3:
            return "March";
        case 4:
            return "April";
        case 5:
            return "May";
        case 6:
            return "June";
        case 7:
            return "July";
        case 8:
            return "August";
        case 9:
            return "September";
        case 10:
            return "October";
        case 11:
            return "November";
        case 12:
            return "December";
        default:
            return "";
    }
}

QString dayFromNumber(int num)
{
    switch (num) {
        case 1:
            return "Monday";
        case 2:
            return "Tuesday";
        case 3:
            return "Wednesday";
        case 4:
            return "Thursday";
        case 5:
            return "Friday";
        case 6:
            return "Saturday";
        case 7:
            return "Sunday";
        default:
            return "";
    }
}

QString processTime(QTime time, bool shortTime)
{
    return QString::number((time.hour() > 12 ? time.hour() - 12 : time.hour())) + ":"
        + QString((time.minute() < 10 ? "0" : "")) + QString::number(time.minute())
        + (shortTime ? "" : ":" + QString((time.second() < 10 ? "0" : "")) + QString::number(time.second()))
        + (time.hour() > 12 ? " PM" : " AM");
}

QString processDate(QDate date, bool shortDate)
{
    return (shortDate ? QString::number(date.month()) + "/" : monthFromNumber(date.month()) + " ")
        + QString::number(date.day()) + (shortDate ? "/" : ", ") + QString::number(date.year());
}

QString processTimestamp(QDateTime timestamp, char format)
{
    QString processed;
    switch (format) {
        case 't':
            processed = processTime(timestamp.time(), true);
            break;
        case 'T':
            processed = processTime(timestamp.time(), false);
            break;
        case 'd':
            processed = processDate(timestamp.date(), true);
            break;
        case 'D':
            processed = processDate(timestamp.date(), false);
            break;
        case 'f':
            processed = processDate(timestamp.date(), false) + " at " + processTime(timestamp.time(), true);
            break;
        case 'F':
            processed = dayFromNumber(timestamp.date().dayOfWeek()) + ", " + processDate(timestamp.date(), false) + " at " + processTime(timestamp.time(), true);
            break;
        case 'R':
        {
            QDateTime current = QDateTime::currentDateTime();
            bool past = timestamp < current;
            if (!past)
                processed += "in ";
            int yearDiff = (past ? current.date().year() - timestamp.date().year() : timestamp.date().year() - current.date().year());
            int monthDiff = timestamp.date().month() - current.date().month();
            monthDiff = (monthDiff < 0 ? -monthDiff : monthDiff);
            if (yearDiff != 0 && !(yearDiff == 1 && monthDiff < 6)) {
                if (yearDiff == 1)
                    processed += (past ? "last year" : "next year");
                else
                    processed += QString::number(yearDiff) + " years";
            } else {
                int dayDiff = timestamp.date().day() - current.date().day();
                dayDiff = (dayDiff < 0 ? -dayDiff : dayDiff);
                if (monthDiff != 0 && !(monthDiff == 1 && dayDiff < 15)) {
                    if (monthDiff == 1)
                        processed += (past ? "last month" : "next month");
                    else
                        processed += QString::number(monthDiff) + " months";
                } else {
                    int hourDiff = timestamp.time().hour() - current.time().hour();
                    hourDiff = (hourDiff < 0 ? -hourDiff : hourDiff);
                    if (dayDiff != 0 && !(dayDiff == 1 && hourDiff < 12)) {
                        if (dayDiff == 1)
                            processed += (past ? "last day" : "next day");
                        else
                            processed += QString::number(dayDiff) + " days";
                    } else {
                        int minuteDiff = timestamp.time().minute() - current.time().minute();
                        minuteDiff = (minuteDiff < 0 ? -minuteDiff : minuteDiff);
                        if (hourDiff != 0 && !(hourDiff == 1 && minuteDiff < 12)) {
                            if (hourDiff == 1)
                                processed += (past ? "last hour" : "next hour");
                            else
                                processed += QString::number(hourDiff) + " hours";
                        } else {
                            int secondDiff = timestamp.time().second() - current.time().second();
                            secondDiff = (secondDiff < 0 ? -secondDiff : secondDiff);
                            if (secondDiff != 0) {
                                processed += QString::number(secondDiff) + (secondDiff > 1 ? " seconds" : "second");
                            } else {
                                processed = "now";
                                break;
                            }
                        }
                    }
                }
            }
            if (past)
                processed += " ago";
            break;
        }
    }
    return processed;
}

MarkdownLabel::MarkdownLabel(const QString& content, Settings::ColorEnum color, Api::RessourceManager *rm, QWidget *parent)
    : QLabel(parent)
{
    if (!content.isNull() && !content.isEmpty()) {
        html = "<html>";
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
                            html += "<code>";
                            i += 2;
                            block = true;
                            special = true;
                        } else {
                            multiCodeBlockEnd = 0;
                        }
                    } else {
                        if ((simpleCodeBlockEnd = content.indexOf('`', i + 1)) != -1) {
                            html += "<code>";
                            block = true;
                            special = true;
                        } else {
                            simpleCodeBlockEnd = 0;
                        }
                    }
                } else if (content[i] == '_') {
                    if (i + 1 < content.size() && content[i + 1] == '_') {
                        if (underlineEnd == 0 && (underlineEnd = content.indexOf("__", i + 2)) != -1) {
                            html += "<u>";
                            i++;
                            special = true;
                        } else {
                            underlineEnd = 0;
                        }
                    } else {
                        if (italicUnderscoreEnd == 0 && italicStarEnd == 0
                            && (italicUnderscoreEnd = content.indexOf('_', i + 1)) != -1) {
                                html += "<i>";
                                special = true;
                        } else {
                            italicUnderscoreEnd = 0;
                        }
                    }
                } else if (content[i] == '*') {
                    if (i + 1 < content.size() && content[i + 1] == '*') {
                        if (i + 2 < content.size() && content[i + 2] == '*') {
                            int end = 0;
                            if (boldEnd == 0 && italicStarEnd == 0 && italicUnderscoreEnd == 0
                                    && (end = content.indexOf("***", i + 3)) != -1
                                    && end < content.indexOf('\n', i + 3)
                                    && !content[i + 3].isSpace()) {
                                boldEnd = end;
                                italicStarEnd = end + 2;
                                html += "<i><b>";
                                i += 2;
                                special = true;
                            }
                        } else {
                            if (boldEnd == 0 && (boldEnd = content.indexOf("**", i + 2)) != -1 && !content[i + 2].isSpace()) {
                                html += "<b>";
                                i++;
                                special = true;
                            } else {
                                boldEnd = 0;
                            }
                        }
                    } else {
                        if (italicStarEnd == 0 && (italicStarEnd = content.indexOf('*', i + 1)) != -1 && !content[i + 1].isSpace() && content[i + 1] != '\n') {
                            html += "<i>";
                            special = true;
                        } else {
                            italicStarEnd = 0;
                        }
                    }
                } else if (i + 1 < content.size() && content[i] == '~' && content[i + 1] == '~') {
                    if (barredEnd == 0 && (barredEnd = content.indexOf("~~", i + 2)) != -1) {
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
                html += "</code>";
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
                    int specialIdEnd = content.indexOf('>', i + 1);
                    int newLineIndex = content.indexOf("\n", i + 1);
                    if (specialIdEnd != -1 && (specialIdEnd < newLineIndex || newLineIndex == -1)) {
                        QStringList specialTokens = content.mid(i + 1, specialIdEnd - i - 1).split(':');
                        int specialNameEnd = content.indexOf(':', i + 3);
                        if (specialTokens.size() == 3) {
                            if (specialTokens[0].isEmpty() || specialTokens[0][0] == 'a') { // emoji
                                QString fileName = specialTokens[2] + ".webp";
                                html += "<img src=\"cache/" + fileName + "\" height=\"" + QString::number(Settings::scale(20)) + "\" width=\"" + QString::number(Settings::scale(20)) + "\">";
                                rm->getImage([this](Api::CallbackStruct cb){this->update();}, "https://cdn.discordapp.com/emojis/" + fileName, fileName);
                            } else if (specialTokens[0][0] == 't') { // timestamp
                                html += processTimestamp(QDateTime::fromSecsSinceEpoch(specialTokens[1].toInt()).toLocalTime(), specialTokens[2][0].toLatin1());
                            }
                        } else {
                            if (specialTokens[0][0] == '#') { // channel
                                block = true;
                                rm->getGuilds([=](Api::CallbackStruct cb){
                                    Api::Snowflake channelId = Api::Snowflake(content.mid(i + 2, specialIdEnd - i - 2).toULongLong());
                                    QVector<Api::Guild *> guilds = *reinterpret_cast<QVector<Api::Guild *> *>(cb.data);
                                    for (int i = 0 ; i < guilds.size() ; i++) {
                                        for (int j = 0 ; j < guilds[i]->channels.size() ; j++) {
                                            if (channelId == guilds[i]->channels[j]->id)
                                                html += "<span class=\"highlight\">#" + guilds[i]->channels[j]->name + "</span>";
                                        }
                                    }
                                });
                            } else if (specialTokens[0][0] == '@') { // user
                                block = true;
                                int userIndex = html.size();
                                int savedOffset = usernamesOffset;
                                rm->getUser([=](Api::CallbackStruct cb){
                                    html.insert(userIndex + usernamesOffset - savedOffset, "<span class=\"highlight\">@" + reinterpret_cast<Api::User *>(cb.data)->username + "</span>");
                                    usernamesOffset += reinterpret_cast<Api::User *>(cb.data)->username.size() + 32;
                                    if (finished) this->setText(html);
                                }, specialTokens[0].mid(1).toULongLong());
                            } else {
                                html += content.mid(i + 1, specialIdEnd - i - 1);
                            }
                        }
                        i += specialIdEnd - i;
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
            QString style = "<style>"
                "*{color:" + Settings::colors[Settings::TextNormal].name() + ";}"
                "span{"
                    "background-color:" + Settings::colors[Settings::InteractiveMuted].name() + ";"
                    "border-radius:" + QString::number(Settings::scale(2)) + "px;"
                "}"
                "code{"
                    "background-color:" + Settings::colors[Settings::BackgroundSecondary].name() + ";"
                    "margin-bottom:0px;"
                "}"
                ".highlight{"
                    "background-color: hsl(235, calc(" + QString::number(Settings::saturation) + "*85.6%), 64.7%);"
                    "color:" + Settings::colors[Settings::BrandExperiment200].name() + ";"
                    "border-radius:" + QString::number(Settings::scale(3)) + "px;"
                    "padding: 0 " + QString::number(Settings::scale(2)) + "px;"
                "}"
                "</style>";
            html.insert(html.size(), style);
        }
        html += "</html>";

        QFont font;
        font.setPixelSize(Settings::scale(Settings::fontScaling));
        font.setFamily("whitney");
        this->setFont(font);
        this->setText(html);
        this->setWordWrap(true);
        this->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        this->setCursor(QCursor(Qt::IBeamCursor));
        this->setStyleSheet("color:" + Settings::colors[color].name());
        finished = true;
    }
}

} // namespace Ui
