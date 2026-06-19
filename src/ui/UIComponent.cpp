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
        box(window, 0, 0);
}

void UIComponent::addPadding(int amount)
{
    // waddnstr(window, " ", amount);
    for (int i = 0; i < amount; i++)
        waddch(window, ' ');
}

