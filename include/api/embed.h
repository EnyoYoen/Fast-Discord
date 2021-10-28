#pragma once

#include <string>
#include <vector>

namespace Api {

//https://discord.com/developers/docs/resources/channel#embed-object

struct EmbedFooter
{
    ~EmbedFooter();

    std::string *text;
    std::string *iconUrl;
    std::string *proxyIconUrl;
};

struct EmbedProvider
{
    ~EmbedProvider();

    std::string *name;
    std::string *url;
};

struct EmbedAuthor
{
    ~EmbedAuthor();

    std::string *name;
    std::string *url;
    std::string *iconUrl;
    std::string *proxyIconUrl;
};

struct EmbedField
{
    ~EmbedField();

    std::string *name;
    std::string *value;
    bool         einline;
};

struct EmbedTVI
{
    ~EmbedTVI();

    std::string *url;
    std::string *proxyUrl;
    int          height;
    int          width;
};
typedef EmbedTVI EmbedThumbnail;
typedef EmbedTVI EmbedVideo;
typedef EmbedTVI EmbedImage;

struct Embed
{
    ~Embed();

    std::vector<EmbedField *> *fields;
    EmbedFooter               *footer;
    EmbedImage                *image;
    EmbedThumbnail            *thumbnail;
    EmbedVideo                *video;
    EmbedProvider             *provider;
    EmbedAuthor               *author;
    std::string               *title;
    std::string               *type;
    std::string               *description;
    std::string               *url;
    std::string               *timestamp;
    int                        color;
};

} // namespace Api
