#include "io/M3uLoader.h"
#include "model/MusicLibrary.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

std::vector<Playlist> M3uLoader::loadPlaylists(const std::string& dir, MusicLibrary& library)
{
    std::vector<Playlist> playlists;

    for (const auto& entry: fs::directory_iterator(dir))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".m3u")
            continue;
        
        std::ifstream file(entry.path());

        if (!file) continue;

        Playlist playlist;
        std::string song_path;
        while (std::getline(file, song_path))
        {
            Song* song = library.getSong(song_path);

            if (song) playlist.addSong(song);
        }

        playlists.push_back(playlist);
    }

    return playlists;
}

