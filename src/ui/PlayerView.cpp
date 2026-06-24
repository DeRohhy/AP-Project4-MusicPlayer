#include "ui/PlayerView.h"

#include <ncurses.h>
#include <sstream>
#include <iomanip>

void PlayerView::draw()
{   
    song = music_library.getSong(music_player.getSoundPath());
    
    refresh();
    // Clear the window before redrawing to prevent ghosting
    werase(window);

    drawOutline();

    const int start_y = 1, start_x = 42;

    drawTitle(start_y);
    drawArtistAndAlbum(start_y + 1);
    drawDuration(start_y, start_x);
    drawControls(start_y + 1, start_x - 1);
    wrefresh(window);
}

void PlayerView::drawTitle(int start_y)
{
    if (!song)
        return;

    wattron(window, A_BOLD | COLOR_PAIR(1));
    
    std::ostringstream ss;
    ss << "♫ " << fit(song->getTitle(), MAX_TITLE_LEN);
    mvwprintw(window, start_y, SIDE_MARGIN, ss.str().c_str());
    
    wattroff(window, A_BOLD | COLOR_PAIR(1));
}

void PlayerView::drawArtistAndAlbum(int start_y)
{
    if (!song)
        return;

    wattron(window, A_DIM);

    std::ostringstream ss;
    ss << fit(song->getArtist(), MAX_ARTIST_LEN) 
       << " • " << fit(song->getAlbum(), MAX_ALBUM_LEN);
       
    mvwprintw(window, start_y, SIDE_MARGIN, ss.str().c_str());
    
    wattroff(window, A_DIM);   
}

void PlayerView::drawDuration(int start_y, int start_x)
{
    int cur_time = (int)music_player.getTime();
    int full = (song ? ((double)cur_time / (double)song->getDuration()) * DURATION_BAR_LEN : 0);
    int remaining = DURATION_BAR_LEN - full;
    int gap = 1;

    wmove(window, start_y, start_x);

    wattron(window, A_DIM);
    waddstr(window, timeToStr(cur_time).c_str());
    wattroff(window, A_DIM);

    addPadding(gap);

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

    addPadding(gap);

    wattron(window, A_DIM);
    waddstr(window, timeToStr(song ? song->getDuration() : 0).c_str());
    wattroff(window, A_DIM);
}

void PlayerView::drawControls(int start_y, int start_x)
{
    bool shuffle = music_player.isShuffle();
    bool is_playing = music_player.isPlaying();
    PlaybackMode playback_mode =  music_player.getPlaybackMode();
    int gap = 2;

    wmove(window, start_y, start_x);
    
    addPadding(gap);
    
    wattron(window, shuffle ? COLOR_PAIR(2) : A_DIM);
    wprintw(window, "[s]⇄");
    wattroff(window, shuffle ? COLOR_PAIR(2) : A_DIM);
    
    addPadding(gap);

    wattron(window, A_DIM);
    wprintw(window, "[j]⏮");
    wattroff(window, A_DIM);
    
    addPadding(gap);

    wattron(window, A_DIM);
    wprintw(window, "[←]-%d", SEEK_AMOUNT);
    wattroff(window, A_DIM);

    addPadding(gap);

    wattron(window, is_playing ? COLOR_PAIR(2) : A_DIM);
    wprintw(window, is_playing ? "[p]⏸" : "[p]⏵");
    wattroff(window, is_playing ? COLOR_PAIR(2) : A_DIM);

    addPadding(gap);

    wattron(window, A_DIM);
    wprintw(window, "[→]+%d", SEEK_AMOUNT);
    wattroff(window, A_DIM);

    addPadding(gap);

    wattron(window, A_DIM);
    wprintw(window, "[l]⏭");
    wattroff(window, A_DIM);

    addPadding(gap);
    wattron(window, playback_mode != PlaybackMode::NO_REPEAT ? COLOR_PAIR(2) : A_DIM);
    wprintw(window, "[r]%s", playback_mode == PlaybackMode::REPEAT_ONE ? "↻₁" : "↻");
    wattroff(window, playback_mode != PlaybackMode::NO_REPEAT ? COLOR_PAIR(2) : A_DIM);
}

void PlayerView::handleInput(int op)
{
    if (!is_focused)
        return;

    switch (op)
    {
    case 's':
        controller.toggleShuffle();
        break;
    case 'j':
        controller.previousTrack();
        break;
    case KEY_LEFT:
        controller.seek(-SEEK_AMOUNT);
        break;
    case 'p':
        controller.togglePlay();
        break;
    case KEY_RIGHT:
        controller.seek(SEEK_AMOUNT);
        break;
    case 'l':
        controller.nextTrack();
        break;
    case 'r':
        controller.changePlaybackMode();
        break;
    }

    wrefresh(window);
}