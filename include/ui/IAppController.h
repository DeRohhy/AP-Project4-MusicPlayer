#pragma once

#include <string>

class IAppController
{
public:
    virtual ~IAppController() = default;


    virtual void togglePlay() = 0;
    virtual void toggleShuffle() = 0;
    virtual void nextTrack() = 0;
    virtual void previousTrack() = 0;
    virtual void changePlaybackMode() = 0;
    virtual void advanceTrack() = 0;
    // virtual void playSong(const std::string& path) = 0;
    // virtual void selectPlaylist(const std::string& playlist_name) = 0;
    virtual void seek(int seconds) = 0;
};