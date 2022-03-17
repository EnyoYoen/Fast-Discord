#pragma once

#include <QString>
#include <QVector>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

namespace Api {

QVector<QString> getStringsFromJson(QJsonArray jsonArray);

// Template function that we specialize with all Discord's API JSON objects that we can recieve, to unmarshal them
template <typename T>
void unmarshal(QJsonObject jsonObj, T **object);

template <typename T>
void unmarshal(QJsonValue jsonVal, const QString& key, T **object)
{
    unmarshal(jsonVal[key].toObject(), object);
}

// Unmarshal arrays of JSON objects
template <typename T>
QVector<T*> unmarshalMultiple(QJsonArray jsonArray)
{
    QVector<T*> objects;

    for (int i = 0 ; i < jsonArray.size() ; i++) {
        T *object;
        unmarshal<T>(jsonArray[i].toObject(), &object);
        objects.push_back(object);
    }

    return objects;
}

// Unmarshal arrays of JSON objects
template <typename T>
QVector<T*> unmarshalMultiple(QJsonObject jsonObj, QString key)
{
    QVector<T*> objects;

    // Getting the JSON array at 'key'
    QJsonArray jsonArray = jsonObj[key].toArray();

    for (int i = 0 ; i < jsonArray.size() ; i++) {
        T *object;
        unmarshal<T>(jsonArray[i].toObject(), &object);
        objects.push_back(object);
    }

    return objects;
}

} // namespace Api
