#include "ui/PlaylistView.h"

void PlaylistView::draw()
{
    refresh();
    // Clear the window before redrawing to prevent ghosting
    werase(window);

    drawOutline();

    wrefresh(window);
}

void PlaylistView::handleInput(int op)
{
    if (!is_focused)
        return;
}