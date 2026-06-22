#pragma once

#include "ui/UIComponent.h"

#include "model/Playlist.h"

#include "player/Player.h"
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"

#include <vector>

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
    static constexpr int MAX_VISIBLE_SONGS = 5;
    static constexpr int SIDE_MARGIN = 2;

    ConfigManager& config_manager;
    Player& music_player;
    MusicLibrary& music_library;

    Playlist active_playlist;

    struct Column { std::string title; int width; };
    const std::vector<Column> columns
    {
        {"#", 4},
        {"Title", 24},
        {"Album", 16},
        {"⏱", 8},
    };

    int songs_starting_index = 0;
    int selected_song = 0;

    void loadPlaylist(const std::string& playlist_name);

    void drawHeader(int start_y);
    void showInfoHelper(int start_y) const;
    void showSong(int y, int index, const Song* song);
    void showSongs(int start_y);

    void handleKeyUp();
    void handleKeyDown();
    void handlePlaySong();
};