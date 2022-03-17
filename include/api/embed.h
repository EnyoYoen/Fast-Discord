#pragma once

#include "api/optional.h"

#include <QString>
#include <QVector>

namespace Api {

//https://discord.com/developers/docs/resources/channel#embed-object

struct EmbedFooter
{
    QString text;
    QString iconUrl;
    QString proxyIconUrl;
};

struct EmbedProvider
{
    QString name;
    QString url;
};

struct EmbedAuthor
{
    QString name;
    QString url;
    QString iconUrl;
    QString proxyIconUrl;
};

struct EmbedField
{
    QString name;
    QString value;
    optbool einline;
};

struct EmbedTVI
{
    QString url;
    QString proxyUrl;
    qint32  height;
    qint32  width;
};
typedef EmbedTVI EmbedThumbnail;
typedef EmbedTVI EmbedVideo;
typedef EmbedTVI EmbedImage;

struct Embed
{
    ~Embed()
    {
        delete footer;
        delete image;
        delete thumbnail;
        delete video;
        delete provider;
        delete author;
    }

    QVector<EmbedField *>  fields;
    EmbedFooter           *footer;
    EmbedImage            *image;
    EmbedThumbnail        *thumbnail;
    EmbedVideo            *video;
    EmbedProvider         *provider;
    EmbedAuthor           *author;
    QString                title;
    QString                type;
    QString                description;
    QString                url;
    QString                timestamp;
    qint32                 color;
};

} // namespace Api
