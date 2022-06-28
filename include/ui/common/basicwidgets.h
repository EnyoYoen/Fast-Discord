#pragma once

#include "settings/settings.h"

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QMovie>

#define CUSTOM_WIDGET(CODE)\
protected:\
	QMovie *mov = nullptr;\
private:\
	QImage img;\
	QPixmap pm;\
	QColor bgc = Settings::colors[Settings::None];\
	Settings::ColorEnum bg = Settings::None;\
	Settings::ColorEnum bd = Settings::None;\
	QRect imgCoords;\
	int brtl = 0, brtr = 0, brbl = 0, brbr = 0;\
	int bdt = 0, bdl = 0, bdb = 0, bdr = 0;\
	bool imgCentered;\
public:\
	void setMovie(QMovie *movie, bool centered = true, QRect coords = QRect())\
	{\
		img = QImage();\
		imgCentered = centered;\
		mov = movie;\
		imgCoords = coords;\
		update();\
	}\
	void setImage(QString file, bool centered = true, QRect coords = QRect())\
	{\
		imgCentered = centered;\
		img.load(file);\
		imgCoords = coords;\
		update();\
	}\
	void setPixmap(QPixmap pixmap, bool centered = true, QRect coords = QRect())\
	{\
		img = QImage();\
		imgCentered = centered;\
		pm = pixmap;\
		imgCoords = coords;\
		update();\
	}\
	void setBackgroundColor(Settings::ColorEnum color)\
	{\
		bg = color;\
		update();\
	}\
	void setBackgroundColor(QColor color)\
	{\
		bgc = color;\
		update();\
	}\
	void setBorderColor(Settings::ColorEnum color)\
	{\
		bd = color;\
		update();\
	}\
	void setBorderSize(int border)\
	{\
		bdt = bdr = bdb = bdl = border;\
		update();\
	}\
	void setBorderSize(int bdtp, int bdrp, int bdbp, int bdlp)\
	{\
		bdt = bdtp;\
		bdr = bdrp;\
		bdb = bdbp;\
		bdl = bdlp;\
		update();\
	}\
	void setBorderRadius(int br)\
	{\
		brtl = brtr = brbl = brbr = br;\
		update();\
	}\
	void setBorderRadius(int brtlp, int brtrp, int brblp, int brbrp)\
	{\
		brtl = brtlp;\
		brtr = brtrp;\
		brbl = brblp;\
		brbr = brbrp;\
		update();\
	}\
