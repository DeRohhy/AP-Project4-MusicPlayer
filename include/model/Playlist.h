#pragma once

#include <vector>
#include <string>
#include "model/Song.h"

class Playlist
{
public:
    Playlist() = default;

    std::string getPlaylistName() { return playlist_name; }

    std::string getPlaylistName() const { return playlist_name; }

    void setPlaylistName(std::string name) { playlist_name = name; }
    void addSong(const Song* song) { songs.push_back(song); }

    void sortByTitle();
    void sortByArtist();
    void sortByAlbum();
    void sortByGenre();
    void sortByYear(bool sort_descending = false);
    void sortByDuration();

private:
    std::string playlist_name;
    std::vector<const Song*> songs;
};