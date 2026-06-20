#pragma once

#include <memory>
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"
#include "player/Player.h"
#include "ui/PlayerView.h"
#include "ui/LibraryPanel.h"
#include "ui/PlaylistView.h"

class UI
{
public:
    UI();
    ~UI();

    void run();

    void draw();
private:
    static constexpr int SCREEN_HEIGHT = 10;
    static constexpr int SCREEN_WIDTH = 78;

    ConfigManager config_manager;
    MusicLibrary music_library;
    Player music_player;
    std::unique_ptr<PlayerView> player_view;    
    std::unique_ptr<LibraryPanel> library_panel;
    std::unique_ptr<PlaylistView> playlist_view;

    void init_player_view();
    void init_library_panel();
    void init_playlist_view();
};