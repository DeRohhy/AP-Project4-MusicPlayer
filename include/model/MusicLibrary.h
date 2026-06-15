#pragma once

#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "model/Song.h"
#include "model/Playlist.h"
#include "io/CsvLoader.h"
#include "io/M3uLoader.h"

const std::string LIBRARY = "data/library.csv";
const std::string PLAYLISTS = "data/playlists";
class MusicLibrary
{
public:
    MusicLibrary() :
        all_songs{ CsvLoader::loadAllMusic(LIBRARY) },
        playlists{ M3uLoader::loadPlaylists(PLAYLISTS, *this) } {}

    Song* getSong(const std::string& path);
private:
    std::vector<std::unique_ptr<Song>> all_songs;
    std::vector<Playlist> playlists;
};