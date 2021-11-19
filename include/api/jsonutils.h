#pragma once

#include "nlohmann/json.hpp"

#include <vector>

using json = nlohmann::json;

namespace Api {

// Template function that we specialize with all Discord's API JSON objects that we can recieve, to unmarshal them
template <typename T>
void unmarshalObj(json jsonObj, T **object);

// Unmarshal a JSON object
template <typename T>
bool unmarshal(json jsonObj, const std::string& key, T **object)
{
    try {
        // Getting the JSON object at 'key', if not empty
        jsonObj = key == static_cast<std::string>("") ? jsonObj : jsonObj.at(key);

        unmarshalObj<T>(jsonObj, object); // Unmarshal the object
        return true; // Everything's all right, no exceptions
    } catch(std::exception&) {
        *object = nullptr;
        return false; // There was an error
    }
}

// Unmarshal arrays of JSON objects
template <typename T>
bool unmarshalMultiple(json jsonObjs, const std::string& key, std::vector<T *> **objects)
{
    try {
        // Getting the JSON object at 'key', if not empty
        jsonObjs = key == static_cast<std::string>("") ? jsonObjs : jsonObjs.at(key);
        *objects = new std::vector<T *>;

        for (unsigned int i = 0 ; i < jsonObjs.size() ; i++) {
            T *object;
            if (unmarshal<T>(jsonObjs[i], "", &object) == false) return false;
                // There was an exception
            (*objects)->push_back(object);
        }

        return true;
    } catch(std::exception&) {
        *objects = nullptr;
        return false; // There is nothing at the key specified
    }
}

} // namespace Api
