#pragma once

#include <string>
#include <vector>

class Song;

class CsvLoader
{
public:
    static std::vector<Song> loadAllMusic(const std::string& library_path);
};