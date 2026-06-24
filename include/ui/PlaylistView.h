#pragma once

#include "ui/UIComponent.h"
#include "ui/IAppController.h"

#include "model/Playlist.h"

#include "player/Player.h"
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"

#include <vector>

enum class SortOption
{
    NONE,
    TITLE,
    ARTIST,
    ALBUM,
    YEAR,
    DUR,
};

class PlaylistView : public UIComponent
{
public:
    PlaylistView(int y, int x, int h, int w,
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

    void loadPlaylist(const std::string& playlist_name);

private:
    static constexpr int MAX_VISIBLE_SONGS = 6;
    static constexpr int SIDE_MARGIN = 2;

    MusicLibrary& music_library;
    Player& music_player;
    IAppController& controller;

    Playlist active_playlist;

    struct Column { std::string title; int width; };
    const std::vector<Column> columns
    {
        {"#", 4},
        {"Title", 24},
        {"Album", 16},
        {"⏱", 8},
    };

    struct SortingField { std::string title; SortOption option; char key; };
    const std::vector<SortingField> sorting_options {
        {"Title", SortOption::TITLE, '1'},
        {"Artist", SortOption::ARTIST, '2'},
        {"Album", SortOption::ALBUM, '3'},
        {"Year", SortOption::YEAR, '4'},
        {"Dur", SortOption::DUR, '5'},
    };
    bool is_sort_ascending = true;
    SortOption selected_sort = SortOption::NONE;
    bool revert_sort = false;

    int songs_starting_index = 0;
    int selected_song = 0;

    void drawHeader(int start_y);
    void showInfoHelper(int start_y) const;
    void showSong(int y, int index, const Song* song);
    void showSongs(int start_y);
    void showControls(int start_y);
    void showSortingControls(int start_y);

    void handleKeyUp();
    void handleKeyDown();
    void handleSortKey(int key);
};