#pragma once

#include "user.h"

#include <string>
#include <vector>

namespace Api {

//https://discord.com/developers/docs/resources/emoji
struct Emoji
{
    ~Emoji();

    User                     *user;
    std::vector<std::string> *roles;
    std::string              *id;
    std::string              *name;
    bool                      requireColons;
    bool                      managed;
    bool                      animated;
    bool                      available;
};

struct Reaction
{
    ~Reaction();

    Emoji *emoji;
    int   count;
    bool  me;
};

} // namespace Api
