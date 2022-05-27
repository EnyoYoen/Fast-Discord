#pragma once

#include <QString>

namespace Api {

struct Error
{
    QString message;
    QString strCode;
    int     intCode;
};

} // namespace Api
