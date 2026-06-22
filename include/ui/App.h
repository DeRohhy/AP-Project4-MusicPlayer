#pragma once

#include <memory>
#include "ui/IAppController.h"
#include "io/ConfigManager.h"
#include "model/MusicLibrary.h"
#include "player/Player.h"
#include "ui/PlayerView.h"
#include "ui/LibraryPanel.h"
#include "ui/PlaylistView.h"

class App : public IAppController
{
public:
    App();
    ~App();

    void run();
    void draw();


    void togglePlay() override;
    void toggleShuffle() override;
    void nextTrack() override;
    void previousTrack() override;
    void advanceTrack() override;
    void changePlaybackMode() override;
    void playSong(const std::string& path) override;
    void selectPlaylist(const std::string& playlist_name) override;
    void activatePlaylist(const std::string& playlist_name) override;
    void seek(int seconds) override;

private:
    static constexpr int SCREEN_HEIGHT = 32;
    static constexpr int SCREEN_WIDTH = 90;

    ConfigManager config_manager;
    MusicLibrary music_library;
    Player music_player;
    std::unique_ptr<PlayerView> player_view;    
    std::unique_ptr<LibraryPanel> library_panel;
    std::unique_ptr<PlaylistView> playlist_view;

    void init_player_view();
    void init_library_panel();
    void init_playlist_view();
    void load_last_data();
};