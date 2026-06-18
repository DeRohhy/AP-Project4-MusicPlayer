#pragma once

#include "ui/UIComponent.h"

#include "model/Song.h"

class PlayerView : public UIComponent
{
public:
    PlayerView(int y, int x, int h, int w, ConfigManager _config_manager)
        : UIComponent(y, x, h, w, _config_manager) {}

    void setSong(Song* new_song) { song = new_song; };

    void draw() override;

private:
    static constexpr size_t MAX_TITLE_LEN = 32;
    static constexpr size_t MAX_ARTIST_LEN = 16;
    static constexpr size_t MAX_ALBUM_LEN = 16;
    static constexpr int DURATION_BAR_LEN = 25;

    Song* song = nullptr;

    void drawTitle();
    void drawArtistAndAlbum();
    void drawDuration(int cur_time);
    void drawControls();
};