#pragma once

#include "../external/miniaudio.h"
#include "model/Playlist.h"
#include <string>
#include <vector>

enum class PlaybackMode
{
    NO_REPEAT,
    REPEAT_ONE,
    REPEAT_ALL,
};

class Player
{
public:
    Player();
    ~Player();

    bool isSoundInitialized() { return sound_initialized; }
    bool isPlaying();
    float getTime();
    std::string getSoundPath() { return sound_path; }
    std::string getPlaylistName() { return active_playlist->getPlaylistName(); }
    bool isFinished() { return ma_sound_at_end(&sound); }
    bool isAtEndOfQueue() const; // ignores playback mode
    bool isAtStartOfQueue() const; // ignores playback mode
    bool isShuffle() const { return shuffle; }
    PlaybackMode getPlaybackMode() const { return playback_mode; }

    void setSound(const std::string& path);
    void setPlaylist(const Playlist* playlist);
    void setShuffle(bool val);
    void setPlaybackMode(PlaybackMode mode) { playback_mode = mode; }

    void play();
    void pause();
    void seekBy(int seconds);
    void buildQueue();
    void buildQueueFrom(int index);
    void nextSong();
    void previousSong();
    void advanceTrack();

private:
    ma_engine engine;
    ma_sound sound;
    std::string sound_path;
    bool sound_initialized; 

    const Playlist* active_playlist; 
    bool shuffle;
    PlaybackMode playback_mode;
    std::vector<int> play_queue;
    int queue_index;

    int getCurrentSongIndex();
};