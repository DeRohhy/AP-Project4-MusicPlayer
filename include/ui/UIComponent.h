#pragma once

#include <ncurses.h>
#include <string>


class UIComponent
{
public:
    UIComponent(int y, int x, int h, int w) 
        : start_y(y), start_x(x), height(h), width(w) 
        {
            window = newwin(height, width, start_y, start_x);
            refresh();
        }
    
    virtual ~UIComponent() { delwin(window); }

    void setFocus(bool val) { is_focused = val; };

    virtual void draw() = 0;
    virtual void handleInput(int op) = 0;

    // Truncate to `width` chars, ending with "..." if it was too long
    std::string fit(const std::string& s, size_t max_width);
    std::string timeToStr(int time);

    void drawOutline();
    void addPadding(int amount);
    void drawDivider(int y);
    void drawScrollBar(int pos_y, int pos_x,
                       int height,
                       int visible_items, int total_items,
                       int scroll_pos);

protected:
    WINDOW* window;
    int start_y;
    int start_x;
    int height;
    int width;

    bool is_focused;
};