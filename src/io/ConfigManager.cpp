#include "io/ConfigManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

const std::string config_path = "data/settings.cfg";

ConfigManager::ConfigManager()
{
    load();
}

ConfigManager::~ConfigManager()
{
    save();
}

std::string ConfigManager::get(std::string key)
{
    if (!isValidField(key))
        return "";
    
    return config[key];
}

void ConfigManager::set(std::string key, std::string value)
{
    if (!isValidField(key))
        return;
    
    config[key] = value;
}

bool ConfigManager::isValidField(std::string key)
{
    static const std::unordered_set<std::string> ALLOWED_FIELDS
    {
        "shuffle",
        "playback_mode",
        "active_playlist",
        "focused_playlist",
        "last_played",
    };

    return ALLOWED_FIELDS.find(key) != ALLOWED_FIELDS.end();
}

void ConfigManager::load()
{
    std::ifstream file(config_path, std::ios::in);

    if (!file.is_open())
        throw std::runtime_error("Failed to open config file: " + config_path);
    

    std::string line;
    while(std::getline(file, line))
    {
        if (line.empty()) continue;

        std::stringstream ss(line);

        std::string key;
        std::getline(ss, key, ':'); // settings are save in key:value format

        std::string value = "";
        getline(ss, value);

        if (isValidField(key))
            config[key] = value;
    }
}

void ConfigManager::save()
{
    std::ofstream file(config_path, std::ios::out);

    if (!file.is_open())
        throw std::runtime_error("Failed to open config file: " + config_path);

    for(const auto& [key, value]: config)
        file << key << ':' << value << '\n';
}