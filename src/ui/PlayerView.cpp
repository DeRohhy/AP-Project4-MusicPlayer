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

    if (is_focused)
    {
        wattron(window, COLOR_PAIR(2));   
        box(window, 0, 0);
        wattroff(window, COLOR_PAIR(2));   
    }
    else
        box(window, 0, 0);

    drawTitle();
    drawArtistAndAlbum();
    drawDuration();
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

void PlayerView::drawDuration()
{
    int cur_time = (int)music_player.getTime();
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
    
    addPadding(3);
    
    wattron(window, shuffle ? COLOR_PAIR(2) : A_DIM);
    wprintw(window, "[s]⇄");
    wattroff(window, shuffle ? COLOR_PAIR(2) : A_DIM);
    
    addPadding(3);
    wattron(window, A_DIM);
    wprintw(window, "[j]⏮");
    wattroff(window, A_DIM);
    addPadding(3);


    wattron(window, music_player.isPlaying() ? COLOR_PAIR(2) : A_DIM);
    wprintw(window, music_player.isPlaying() ? "[p]⏸" : "[p]⏵");
    wattroff(window, music_player.isPlaying() ? COLOR_PAIR(2) : A_DIM);
    addPadding(3);

    wattron(window, A_DIM);
    wprintw(window, "[l]⏭");
    wattroff(window, A_DIM);

    addPadding(3);
    wattron(window, playback_mode != "NO_REPEAT" ? COLOR_PAIR(2) : A_DIM);
    wprintw(window, "[r]%s", playback_mode == "REPEAT_ONE" ? "↻₁" : "↻");
    wattroff(window, playback_mode != "NO_REPEAT" ? COLOR_PAIR(2) : A_DIM);
}

void PlayerView::handleInput(int op)
{
    if (!is_focused)
        return;

    switch (op)
    {
    case 's':
        handleShuffle();
        break;
    case 'j':
        handlePreviousTrack();
        break;
    case 'p':
        handlePlay();
        break;
    case 'l':
        handleNextTrack();
        break;
    case 'r':
        handlePlaybackMode();
        break;
    }

    wrefresh(window);
}

void PlayerView::handleShuffle()
{
    if (config_manager.get("shuffle") == "1")
        config_manager.set("shuffle", "0");
    else
        config_manager.set("shuffle", "1");
}

void PlayerView::handleNextTrack()
{

}

void PlayerView::handlePreviousTrack()
{

}

void PlayerView::handlePlay()
{
    if (music_player.isPlaying())
        music_player.pause();
    else
        music_player.play();

}

void PlayerView::handlePlaybackMode()
{
    std::string cur_mode = config_manager.get("playback_mode"), new_mode;
    if (cur_mode == "NO_REPEAT")
        new_mode = "REPEAT_ALL";
    else if (cur_mode == "REPEAT_ALL")
        new_mode = "REPEAT_ONE";
    else
        new_mode = "NO_REPEAT";
    
    config_manager.set("playback_mode", new_mode);
}
