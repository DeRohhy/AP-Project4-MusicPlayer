#pragma once

#include <vector>
#include <string>
#include "model/Song.h"

class Playlist
{
public:
    Playlist() = default;

    std::string getPlaylistName() const { return playlist_name; }

    void setPlaylistName(std::string name) { playlist_name = name; }
    void addSong(const Song* song) { songs.push_back(song); }
    size_t getSongAmount() const { return songs.size(); }
    const Song* getSong(int i) const { return songs[i]; }

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