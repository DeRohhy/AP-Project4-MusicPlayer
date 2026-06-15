#pragma once

#include <string>
#include <vector>

class Song
{
    friend class CsvLoader;

public:
    Song() = default;
    ~Song() = default;

    std::string getPath() { return file_path; }

private:
    std::string title;
    std::string artist;
    std::string album;
    std::string genre;
    int year = 0;
    int duration_sec = 0;
    std::string file_path;
};