protected:\
	void paintEvent(QPaintEvent *event)\
	{\
		QPainter p(this);\
		p.setRenderHint(QPainter::Antialiasing);\
        p.setRenderHint(QPainter::SmoothPixmapTransform);\
		p.setPen(Qt::NoPen);\
\
		int w = this->width(), h = this->height();\
\
		QPainterPath path;\
		QBrush brush((bgc == Settings::colors[Settings::None] ? Settings::colors[bg] : bgc), Qt::SolidPattern);\
		p.setBrush(brush);\
		if (brtl == brtr && brtl == brbl && brtl == brbr) {\
            path.addRoundedRect(0, 0, w, h, brtl, brbl);\
			p.drawPath(path);\
		} else {\
			QVector<QPoint> points;\
			if (brtl == 0) {\
				points << QPoint(0, 0);\
			} else {\
				points << QPoint(0, brtl) << QPoint(brtl, brtl) << QPoint(brtl, 0);\
				p.drawPie(0, 0, 2*brtl, 2*brtl, 90*16, 90*16);\
			}\
			if (brtr == 0) {\
				points << QPoint(w, 0);\
			} else {\
				points << QPoint(w - brtr, 0) << QPoint(w - brtr, brtr) << QPoint(w, brtr);\
				p.drawPie(w-2*brtr, 0, 2*brtr, 2*brtr, 360*16, 90*16);\
			}\
			if (brbr == 0) {\
				points << QPoint(w, h);\
			} else {\
				points << QPoint(w, h - brbr) << QPoint(w - brbr, h - brbr) << QPoint(w - brbr, h);\
				p.drawPie(w-2*brbr, h-2*brbr, 2*brbr, 2*brbr, 270*16, 90*16);\
			}\
			if (brbl == 0) {\
				points << QPoint(0, h);\
			} else {\
				points << QPoint(brbl, h) << QPoint(brbl, h - brbl) << QPoint(0, h - brbl);\
				p.drawPie(0, h-2*brbl, 2*brbl, 2*brbl, 180*16, 90*16);\
			}\
\
			QPolygon poly(points);\
			path.addPolygon(poly);\
			p.drawPath(path);\
		}\
\
		if (bdt) {\
			QPen pen(Settings::colors[bd]);\
			pen.setWidth(bdt);\
			p.setPen(pen);\
			p.drawLine(brtl, bdt/2, w - brtr, bdt/2);\
			p.drawArc(bdt/2, bdt/2, 2*brtl, 2*brtl, 90*16, 45*16);\
			p.drawArc(w-2*brtr, bdt/2, 2*brtr-bdt/2, 2*brtr, 45*16, 45*16);\
		}\
		if (bdl) {\
			QPen pen(Settings::colors[bd]);\
			pen.setWidth(bdl);\
			p.setPen(pen);\
			p.drawLine(bdt/2, brtl, bdt/2, h-brbl);\
			p.drawArc(bdt/2, bdt/2, 2*brtl, 2*brtl, 135*16, 45*16);\
			p.drawArc(bdt/2, h-2*brbl, 2*brbl, 2*brbl-bdt/2, 180*16, 45*16);\
		}\
		if (bdb) {\
			QPen pen(Settings::colors[bd]);\
			pen.setWidth(bdb);\
			p.setPen(pen);\
			p.drawLine(brbl, h-bdt/2, w - brbr, h-bdt/2);\
			p.drawArc(bdt/2, h-2*brbl, 2*brbl, 2*brbl-bdt/2, 225*16, 45*16);\
			p.drawArc(w-2*brbr, h-2*brbr, 2*brbr-bdt/2, 2*brbr-bdt/2, 270*16, 45*16);\
		}\
		if (bdr) {\
			QPen pen(Settings::colors[bd]);\
			pen.setWidth(bdr);\
			p.setPen(pen);\
			p.drawLine(w-bdt/2, brtr, w-bdt/2, h-brbr);\
			p.drawArc(w-2*brtr, bdt/2, 2*brtr-bdt/2, 2*brtr, 360*16, 45*16);\
			p.drawArc(w-2*brbr, h-2*brbr, 2*brbr-bdt/2, 2*brbr-bdt/2, 315*16, 45*16);\
		}\
\
		if (mov != nullptr) pm = mov->currentPixmap().scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);\
		if (!img.isNull() || !pm.isNull()) {\
			p.setClipPath(path);\
			if (img.isNull()) {\
				p.drawPixmap(QRect((this->width() - pm.width())/2, (this->height() - pm.height())/2, pm.width(), pm.height()), pm);\
			} else {\
				p.drawImage(QRect((img.width() < this->width() ? (this->width() - img.width())/2 : 0),\
					(img.height() < this->height() ? (this->height() - img.height())/2 : 0),\
					(img.width() < this->width() ? img.width() : this->width()),\
					(img.height() < this->height() ? img.height() : this->height())),\
					(img.height() < this->height() && img.width() < this->width() ? img : img.scaled(this->width(), this->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));\
			}\
		}\
\
		CODE\
	}

class Widget : public QWidget
{
	CUSTOM_WIDGET( )
public:
	Widget(QWidget *parent = nullptr) : QWidget(parent) {
		this->setAttribute(Qt::WA_NoSystemBackground);
	}
};

class Label : public QWidget
{
	CUSTOM_WIDGET(
		if (!text.isNull()) {
			p.setPen(QPen(Settings::colors[textColor]));
			p.setFont(font);
			p.drawText((textCoords.isNull() ? this->rect() : textCoords), flags, text);
		}
	)
public:
	Label(QString textp, QWidget *parent);
	Label(QWidget *parent);

	void setText(QString text);
	void setTextColor(Settings::ColorEnum color);
	void setFont(QFont fontp);
	void setFlags(int flagsp);
	void setTextCoordinates(QRect coordinates);
	
	QString text;

private:
	QFont font;
	QRect textCoords;
	Settings::ColorEnum textColor = Settings::None;
	int flags = Qt::AlignVCenter | Qt::AlignLeft;
};