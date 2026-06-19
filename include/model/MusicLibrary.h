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

    const Song* getSong(const std::string& path);
    std::vector<Playlist> getPlaylistsRange(int starting_index, int n) const;
    size_t getPlaylistAmount() const { return playlists.size(); }
// private:
    std::vector<std::unique_ptr<Song>> all_songs;
    std::vector<Playlist> playlists;
};