#pragma once

#include <vector>
#include "model/Song.h"

class Playlist
{
public:
    Playlist() = default;

    void addSong(Song* song) { songs.push_back(song); }
// private:
    std::vector<Song*> songs;    
};