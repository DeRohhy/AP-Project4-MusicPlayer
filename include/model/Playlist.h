#pragma once

#include <vector>
#include "model/Song.h"
#include <iostream>

class Playlist
{
public:
    Playlist() = default;

    void addSong(Song* song) { songs.push_back(song); }

    void sortByTitle();
    void sortByArtist();
    void sortByAlbum();
    void sortByGenre();
    void sortByYear(bool sort_descending = false);
    void sortByDuration();

private:
    std::vector<Song*> songs;
};