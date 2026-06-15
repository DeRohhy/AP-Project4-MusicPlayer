#pragma once

#include <vector>
#include "model/Song.h"
#include "model/Playlist.h"

class MusicLibrary;

class M3uLoader
{
public:
    static std::vector<Playlist> loadPlaylists(const std::string& dir, MusicLibrary& library);
};