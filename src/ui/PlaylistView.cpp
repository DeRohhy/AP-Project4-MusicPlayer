#include "ui/PlaylistView.h"
#include "model/Playlist.h"
#include "model/Song.h"

#include <string>

void PlaylistView::draw()
{
    std::string playlist_name = config_manager.get("active_playlist");
    auto result = music_library.getPlaylist(playlist_name);
    
    if (!result.has_value())
        return;

    active_playlist = result.value();

    refresh();
    // Clear the window before redrawing to prevent ghosting
    werase(window);
    drawOutline();

    int start_y = 1;
    drawHeader(start_y);
    showSongs(start_y + 3);

    wrefresh(window);
}

void PlaylistView::handleInput(int op)
{
    if (!is_focused)
        return;

    switch (op)
    {
    case KEY_UP:
        handleKeyUp();
        break;
    case KEY_DOWN:
        handleKeyDown();
        break;
    case '\n':
    case '\r':
    case KEY_ENTER:
        handlePlaySong();
        
        break;
    }

}

void PlaylistView::drawHeader(int start_y)
{
    wattron(window, A_DIM);
    mvwprintw(window, start_y, SIDE_MARGIN, "Playlist");
    wattroff(window, A_DIM);

    wattron(window, A_BOLD | COLOR_PAIR(1));

    const int max_playlist_name = 32;

    mvwprintw(window, start_y + 1, SIDE_MARGIN, "%s", fit(active_playlist.getPlaylistName(), max_playlist_name).c_str());
    wattroff(window, A_BOLD | COLOR_PAIR(1));

}

void PlaylistView::showInfoHelper(int start_y) const
{
    int gap = 2;
    int x = SIDE_MARGIN;
    wattron(window, A_DIM);
    for (const auto& [title, width]: columns)
    {
        mvwprintw(window, start_y, x, "%s", title.c_str());
        x += width + gap;
    }
    wattroff(window, A_DIM);
}

void PlaylistView::showSong(int start_y, int index, const Song* song)
{
    if (!song) return;

    int gap = 2;
    auto drawRow = [&](int y, const std::vector<std::string>& cells)
    {
        int x = SIDE_MARGIN;
        for (size_t i = 0; i < columns.size(); i++)
        {
            mvwprintw(window, y, x, "%s", fit(cells[i], columns[i].width).c_str());
            x += columns[i].width + gap; 
        }

    };

    const std::vector<std::string> first_row_cells{
        std::to_string(index + 1),
        song->getTitle(),
        song->getAlbum(),
        timeToStr(song->getDuration()),
    };
    const std::vector<std::string> second_row_cells{
        "",
        song->getArtist() + " • " + std::to_string(song->getYear()),
        song->getGenre(),
        "",
    };

    if (selected_song == index)
        wattron(window, COLOR_PAIR(1));
    else if (song->getPath() == music_player.getSoundPath())
        wattron(window, COLOR_PAIR(2));
    else 
        wattron(window, A_DIM);
    drawRow(start_y, first_row_cells);
    if (selected_song == index)
        wattroff(window, COLOR_PAIR(1));
    else if (song->getPath() == music_player.getSoundPath())
        wattroff(window, COLOR_PAIR(2));
    else 
        wattroff(window, A_DIM);

    wattron(window, A_DIM);
    drawRow(start_y + 1, second_row_cells);
    wattroff(window, A_DIM);


}

void PlaylistView::showSongs(int start_y)
{
    showInfoHelper(start_y);
    drawDivider(start_y + 1);


    int y = start_y + 2;
    for (size_t i = songs_starting_index; i < songs_starting_index + MAX_VISIBLE_SONGS && i < active_playlist.getSongAmount(); i++)
    {
        const Song* cur_song = active_playlist.getSong(i);
        if (!cur_song) continue;

        showSong(y, i, cur_song);

        y += 3;
    }

    if (active_playlist.getSongAmount() > MAX_VISIBLE_SONGS)
    {
        drawScrollBar(start_y + 2, width - 2,
                      MAX_VISIBLE_SONGS * 3 - 1,
                      MAX_VISIBLE_SONGS, active_playlist.getSongAmount(),
                      songs_starting_index
                    );
    }
}

void PlaylistView::handleKeyUp()
{
    if (selected_song <= 0)
        return;
    
    if (selected_song == songs_starting_index)
        songs_starting_index--;

    selected_song--;
}

void PlaylistView::handleKeyDown()
{
    if (selected_song + 1 >= active_playlist.getSongAmount())
        return;

    if (selected_song == songs_starting_index + MAX_VISIBLE_SONGS - 1)
        songs_starting_index++;

    selected_song++;
}

void PlaylistView::handlePlaySong()
{
    const Song* song = active_playlist.getSong(selected_song);

    if (!song)
        throw std::runtime_error("Could not located song");

    music_player.setSound(song->getPath());
    config_manager.set("last_played", song->getPath());
    music_player.play();
}