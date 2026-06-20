#pragma once

#include "ui/UIComponent.h"

#include "player/Player.h"
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"

class PlaylistView : public UIComponent
{
public:
    PlaylistView(int y, int x, int h, int w,
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
    ConfigManager& config_manager;
    Player& music_player;
    MusicLibrary& music_library;


};