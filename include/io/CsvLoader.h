#pragma once

#include <string>
#include <vector>
#include <memory>

class Song;

class CsvLoader
{
public:
    static std::vector<std::unique_ptr<Song>> loadAllMusic(const std::string& library_path);
};