#pragma once

#include <string>

class Song
{
public:
    Song() = default;
    ~Song() = default;
private:
    std::string title;
    std::string artist;
    std::string album;
    std::string genre;
    int year;
    int duration_sec;
    std::string file_path;
};