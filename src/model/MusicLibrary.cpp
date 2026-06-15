#include "model/MusicLibrary.h"

Song* MusicLibrary::getSong(const std::string& path)
{
    for (const auto& song: all_songs)
    {
        if (song->getPath() == path)
            return song.get();
    }
    return nullptr;
}
