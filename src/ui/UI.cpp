#include "ui/UI.h"

#include <ncurses.h>
#include <algorithm>
#include <vector>

UI::UI()
{
    initscr();
    cbreak(); // allow break from app with control + c
    noecho(); // doesnt show user input on screen (might get removed)
    keypad(stdscr, TRUE);
    curs_set(false); // make cursor invisible (might get removed)
    timeout(100); // ignores user input every 100 milliseconds

    if (has_colors())
    {
        start_color();
        use_default_colors();
        
        init_pair(1, COLOR_WHITE, -1); // main text        
        init_pair(2, COLOR_GREEN, -1); // accent color
        init_pair(3, COLOR_RED, -1); // error color
    }
}

UI::~UI()
{
    keypad(stdscr, FALSE);
    curs_set(true);
    endwin();
}

void UI::run()
{
    music_player.setSound(music_library.all_songs[4].get()->getPath());
    // music_player.play();

    init_player_view();
    init_library_panel();


    std::vector<UIComponent*> components = {
        player_view.get(),
        library_panel.get(),
    };
    int focused_index = 0;
    components[focused_index]->setFocus(true);

    bool running = true;


    draw();
    while (running)
    {
        draw();

        int op = getch();
        switch (op)
        {
        case 'q':
        case 'Q':
        case '0':
            running = false;
            break;
        case '\t': // switch focus when user hits tab
            components[focused_index]->setFocus(false);
            focused_index = (focused_index + 1) % components.size();
            components[focused_index]->setFocus(true);
            break;
        default:
            components[focused_index]->handleInput(op);
        }

        refresh();
    }

}

void UI::draw()
{
    int cur_screen_height, cur_screen_width;
    getmaxyx(stdscr, cur_screen_height, cur_screen_width);

    clear();

    if (cur_screen_height < SCREEN_HEIGHT || cur_screen_width < SCREEN_WIDTH)
    {
        attron(COLOR_PAIR(3));
        mvprintw(cur_screen_height / 2 - 1, 0, "Terminal too small!");
        mvprintw(cur_screen_height / 2, 0, 
            "Please resize to at least %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
        attroff(COLOR_PAIR(3));
        
        attron(A_DIM);
        mvprintw(cur_screen_height / 2 + 1, 0, "[q]quit");
        attroff(A_DIM);
        refresh();
        return;
    }

    player_view->draw();
    library_panel->draw();
    
}

void UI::init_player_view()
{
    int start_x, start_y, height, width;
    height = 4;
    width = SCREEN_WIDTH;
    start_y = 15;
    start_x = 0;

    player_view = std::make_unique<PlayerView>(start_y, start_x, height, width,
                                               config_manager,
                                               music_player,
                                               music_library
                                              );
}

void UI::init_library_panel()
{
    int start_x, start_y, height, width;
    height = 15;
    width = 26;
    start_y = 0;
    start_x = 0;

    library_panel = std::make_unique<LibraryPanel>(start_y, start_x, height, width,
                                               config_manager,
                                               music_player,
                                               music_library
                                              );
}