// ========== Naming Convention Guideline ==========
// Classes & Enums & Structs: PascalCase
// Functions: camelCase
// Variables: lower_snake_case
// Constants: UPPER_SNAKE_CASE
// =================================================

#include "player/Player.h"
#include <string>
#include <iostream>
int main()
{
    Player player;
    std::string path = "data/music/Lonely Town, Lonely Street.mp3";
    player.setSound(path);
    player.play();

    char s;
    std::cin >> s;
    return 0;
}