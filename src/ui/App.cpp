#include "ui/App.h"

#include <ncurses.h>
#include <algorithm>
#include <vector>

#include <iostream>

App::App()
{
    initscr();
    cbreak(); // allow break from app with control + c
    noecho(); // doesnt show user input on screen
    keypad(stdscr, TRUE);
    curs_set(false); // make cursor invisible
    timeout(100); // ignores user input every 100 milliseconds

    if (has_colors())
    {
        start_color();
        use_default_colors();
        
        init_pair(1, COLOR_WHITE, -1); // main text        
        init_pair(2, COLOR_GREEN, -1); // accent color
        init_pair(3, COLOR_RED, -1); // error color
    }
}

App::~App()
{
    keypad(stdscr, FALSE);
    curs_set(true);
    endwin();
}

void App::run()
{
    load_last_data();
    
    init_player_view();
    init_library_panel();
    init_playlist_view();

    std::vector<UIComponent*> components = {
        player_view.get(),
        library_panel.get(),
        playlist_view.get(),
    };
    int focused_index = 0;
    components[focused_index]->setFocus(true);

    bool running = true;

    draw();
    while (running)
    {
        draw();

        if (music_player.isFinished())
            advanceTrack();

        playlist_view->loadPlaylist(config_manager.get("focused_playlist"));        

        int op = getch();
        switch (op)
        {
        case KEY_RESIZE:
            clear();
            refresh();
            
            for (auto* ui_window: components)
                ui_window->revertWindowSize();
            
            break;
        case 'q':
        case 'Q':
            running = false;
            break;
        case '\t': // switch focus when user hits tab
            components[focused_index]->setFocus(false);
            focused_index = (focused_index + 1) % components.size();
            components[focused_index]->setFocus(true);
            break;
        default:
            components[focused_index]->handleInput(op);
        }

        refresh();
    }

}

void App::draw()
{
    int cur_screen_height, cur_screen_width;
    getmaxyx(stdscr, cur_screen_height, cur_screen_width);

    if (cur_screen_height < SCREEN_HEIGHT || cur_screen_width < SCREEN_WIDTH)
    {
        erase(); // erase stdscr buffer, no forced full-screen clearok
        attron(COLOR_PAIR(3));
        mvprintw(cur_screen_height / 2 - 1, 0, "Terminal too small!");
        mvprintw(cur_screen_height / 2, 0,
            "Please resize to at least %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
        attroff(COLOR_PAIR(3));

        attron(A_DIM);
        mvprintw(cur_screen_height / 2 + 1, 0, "[q] quit");
        attroff(A_DIM);
        wnoutrefresh(stdscr);
        doupdate();
        return;
    }

    erase();
    wnoutrefresh(stdscr);

    player_view->draw();
    library_panel->draw();
    playlist_view->draw();

    doupdate(); // single atomic screen update
}

void App::togglePlay() 
{
    if (music_player.isPlaying())
        music_player.pause();
    else
        music_player.play();
}
void App::toggleShuffle() 
{
    bool cur = music_player.isShuffle();
    music_player.setShuffle(!cur);
    config_manager.set("shuffle", std::to_string(!cur));
}

void App::nextTrack()
{
    music_player.nextSong();
    if (music_player.isSoundInitialized())
        config_manager.set("last_played", music_player.getSoundPath());
    music_player.play();
}

void App::previousTrack()
{
    music_player.previousSong();
    if (music_player.isSoundInitialized())
        config_manager.set("last_played", music_player.getSoundPath());
    music_player.play();
}

void App::advanceTrack()
{
    if (music_player.getPlaybackMode() == PlaybackMode::NO_REPEAT && music_player.isAtEndOfQueue())
        return;
    if (music_player.getPlaybackMode() == PlaybackMode::REPEAT_ONE)
    {
        music_player.play();
        return;
    }

    nextTrack();
}

void App::changePlaybackMode() // NO_REPEAT -> REPEAT_ALL -> REPEAT_ONE -> NO_REPEAT
{
    PlaybackMode cur_mode = music_player.getPlaybackMode();
    if (cur_mode == PlaybackMode::NO_REPEAT)
        music_player.setPlaybackMode(PlaybackMode::REPEAT_ALL);
    else if (cur_mode == PlaybackMode::REPEAT_ALL)
        music_player.setPlaybackMode(PlaybackMode::REPEAT_ONE);
    else
        music_player.setPlaybackMode(PlaybackMode::NO_REPEAT);
}

void App::selectPlaylist(const std::string& playlist_name)
{
    config_manager.set("focused_playlist", playlist_name);
}

void App::activatePlaylist(const std::string& playlist_name)
{
    if (config_manager.get("active_playlist") != playlist_name)
    {
        music_player.setPlaylist(music_library.getPlaylistPointer(playlist_name));
        config_manager.set("active_playlist", playlist_name);
    }
}

void App::playSong(const std::string& path)
{
    const Song* song = music_library.getSong(path);

    if (!song)
        return;

    music_player.setSound(song->getPath());
    config_manager.set("last_played", song->getPath());
    music_player.play();
}

void App::seek(int seconds)
{
    music_player.seekBy(seconds);
}

void App::load_last_data()
{
    std::string song_path = config_manager.get("last_played");
    music_player.setSound(song_path);

    std::string playlist_name = config_manager.get("active_playlist");
    music_player.setPlaylist(music_library.getPlaylistPointer(playlist_name));

    
    std::string mode = config_manager.get("playback_mode");

    if (mode == "NO_REPEAT")
        music_player.setPlaybackMode(PlaybackMode::NO_REPEAT);
    else if (mode == "REPEAT_ONE")
        music_player.setPlaybackMode(PlaybackMode::REPEAT_ONE);
    if (mode == "REPEAT_ALL")
        music_player.setPlaybackMode(PlaybackMode::REPEAT_ALL);

    std::string shuffle = config_manager.get("shuffle");
    music_player.setShuffle(shuffle == "1");
}

void App::init_player_view()
{
    int start_x, start_y, height, width;
    height = 4;
    width = SCREEN_WIDTH;
    start_y = SCREEN_HEIGHT - height;
    start_x = 0;

    player_view = std::make_unique<PlayerView>(
        start_y, start_x, height, width,
        music_library,
        music_player,
        *this
    );
}

void App::init_library_panel()
{
    int start_x, start_y, height, width;
    height = SCREEN_HEIGHT - player_view->getHeight();
    width = 28;
    start_y = 0;
    start_x = 0;

    library_panel = std::make_unique<LibraryPanel>(
        start_y, start_x, height, width,
        music_library,
        music_player,
        *this
    );
}

void App::init_playlist_view()
{
    int start_x, start_y, height, width;
    height = SCREEN_HEIGHT - player_view->getHeight();
    width = SCREEN_WIDTH - library_panel->getWidth();
    start_y = 0;
    start_x = library_panel->getWidth();

    playlist_view = std::make_unique<PlaylistView>(
        start_y, start_x, height, width,
        music_library,
        music_player,
        *this
    );
}

