#pragma once

#include <string>

namespace Api {

//https://discord.com/developers/docs/resources/channel#overwrite-object
struct Overwrite
{
    ~Overwrite();

    std::string *id;
    std::string *allow;
    std::string *deny;
    int          type;
};

} // namespace Api
