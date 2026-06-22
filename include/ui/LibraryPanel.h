#pragma once

#include "ui/UIComponent.h"
#include "ui/IAppController.h"

#include "player/Player.h"
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"

#include "model/Playlist.h"

#include <vector>

class LibraryPanel : public UIComponent
{
public:
    LibraryPanel(int y, int x, int h, int w,
               MusicLibrary& _music_library,
               Player& _music_player,
               IAppController& _controller)
        : UIComponent(y, x, h, w),
          music_library(_music_library),
          music_player(_music_player),
          controller(_controller)
    {}

    void draw() override;
    void handleInput(int op) override;
    
private:
    static constexpr int MAX_VISIBLE_PLAYLISTS = 8;
    static constexpr int SIDE_MARGIN = 2;
    
    MusicLibrary& music_library;
    Player& music_player;
    IAppController& controller;
    
    std::vector<Playlist> visible_playlists;
    
    int playlists_start_index = 0;
    int selected_playlist = 0;

    void drawHeader(int start_y);
    void showPlaylists(int start_y);

    void handleKeyUp();
    void handleKeyDown();
    void showControls();
};