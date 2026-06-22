#pragma once

#include "ui/UIComponent.h"

#include "player/Player.h"
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"

#include "model/Playlist.h"

#include <vector>

class LibraryPanel : public UIComponent
{
public:
    LibraryPanel(int y, int x, int h, int w,
                 ConfigManager& _config_manager,
                 Player& _music_player,
                 MusicLibrary& _music_library)
        : UIComponent(y, x, h, w),
            config_manager(_config_manager),
            music_player(_music_player),
            music_library(_music_library)
    {}

    void draw() override;
    void handleInput(int op) override;
    
private:
    static constexpr int MAX_VISIBLE_PLAYLISTS = 8;
    static constexpr int SIDE_MARGIN = 2;
    
    ConfigManager& config_manager;
    Player& music_player;
    MusicLibrary& music_library;
    
    std::vector<Playlist> visible_playlists;
    
    int playlists_start_index = 0;
    int selected_playlist = 0;

    void drawHeader(int start_y);
    void showPlaylists(int start_y);

    void handleKeyUp();
    void handleKeyDown();
    void showControls();
};