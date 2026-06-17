#pragma once

#include <string>
#include <unordered_map>

class ConfigManager
{
public:
    ConfigManager();
    
    std::unordered_map<std::string, std::string> config;
private:    

    void loadConfig();
};