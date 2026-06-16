#include "model/Playlist.h"

#include <algorithm>

void Playlist::sortByTitle()
{
    std::sort(songs.begin(), songs.end(),
    [&](Song* A, Song* B)
    {
        return A->getTitle() < B->getTitle();
    }
    );
}

void Playlist::sortByArtist()
{
    std::sort(songs.begin(), songs.end(),
    [&](Song* A, Song* B)
    {
        return A->getArtist() < B->getArtist();
    }
    );
}
void Playlist::sortByAlbum()
{
    std::sort(songs.begin(), songs.end(),
    [&](Song* A, Song* B)
    {
        return A->getAlbum() < B->getAlbum();
    }
    );
}

void Playlist::sortByGenre()
{
    std::sort(songs.begin(), songs.end(),
    [&](Song* A, Song* B)
    {
        return A->getGenre() < B->getGenre();
    }
    );
}

void Playlist::sortByYear(bool sort_descending)
{
    std::sort(songs.begin(), songs.end(),
    [&](Song* A, Song* B)
    {
        if (sort_descending)
            return A->getYear() > B->getYear();
        return A->getYear() < B->getYear();
    }
    );
}

void Playlist::sortByDuration()
{
    std::sort(songs.begin(), songs.end(),
    [&](Song* A, Song* B)
    {
        return A->getDuration() < B->getDuration();
    }
    );
}