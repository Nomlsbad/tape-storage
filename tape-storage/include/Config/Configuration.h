#ifndef TAPE_STORAGE_CONFIGURATION_H
#define TAPE_STORAGE_CONFIGURATION_H

#include "Configs.h"
#include "JsonConfig.h"

#include <type_traits>

class Configuration
{
public:

    static void initialize(std::string path)
    {
        if (!std::filesystem::exists(path))
        {
            std::stringstream stringstream;
            stringstream << "File with name \"" << path << "\"doesn't exist";
            throw std::invalid_argument(std::move(stringstream).str());
        }
        configPath = std::move(path);
    }

    template<std::derived_from<JsonConfig> Config>
    static basic_json& get(const std::string& param)
    {
        static Config config;
        config.update(configPath, true);
        return config[param];
    }

private:

    static inline std::string configPath = "Configuration::initialize(): call me =(";
};

#endif // TAPE_STORAGE_CONFIGURATION_H
