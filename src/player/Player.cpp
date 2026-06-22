#define MINIAUDIO_IMPLEMENTATION

#include "player/Player.h"
#include <iostream>

Player::Player()
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
    ma_sound_uninit(&sound);
    if (ma_sound_init_from_file(&engine, path.c_str(), MA_SOUND_FLAG_STREAM, NULL, NULL, &sound) != MA_SUCCESS)
        return;
    sound_path = path;
}

bool Player::isPlaying()
{
    return ma_sound_is_playing(&sound);
}

float Player::getTime()
{
    ma_uint64 frames = 0;
    ma_sound_get_cursor_in_pcm_frames(const_cast<ma_sound*>(&sound), &frames);

    return (float)frames / (float)ma_engine_get_sample_rate(
        const_cast<ma_engine*>(&engine)
    );
}

void Player::play()
{
    if (!isPlaying())
        ma_sound_start(&sound);
}

void Player::pause()
{
    if (isPlaying())
        ma_sound_stop(&sound);
}