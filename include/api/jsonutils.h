#pragma once

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

#include <vector>

namespace Api {

std::vector<std::string> *getStringsFromJson(QJsonArray jsonArray);

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
void unmarshalMultiple(QJsonArray jsonArray, std::vector<T *> **objects)
{
    *objects = new std::vector<T *>;

    for (int i = 0 ; i < jsonArray.size() ; i++) {
        T *object;
        unmarshal<T>(jsonArray[i].toObject(), &object);
        (*objects)->push_back(object);
    }
}

// Unmarshal arrays of JSON objects
template <typename T>
void unmarshalMultiple(QJsonObject jsonObj, QString key, std::vector<T *> **objects)
{
    *objects = new std::vector<T *>;

    // Getting the JSON array at 'key'
    QJsonArray jsonArray = jsonObj[key].toArray();

    for (int i = 0 ; i < jsonArray.size() ; i++) {
        T *object;
        unmarshal<T>(jsonArray[i].toObject(), &object);
        (*objects)->push_back(object);
    }
}

} // namespace Api
