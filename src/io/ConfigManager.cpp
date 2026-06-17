#include "io/ConfigManager.h"

#include <fstream>
#include <unordered_set>
#include <sstream>

const std::string config_path = "data/settings.cfg";

ConfigManager::ConfigManager()
{
    loadConfig();
}

void ConfigManager::loadConfig()
{
    static const std::unordered_set<std::string> allowed_fields
    {
        "last_played",
        "active_playlist"
    };

    std::ifstream file(config_path, std::ios::in);

    if (!file.is_open())
        throw std::runtime_error("Failed to open config file: " + config_path);
    

    std::string line;
    while(std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);

        std::string key;
        std::getline(ss, key, ':');

        std::string value = "";
        ss >> value;

        if (allowed_fields.find(key) != allowed_fields.end())
            config[key] = value;
    }
}