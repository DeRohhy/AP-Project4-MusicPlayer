#pragma once

#include <ncurses.h>
#include <string>
#include <io/ConfigManager.h>

class UIComponent
{
public:
    UIComponent(int y, int x, int h, int w, ConfigManager& _config_manager)
        : start_y(y), start_x(x), height(h), width(w), config_manager(_config_manager)
        {
            window = newwin(height, width, start_y, start_x);
            refresh();
        }
    
    virtual ~UIComponent() { delwin(window); }

    virtual void draw() = 0;

    // Truncate to `width` chars, ending with "..." if it was too long
    std::string fit(const std::string& s, size_t max_width);

    std::string timeToStr(int time);

    void addPadding(int amount);

protected:
    WINDOW* window;
    int start_y;
    int start_x;
    int height;
    int width;
    ConfigManager config_manager;
};