#ifndef TAPE_STORAGE_JSONCONFIG_H
#define TAPE_STORAGE_JSONCONFIG_H

#include "Config/IConfig.h"

#include "nlohmann/json.hpp"

using basic_json = nlohmann::basic_json<>;
using json = nlohmann::json;
using json_ptr = json::json_pointer;

class JsonConfig: public IConfig
{
protected:

    JsonConfig();
    explicit JsonConfig(json_ptr jsonPtr);

public:

    void update(const std::string& path, bool checkOnlyPath = false);
    void readFromFile(const std::string& path) override;

    basic_json& operator[] (const std::string& param);
    basic_json operator[] (const std::string& param) const;

protected:

    json config;
    json_ptr jsonPtr;

    std::string configPath;
};


#define JSON_CONFIG(ConfigName, JsonPtr)                                                                               \
    class ConfigName: public JsonConfig                                                                                \
    {                                                                                                                  \
    public:                                                                                                            \
                                                                                                                       \
        ConfigName();                                                                                                  \
    };                                                                                                                 \
                                                                                                                       \
    inline ConfigName::ConfigName()                                                                                    \
        : JsonConfig(JsonPtr##_json_pointer)


#endif // TAPE_STORAGE_JSONCONFIG_H
