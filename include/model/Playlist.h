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

    void sortByTitle(bool sort_descending = false);
    void sortByArtist(bool sort_descending = false);
    void sortByAlbum(bool sort_descending = false);
    void sortByGenre(bool sort_descending = false);
    void sortByYear(bool sort_descending = false);
    void sortByDuration(bool sort_descending = false);

private:
    std::string playlist_name;
    std::vector<const Song*> songs;
};