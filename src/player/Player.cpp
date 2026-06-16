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
    if (ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, &sound) != MA_SUCCESS)
        throw std::runtime_error("Failed to set sound");
}

void Player::play()
{
    ma_sound_start(&sound);
}

void Player::pause()
{
    ma_sound_stop(&sound);
}