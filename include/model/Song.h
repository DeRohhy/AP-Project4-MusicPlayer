#pragma once

#include <string>
#include <vector>

class Song
{
    friend class CsvLoader;

public:
    Song() = default;
    ~Song() = default;

    std::string getTitle() { return title; }
    std::string getArtist() { return artist; }
    std::string getAlbum() { return album; }
    std::string getGenre() { return genre; }
    int getYear() { return year; }
    int getDuration() { return duration_sec; }
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