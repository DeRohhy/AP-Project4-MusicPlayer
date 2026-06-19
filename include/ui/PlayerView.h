#pragma once

#include "ui/UIComponent.h"

#include "model/Song.h"
#include "player/Player.h"
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"

class PlayerView : public UIComponent
{
public:
    PlayerView(int y, int x, int h, int w,
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
    static constexpr size_t MAX_TITLE_LEN = 32;
    static constexpr size_t MAX_ARTIST_LEN = 16;
    static constexpr size_t MAX_ALBUM_LEN = 16;
    static constexpr int DURATION_BAR_LEN = 25;

    ConfigManager& config_manager;
    Player& music_player;
    MusicLibrary& music_library;

    Song* song;
    
    void drawTitle();
    void drawArtistAndAlbum();
    void drawDuration();
    void drawControls();

    void handleShuffle();
    void handlePreviousTrack();
    void handleNextTrack();
    void handlePlay();
    void handlePlaybackMode();
};