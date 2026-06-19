#include "model/MusicLibrary.h"

const Song* MusicLibrary::getSong(const std::string& path)
{
    for (const auto& song: all_songs)
    {
        if (song->getPath() == path)
            return song.get();
    }
    return nullptr;
}

std::vector<Playlist> MusicLibrary::getPlaylistsRange(int starting_index, int n) const
{
    std::vector<Playlist> result;

    if (starting_index < 0 || starting_index >= playlists.size())
        return result;

    for (size_t i = starting_index; i < starting_index + n && i < playlists.size(); ++i)
        result.push_back(playlists[i]);

    return result;
}
