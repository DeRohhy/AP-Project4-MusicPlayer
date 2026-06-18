// ========== Naming Convention Guideline ==========
// Classes & Enums & Structs: PascalCase
// Functions: camelCase
// Variables: lower_snake_case
// Constants: UPPER_SNAKE_CASE
// =================================================

#include "ui/UI.h"
#include <clocale> // Needed for setlocale

int main()
{
    std::setlocale(LC_ALL, ""); // fixes unicodes not showing properly

    UI ui;
    ui.run();

    return 0;
}