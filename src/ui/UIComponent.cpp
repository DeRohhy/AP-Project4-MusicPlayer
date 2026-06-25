#include "ui/UIComponent.h"
#include <sstream>
#include <iomanip>


std::string UIComponent::fit(const std::string& s, size_t max_width)
{
    if (s.size() <= max_width)
        return s;
    if (max_width <= 3)
        return s.substr(0, max_width);
    return s.substr(0, max_width - 3) + "...";
}

std::string UIComponent::timeToStr(int time)
{
    int mins = time / 60;
    int secs = time % 60;
    std::ostringstream ss;

    ss << std::setw(2) << std::setfill('0') << mins << ":"
       << std::setw(2) << std::setfill('0') << secs;

    return ss.str();
}


void UIComponent::drawOutline()
{
    if (is_focused) // draw green outline if this window is focused
    {
        wattron(window, COLOR_PAIR(2));   
        box(window, 0, 0);
        wattroff(window, COLOR_PAIR(2));   
    }
    else
    {
        wattron(window, A_DIM);
        box(window, 0, 0);
        wattroff(window, A_DIM);
    }
}

void UIComponent::addPadding(int amount)
{
    // waddnstr(window, " ", amount);
    for (int i = 0; i < amount; i++)
        waddch(window, ' ');
}

void UIComponent::drawDivider(int y)
{
    int padding = 2;
    int x = padding;

    wattron(window, A_DIM);
    while (x < width - padding)
        mvwprintw(window, y, x++, "─");
    wattroff(window, A_DIM);
}

void UIComponent::drawScrollBar(int pos_y, int pos_x,
                                int height,
                                int visible_items, int total_items,
                                int scroll_pos)
{
    int thumb_size = (double)height * ((double)visible_items / (double)total_items);
    int thumb_y = (double)(height - thumb_size) * (double)scroll_pos / (double)(total_items - visible_items);
    int thumb_x = pos_x;

    int cur_y = pos_y;

    wattron(window, A_DIM);
    for (int i = 0; i < thumb_y; i++)
        mvwprintw(window, cur_y++, thumb_x, "┃");
        
    for (int i = 0; i < thumb_size; i++)
        mvwprintw(window, cur_y++, thumb_x, "█");

    for (int i = 0; i + thumb_y + thumb_size < height; i++)
        mvwprintw(window, cur_y++, thumb_x, "┃");
    wattroff(window, A_DIM);
}

void UIComponent::revertWindowSize()
{
    wresize(window, height, width);
    mvwin(window, start_y, start_x);
}