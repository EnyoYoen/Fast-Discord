#pragma once

#include <string>

namespace Api {

//https://discord.com/developers/docs/resources/user#user-object
struct User
{
    ~User();

    std::string *username;
    std::string *discriminator;
    std::string *avatar;
    std::string *locale;
    std::string *email;
    std::string *id;
    int          flags;
    int          premiumType;
    int          publicFlags;
    bool         bot;
    bool         system;
    bool         mfaEnabled;
    bool         verified;
};

} // namespace Api
