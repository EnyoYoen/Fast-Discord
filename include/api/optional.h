#pragma once

#include <QtGlobal>

typedef qint8 optbool;

enum class Optional : qint8
{
    False,
    True,
    None
};