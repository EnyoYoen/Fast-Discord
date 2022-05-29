#pragma once

#include <QtGlobal>
#include <QString>

namespace Api {

struct Snowflake
{
    Snowflake() { value = 0; }
    Snowflake(quint64 val) { value = val; }

    quint64 discordTimestamp() const { return value >> 22; }
    quint64 unixTimestamp() const { return (value >> 22) + 1420070400000; }
    void clear() { value = 0; }

    friend bool operator==(const Snowflake& lhs, const Snowflake& rhs) { return lhs.value == rhs.value; }
    friend bool operator==(const Snowflake& lhs, int i) { return lhs.value == i; }
    friend bool operator!=(const Snowflake& lhs, const Snowflake& rhs) { return lhs.value != rhs.value; }
    friend bool operator<(const Snowflake& lhs, const Snowflake& rhs) { return lhs.value < rhs.value; }
    friend bool operator>(const Snowflake& lhs, const Snowflake& rhs) { return lhs.value > rhs.value; }
    friend QString operator+(const Snowflake& snow, const QString& str) { return QString::number(snow.value) + str; }
    friend QString operator+(const QString& str, const Snowflake& snow) { return str + QString::number(snow.value); }

    quint64 value = 0;
};


} // namespace Api