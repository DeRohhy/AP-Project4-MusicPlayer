#pragma once

#include "../external/miniaudio.h"
#include <string>

class Player
{
public:
    Player();
    ~Player();

    bool isPlaying();
    float getTime();
    std::string getSoundPath() { return sound_path; }

    void setSound(const std::string& path);

    void play();
    void pause();

private:
    ma_engine engine;
    ma_sound sound;
    std::string sound_path;
};