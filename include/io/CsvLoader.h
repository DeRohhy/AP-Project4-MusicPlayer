#pragma once

#include <string>
#include <vector>

class Song;

class CsvLoader
{
public:
    std::vector<Song> loadAllMusic(const std::string& library_path);
};