#include "ui/PlaylistView.h"
#include "model/Playlist.h"
#include "model/Song.h"

#include <string>

void PlaylistView::draw()
{
    // Clear the window before redrawing to prevent ghosting
    werase(window);
    drawOutline();

    if (!is_focused) is_searching = false;
    
    int start_y = 1;
    drawHeader(start_y);
    showControls(start_y);
    showSongs(start_y + 3);
    int sort_control_pos_y = getHeight() - SIDE_MARGIN * 2;
    showSortingControls(sort_control_pos_y);
    showSearchBox(sort_control_pos_y + 1);
    wnoutrefresh(window);
}

void PlaylistView::handleInput(int op)
{
    if (!is_focused)
        return;

    if (op == '\\') // clear all filters
    {
        search_query.clear();
        selected_sort = SortOption::NONE;
        revert_playlist = true;
        return;
    }

    if (is_searching)
    {
        handleSearchInput(op);
        return;
    }

    if (op - '0' >= 1 && op - '0' <= sorting_options.size())
        handleSortKey(op);

    switch (op)
    {
    case KEY_UP:
        handleKeyUp();
        break;
    case KEY_DOWN:
        handleKeyDown();
        break;
    case '/':
        is_searching = true;
        search_query = "";
        break;
    case '\\':
        search_query.clear();
        break;
    case '\n':
    case '\r':
    case KEY_ENTER:
        if (active_playlist.getSongAmount() == 0)
            break;
        controller.activatePlaylist(active_playlist.getPlaylistName());
        controller.playSong(active_playlist.getSong(selected_song)->getPath());
        break;
    }
}

void PlaylistView::loadPlaylist(const std::string& playlist_name)
{
    if (revert_playlist)
    {
        active_playlist = active_playlist_copy;
        songs_starting_index = selected_song = 0;
        revert_playlist = false;
        return;
    }

    if (playlist_name == active_playlist.getPlaylistName())
        return;

    auto result = music_library.getPlaylist(playlist_name);

    active_playlist = active_playlist_copy = result.has_value() ? result.value() : Playlist();
    songs_starting_index = selected_song = 0;
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

    if (is_focused && selected_song == index)
        wattron(window, COLOR_PAIR(1));
    else if (song->getPath() == music_player.getSoundPath())
        wattron(window, COLOR_PAIR(2));
    else 
        wattron(window, A_DIM);
    drawRow(start_y, first_row_cells);
    if (is_focused && selected_song == index)
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

void PlaylistView::showControls(int start_y)
{
    int gap = 1;
    wattron(window, A_DIM);
    wmove(window, start_y, SIDE_MARGIN * 2 + width / 2);
    wprintw(window, "[up][↑]");
    addPadding(gap);
    wprintw(window, "[down]↓");
    addPadding(gap);
    wprintw(window, "[enter]↵");
    wattroff(window, A_DIM);
}

void PlaylistView::showSortingControls(int start_y)
{
    drawDivider(start_y - 1);
    int gap = 1;
    wmove(window, start_y, SIDE_MARGIN);
    
    wattron(window, COLOR_PAIR(1));
    wprintw(window, "Sort by: ");
    wattroff(window, COLOR_PAIR(1));

    for (const auto& [title, field, key]: sorting_options)
    {
        wattron(window, selected_sort == field ? COLOR_PAIR(2) : A_DIM);
        wprintw(window, "[%c]%s", key, title.c_str());
        if (selected_sort == field)
        wprintw(window, is_sort_ascending ? "▲" : "▼");
        wattroff(window, selected_sort == field ? COLOR_PAIR(2) : A_DIM);
        addPadding(gap);
    }
}

void PlaylistView::showSearchBox(int start_y)
{
    wmove(window, start_y, SIDE_MARGIN);

    wattron(window, is_searching ? COLOR_PAIR(2) : COLOR_PAIR(1));
    wprintw(window, "[/]Search: ");
    wattroff(window, is_searching ? COLOR_PAIR(2) : COLOR_PAIR(1));

    
    wattron(window, A_DIM);
    wprintw(window, "%s", search_query.c_str());
    if (is_searching)
        wprintw(window, "▓");
    std::string controls = "[enter]↵ [\\]clear";
    mvwprintw(window, start_y + 1, width - controls.size(), "%s", controls.c_str());
    wattroff(window, A_DIM);
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

void PlaylistView::handleSortKey(int key)
{
    const SortOption new_sort = sorting_options[int(key - '0') - 1].option;
    if (new_sort == selected_sort)
    {
        if (is_sort_ascending)
            is_sort_ascending = false;
        else // turn off the sort
        {
            selected_sort = SortOption::NONE;
            revert_playlist = true;
            return;
        }
    }
    else
    {
        selected_sort = new_sort;
        is_sort_ascending = true;
    }

    search_query.clear();

    switch (key)
    {
        case '1':
            active_playlist.sortByTitle(!is_sort_ascending);
            break;
        case '2':
            active_playlist.sortByArtist(!is_sort_ascending);
            break;
        case '3':
            active_playlist.sortByAlbum(!is_sort_ascending);
            break;
        case '4':
            active_playlist.sortByYear(!is_sort_ascending);
            break;
        case '5':
            active_playlist.sortByDuration(!is_sort_ascending);
            break;

    }
}

void PlaylistView::handleSearchInput(int op)
{
    if (op == '\n' || op == '\r' || op == KEY_ENTER)
    {
        is_searching = false;
        return;
    }

    static constexpr int ASCII_DEL = 127;
    if ((op == KEY_BACKSPACE || op == ASCII_DEL) && !search_query.empty())
        search_query.pop_back();
    else if (!(search_query.empty() && op == ' ') && isprint(op)) // prevents getting queries with starting empty spaces
        search_query.push_back((char)op);
    
    performSearch();
}

void PlaylistView::performSearch()
{
    // remove leading spaces
    std::string sanitized_query = search_query;
    while (!sanitized_query.empty() && sanitized_query.back() == ' ')
        sanitized_query.pop_back();

    if (sanitized_query.empty())
    {
        revert_playlist = true;
        return;
    }
    selected_sort = SortOption::NONE;

    Playlist filtered;
    filtered.setPlaylistName(active_playlist_copy.getPlaylistName());

    for (size_t i = 0; i < active_playlist_copy.getSongAmount(); i++)
    {
        const Song* song = active_playlist_copy.getSong(i);
        const std::string queries[] = {
            song->getTitle(),
            song->getAlbum(),
            song->getArtist(),
        };

        // case-insensitive search
        for (const auto& query: queries)
        {
            auto it = std::search(
                query.begin(), query.end(),
                sanitized_query.begin(), sanitized_query.end(),
                [](char A, char B) 
                {
                    return std::tolower(A) == std::tolower(B);
                }
            );

            if (it != query.end())
            {
                filtered.addSong(song);
                break;
            }
        }
    }

    selected_song = songs_starting_index = 0;
    active_playlist = filtered;
}