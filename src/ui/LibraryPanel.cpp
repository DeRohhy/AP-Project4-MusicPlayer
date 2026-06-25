#include "ui/LibraryPanel.h"
#include <algorithm>

void LibraryPanel::draw()
{
    visible_playlists = music_library.getPlaylistsRange(playlists_start_index, MAX_VISIBLE_PLAYLISTS);

    // Clear the window before redrawing to prevent ghosting
    werase(window);
    drawOutline();

    int start_y = 1;
    drawHeader(start_y);
    drawDivider(start_y + 1);
    showPlaylists(start_y + 2);
    showControls();

    wnoutrefresh(window);
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
        controller.selectPlaylist(visible_playlists[selected_playlist].getPlaylistName());
        break;
    }

}

void LibraryPanel::drawHeader(int start_y)
{
    wattron(window, A_BOLD | COLOR_PAIR(1));
    mvwprintw(window, start_y, SIDE_MARGIN, "☰ Your Library");
    wattroff(window, A_BOLD | COLOR_PAIR(1));
}

void LibraryPanel::showPlaylists(int start_y)
{
    if (visible_playlists.empty())
    {
        wattron(window, A_DIM);
        mvwprintw(window, start_y, SIDE_MARGIN, "It's empty :(");
        wattroff(window, A_DIM);
        return;
    }
    
    int cur_y = start_y;
    for (size_t i = 0; i < visible_playlists.size(); ++i)
    {
        if (selected_playlist == i)
            wattron(window, COLOR_PAIR(1));
        else if (visible_playlists[i].getPlaylistName() == music_player.getPlaylistName())
            wattron(window, COLOR_PAIR(2));
        else
            wattron(window, A_DIM);
        
        const int max_name_len = 16;
        mvwprintw(window, cur_y++, SIDE_MARGIN,
                  "▶ %s", fit(visible_playlists[i].getPlaylistName(), max_name_len).c_str());
        
        if (selected_playlist == i)
            wattroff(window, COLOR_PAIR(1));
        else if (visible_playlists[i].getPlaylistName() == music_player.getPlaylistName())
            wattroff(window, COLOR_PAIR(2));
        else
            wattroff(window, A_DIM);
    }

    if (music_library.getPlaylistAmount() >= MAX_VISIBLE_PLAYLISTS)
    {
        drawScrollBar(start_y, width - SIDE_MARGIN,
                      MAX_VISIBLE_PLAYLISTS,
                      MAX_VISIBLE_PLAYLISTS, music_library.getPlaylistAmount(),
                      playlists_start_index
                     );
    }
}

void LibraryPanel::handleKeyUp()
{
    if (selected_playlist == 0) 
    {
        playlists_start_index = std::max(0, playlists_start_index - 1);
        visible_playlists = music_library.getPlaylistsRange(playlists_start_index, MAX_VISIBLE_PLAYLISTS);
    }
    else if (selected_playlist > 0)
        selected_playlist--;
}

void LibraryPanel::handleKeyDown()
{
    int visible_items = std::min(MAX_VISIBLE_PLAYLISTS, (int)music_library.getPlaylistAmount());
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
    int gap = 1;
    wattron(window, A_DIM);
    wmove(window, height - SIDE_MARGIN, SIDE_MARGIN);
    wprintw(window, "[up][↑]");
    addPadding(gap);
    wprintw(window, "[down]↓");
    addPadding(gap);
    wprintw(window, "[enter]↵");
    wattroff(window, A_DIM);
}