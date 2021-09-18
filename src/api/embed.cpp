#include "../../include/api/embed.h"

namespace Api {

EmbedFooter::~EmbedFooter()
{
    delete text, iconUrl, proxyIconUrl;
}

EmbedProvider::~EmbedProvider()
{
    delete name, url;
}

EmbedAuthor::~EmbedAuthor()
{
    delete name, url, iconUrl, proxyIconUrl;
}

EmbedField::~EmbedField()
{
    delete name, value;
}

EmbedTVI::~EmbedTVI()
{
    delete url, proxyUrl;
}

Embed::~Embed()
{
    delete fields, footer, image, thumbnail, video, provider, author, author, title, type, description, url, timestamp;
}

} // namespace Api
