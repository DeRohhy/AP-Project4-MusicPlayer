#include "ui/PlayerView.h"

#include <ncurses.h>
#include <iostream>
#include <sstream>
#include <iomanip>

void PlayerView::draw()
{
    if (song == nullptr)
        return;
    
    // Clear the window before redrawing to prevent ghosting
    werase(window);
    box(window, 0, 0);

    drawTitle();
    drawArtistAndAlbum();
    drawDuration(100);
    drawControls();

    wrefresh(window);
}

void PlayerView::drawTitle()
{
    wattron(window, A_BOLD | COLOR_PAIR(1));
    
    std::ostringstream ss;
    ss << "♫ " << fit(song->getTitle(), MAX_TITLE_LEN);
    mvwprintw(window, 1, 2, ss.str().c_str());
    
    wattroff(window, A_BOLD | COLOR_PAIR(1));
}

void PlayerView::drawArtistAndAlbum()
{
    wattron(window, A_DIM);

    std::ostringstream ss;
    ss << fit(song->getArtist(), MAX_ARTIST_LEN) 
       << " • " << fit(song->getAlbum(), MAX_ALBUM_LEN);
       
    mvwprintw(window, 2, 2, ss.str().c_str());
    
    wattroff(window, A_DIM);   
}

void PlayerView::drawDuration(int cur_time)
{
    int full = ((double)cur_time / (double)song->getDuration()) * DURATION_BAR_LEN;
    int remaining = DURATION_BAR_LEN - full;

    wmove(window, 1, 40);

    wattron(window, A_DIM);
    waddstr(window, timeToStr(cur_time).c_str());
    wattroff(window, A_DIM);

    addPadding(1);

    // draw the completed progress bar (Color 1, Bold)
    wattron(window, A_BOLD | COLOR_PAIR(1));
    for (int i = 0; i + 1 < full; i++) 
        waddstr(window, "─"); 
    waddstr(window, "◉"); // Current position indicator
    wattroff(window, A_BOLD | COLOR_PAIR(1));


    // draw the remaining progress bar (Color 2)
    wattron(window, A_DIM);
    for (int i = 0; i + 1 < remaining; i++)
        waddstr(window, "─");
    wattroff(window, A_DIM);

    addPadding(1);

    wattron(window, A_DIM);
    waddstr(window, timeToStr(song->getDuration()).c_str());
    wattroff(window, A_DIM);
}

void PlayerView::drawControls()
{
    bool shuffle = config_manager.get("shuffle") == "1";
    std::string playback_mode = config_manager.get("playback_mode"); 
    
    wmove(window, 2, 40);
    
    addPadding(2);
    
    wattron(window, shuffle ? COLOR_PAIR(3) : A_DIM);
    wprintw(window, "[s]⇄");
    wattroff(window, shuffle ? COLOR_PAIR(3) : A_DIM);
    
    addPadding(3);
    wattron(window, A_DIM);
    wprintw(window, "[j]⏮");
    addPadding(3);
    wprintw(window, "[p]⏵");
    addPadding(3);
    wprintw(window, "[l]⏭");
    wattroff(window, A_DIM);


    addPadding(3);
    wattron(window, playback_mode != "NO_REPEAT" ? COLOR_PAIR(3) : A_DIM);
    wprintw(window, "[r]%s", playback_mode == "REPEAT_ONE" ? "↻₁" : "↻");
    wattron(window, playback_mode != "NO_REPEAT" ? COLOR_PAIR(3) : A_DIM);
}