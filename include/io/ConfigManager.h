#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

class ConfigManager
{
public:
    ConfigManager();
    ~ConfigManager();

    std::string get(std::string key);
    void set(std::string key, std::string value);

private:    
    std::unordered_map<std::string, std::string> config;
    
    bool isValidField(std::string field);
    
    void load();
    void save();
};