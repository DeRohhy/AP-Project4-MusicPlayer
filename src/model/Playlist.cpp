#include "model/Playlist.h"

#include <algorithm>

void Playlist::sortByTitle(bool sort_descending)
{
    std::sort(songs.begin(), songs.end(),
    [&](const Song* A, const Song* B)
    {
        if (sort_descending)
            return A->getArtist() > B->getArtist();
        return A->getTitle() < B->getTitle();
    }
    );
}

void Playlist::sortByArtist(bool sort_descending) 
{
    std::sort(songs.begin(), songs.end(),
    [&](const Song* A, const Song* B)
    {
        if (sort_descending)
            return A->getArtist() > B->getArtist();
        return A->getArtist() < B->getArtist();
    }
    );
}
void Playlist::sortByAlbum(bool sort_descending)
{
    std::sort(songs.begin(), songs.end(),
    [&](const Song* A, const Song* B)
    {
        if (sort_descending)
            return A->getAlbum() > B->getAlbum();
        return A->getAlbum() < B->getAlbum();
    }
    );
}

void Playlist::sortByGenre(bool sort_descending)
{
    std::sort(songs.begin(), songs.end(),
    [&](const Song* A, const Song* B)
    {
        if (sort_descending)
            return A->getGenre() > B->getGenre();
        return A->getGenre() < B->getGenre();
    }
    );
}

void Playlist::sortByYear(bool sort_descending)
{
    std::sort(songs.begin(), songs.end(),
    [&](const Song* A, const Song* B)
    {
        if (sort_descending)
            return A->getYear() > B->getYear();
        return A->getYear() < B->getYear();
    }
    );
}

void Playlist::sortByDuration(bool sort_descending)
{
    std::sort(songs.begin(), songs.end(),
    [&](const Song* A, const Song* B)
    {
        if (sort_descending)
            return A->getDuration() > B->getDuration();
        return A->getDuration() < B->getDuration();
    }
    );
}