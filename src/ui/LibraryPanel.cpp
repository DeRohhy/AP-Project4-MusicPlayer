#include "ui/LibraryPanel.h"
#include <algorithm>

void LibraryPanel::draw()
{
    visible_playlists = music_library.getPlaylistsRange(playlists_start_index, MAX_PLAYLIST_VIEW);

    refresh();
    // Clear the window before redrawing to prevent ghosting
    werase(window);

    drawOutline();

    drawHeader();
    showPlaylists();
    showControls();

    wrefresh(window);
}

void LibraryPanel::handleInput(int op)
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
        config_manager.set("active_playlist", visible_playlists[selected_playlist].getPlaylistName());
        break;
    }

}

void LibraryPanel::drawHeader()
{
    wattron(window, A_BOLD | COLOR_PAIR(1));
    mvwprintw(window, 1, 2, "☰ Your Library");
    wattroff(window, A_BOLD | COLOR_PAIR(1));
}

void LibraryPanel::showPlaylists()
{
    if (visible_playlists.empty())
    {
        wattron(window, A_DIM);
        mvwprintw(window, 2, 2, "It's empty :(");
        wattroff(window, A_DIM);
        return;
    }
    
    int cur_y = 2;
    for (size_t i = 0; i < visible_playlists.size(); ++i)
    {
        if (is_focused && selected_playlist == i)
            wattron(window, COLOR_PAIR(1));
        else if (visible_playlists[i].getPlaylistName() == config_manager.get("active_playlist"))
            wattron(window, COLOR_PAIR(2));
        else
            wattron(window, A_DIM);
        
        int max_name_len = 16;
        mvwprintw(window, cur_y++, 2, "▶ %s",  fit(visible_playlists[i].getPlaylistName(), max_name_len).c_str());
        
        if (is_focused && selected_playlist == i)
            wattroff(window, COLOR_PAIR(1));
        else if (visible_playlists[i].getPlaylistName() == config_manager.get("active_playlist"))
            wattroff(window, COLOR_PAIR(2));
        else
            wattroff(window, A_DIM);
    }

    if (music_library.getPlaylistAmount() >= MAX_PLAYLIST_VIEW)
    {
        drawScrollBar(2, width - 2,
                      MAX_PLAYLIST_VIEW,
                      MAX_PLAYLIST_VIEW, music_library.getPlaylistAmount(),
                      playlists_start_index
                     );
    }
}

void LibraryPanel::handleKeyUp()
{
    if (selected_playlist == 0)
    {
        playlists_start_index = std::max(0, playlists_start_index - 1);
        visible_playlists = music_library.getPlaylistsRange(playlists_start_index, MAX_PLAYLIST_VIEW);
    }
    else if (selected_playlist > 0)
        selected_playlist--;
}

void LibraryPanel::handleKeyDown()
{
    int visible_items = std::min(MAX_PLAYLIST_VIEW, (int)music_library.getPlaylistAmount());
    if (selected_playlist + 1 == visible_items &&
        playlists_start_index + visible_items < (int)music_library.getPlaylistAmount()
    )
    {
        playlists_start_index++;
    }
    else if (selected_playlist + 1 < visible_items)
        selected_playlist++;
}

void LibraryPanel::showControls()
{
    wattron(window, A_DIM);
    wmove(window, height - 2, 2);
    wprintw(window, "[up][↑]");
    addPadding(1);
    wprintw(window, "[down]↓");
    addPadding(1);
    wprintw(window, "[enter]↵");
    wattroff(window, A_DIM);
}