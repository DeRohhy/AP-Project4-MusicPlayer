#pragma once

#include <string>
#include <vector>

class Song
{
    friend class CsvLoader;

public:
    Song() = default;
    ~Song() = default;

    std::string getTitle() const { return title; }
    std::string getArtist() const { return artist; }
    std::string getAlbum() const { return album; }
    std::string getGenre() const { return genre; }
    int getYear() const { return year; }
    int getDuration() const { return duration_sec; }
    std::string getPath() const { return file_path; }

private:
    std::string title;
    std::string artist;
    std::string album;
    std::string genre;
    int year = 0;
    int duration_sec = 0;
    std::string file_path;
};