#define MINIAUDIO_IMPLEMENTATION

#include "player/Player.h"
#include <algorithm>
#include <random>

Player::Player() : sound_initialized { false }, active_playlist { nullptr },
                   shuffle { false }, playback_mode { PlaybackMode::NO_REPEAT }
{
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS)
        throw std::runtime_error("Failed to initialize audio engine");
}

Player::~Player()
{
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
}

void Player::setSound(const std::string& path)
{
    if (sound_initialized)
    {
        sound_initialized = false;
        ma_sound_uninit(&sound);
    }
    if (ma_sound_init_from_file(&engine, path.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, &sound) != MA_SUCCESS)
    {
        sound_initialized = false;
        return;
    }

    sound_initialized = true;
    sound_path = path;

    queue_index = getCurrentSongIndex();
}

void Player::setPlaylist(const Playlist* playlist)
{
    active_playlist = playlist;

    if (!sound_initialized || !active_playlist)
        return;

    if (shuffle)
    {
        buildQueueFrom(getCurrentSongIndex());
        queue_index = 0;
    }
    else
    {
        buildQueue();
        queue_index = getCurrentSongIndex();        
    }
}

void Player::setShuffle(bool val)
{
    shuffle = val;

    if (val)
    {
        buildQueueFrom(getCurrentSongIndex());
        queue_index = 0;
    }
    else
    {
        buildQueue();
        queue_index = getCurrentSongIndex();
    }
}

bool Player::isPlaying()
{
    return sound_initialized && ma_sound_is_playing(&sound);
}

float Player::getTime()
{
    if (!sound_initialized) return 0.0f;

    ma_uint64 frames = 0;
    ma_sound_get_cursor_in_pcm_frames(const_cast<ma_sound*>(&sound), &frames);

    return (float)frames / (float)ma_engine_get_sample_rate(
        const_cast<ma_engine*>(&engine)
    );
}

bool Player::isAtEndOfQueue() const
{
    if (!sound_initialized || !active_playlist)
        return false;
    return queue_index + 1 == active_playlist->getSongAmount();
}

bool Player::isAtStartOfQueue() const
{
    if (!sound_initialized || !active_playlist)
        return false;
    return queue_index == 0;
}


void Player::play()
{
    if (sound_initialized && !isPlaying())
        ma_sound_start(&sound);
}

void Player::pause()
{
    if (sound_initialized && isPlaying())
        ma_sound_stop(&sound);
}

void Player::seekBy(int seconds)
{
    if (!sound_initialized) return;
    
    ma_uint64 cursor, length;
    ma_sound_get_cursor_in_pcm_frames(&sound, &cursor);
    ma_sound_get_length_in_pcm_frames(&sound, &length);

    ma_uint32 rate = ma_engine_get_sample_rate(&engine);
    ma_int64 new_frame = (ma_int64)cursor + (ma_int64)seconds * rate;

    if (new_frame < 0) 
        new_frame = 0;
    if ((ma_uint64)new_frame >= length) 
        new_frame = (ma_int64)(length - 1);

    ma_sound_seek_to_pcm_frame(&sound, (ma_uint64)new_frame);
}

void Player::buildQueue()
{
    if (!active_playlist)
    {
        play_queue.clear();
        return;
    }

    size_t song_count = active_playlist->getSongAmount(); 

    if (song_count == 0)
    {
        play_queue.clear();
        return;
    }

    play_queue.resize(song_count);
    for (size_t i = 0; i < song_count; i++)
        play_queue[i] = i;

    if (!shuffle)
        return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(play_queue.begin(), play_queue.end(), gen);
}

void Player::buildQueueFrom(int index) // place the given song index at the start of our play queue
{
    if (!active_playlist)
    {
        play_queue.clear();
        return;
    }

    size_t song_count = active_playlist->getSongAmount(); 

    if (song_count == 0)
    {
        play_queue.clear();
        return;
    }

    play_queue.resize(song_count);
    for (size_t i = 0; i < song_count; i++)
        play_queue[i] = i;

    if (song_count == 1)
        return;

    if (index >= 0 && index < song_count) 
        std::swap(play_queue[index], play_queue[0]);

    if (!shuffle)
        return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(play_queue.begin() + 1, play_queue.end(), gen);
}

int Player::getCurrentSongIndex()
{
    if (!sound_initialized || !active_playlist)
        return -1;

    for (size_t i = 0; i < active_playlist->getSongAmount(); i++)
    {
        if (sound_path == active_playlist->getSong(play_queue[i])->getPath())
            return i;
    }

    return -1;
}

void Player::nextSong()
{
    if (!active_playlist)
        return;
    
    size_t song_count = active_playlist->getSongAmount();
    if (!song_count)
        return;
    
    queue_index = (queue_index + 1 >= song_count) ? 0 : queue_index + 1;

    setSound(active_playlist->getSong(play_queue[queue_index])->getPath());
    play();
}

void Player::previousSong()
{
    if (!active_playlist)
        return;
    
    size_t song_count = active_playlist->getSongAmount();
    if (!song_count)
        return;
    
    queue_index = (queue_index > 0) ? queue_index - 1 : song_count - 1;

    setSound(active_playlist->getSong(play_queue[queue_index])->getPath());
    play();
}

void Player::advanceTrack()
{
    switch (playback_mode)
    {
    case PlaybackMode::NO_REPEAT:
        if (!isAtEndOfQueue())
            nextSong();
        break;
    case PlaybackMode::REPEAT_ALL:
        nextSong();
        break;
    case PlaybackMode::REPEAT_ONE:
        play();
        break;
    }
}