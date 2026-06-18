#pragma once

#include <memory>
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"
#include "player/Player.h"
#include "ui/PlayerView.h"

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

    void init_player_view();
};