#define MINIAUDIO_IMPLEMENTATION

#include "player/Player.h"
#include <iostream>

Player::Player() : sound_initialized { false }
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