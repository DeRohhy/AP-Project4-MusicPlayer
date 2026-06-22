#pragma once

#include "ui/UIComponent.h"
#include "ui/IAppController.h"

#include "model/Song.h"
#include "player/Player.h"
#include "model/MusicLibrary.h"

class PlayerView : public UIComponent
{
public:
    PlayerView(int y, int x, int h, int w,
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
    static constexpr size_t MAX_TITLE_LEN = 32;
    static constexpr size_t MAX_ARTIST_LEN = 16;
    static constexpr size_t MAX_ALBUM_LEN = 16;
    static constexpr int DURATION_BAR_LEN = 34;
    static constexpr int SIDE_MARGIN = 2;
    static constexpr int SEEK_AMOUNT = 10; // in seconds

    MusicLibrary& music_library;
    Player& music_player;
    IAppController& controller;

    const Song* song;
    
    void drawTitle(int start_y);
    void drawArtistAndAlbum(int start_y);
    void drawDuration(int start_y, int start_x);
    void drawControls(int start_y, int start_x);
};