#include "../../include/api/embed.h"

namespace Api {

EmbedFooter::~EmbedFooter()
{
    delete text;
    delete iconUrl;
    delete proxyIconUrl;
}

EmbedProvider::~EmbedProvider()
{
    delete name;
    delete url;
}

EmbedAuthor::~EmbedAuthor()
{
    delete name;
    delete url;
    delete iconUrl;
    delete proxyIconUrl;
}

EmbedField::~EmbedField()
{
    delete name;
    delete value;
}

EmbedTVI::~EmbedTVI()
{
    delete url;
    delete proxyUrl;
}

Embed::~Embed()
{
    delete fields;
    delete footer;
    delete image;
    delete thumbnail;
    delete video;
    delete provider;
    delete author;
    delete author;
    delete title;
    delete type;
    delete description;
    delete url;
    delete timestamp;
}

} // namespace Api
