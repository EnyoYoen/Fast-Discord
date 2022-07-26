#include "ui/common/basicwidgets.h"

Label::Label(QString textp, QWidget *parent) : QWidget(parent) {
    text = textp;
    textColor = Settings::None;
	this->setAttribute(Qt::WA_NoSystemBackground);
}

Label::Label(QWidget *parent) : QWidget(parent) {
    textColor = Settings::None;
}

void Label::setText(QString textp)
{
    text = textp;
    update();
}

void Label::setTextColor(Settings::ColorEnum color)
{
    textColor = color;
    update();
}

void Label::setTextColor(QColor color)
{
    tc = color;
    update();
}

void Label::setFont(QFont fontp)
{
    font = fontp;
    update();
}

void Label::setFlags(int flagsp)
{
    flags = flagsp;
    update();
}

void Label::setTextCoordinates(QRect coordinates)
{
    textCoords = coordinates;
    update();
}