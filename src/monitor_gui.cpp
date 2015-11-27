#include "monitor_gui.h"

#include <iostream>

#include <boost/date_time.hpp>
#include <curses.h>

#define COLORPAIR_PATH 1
#define COLORPAIR_EMPTY 2

using namespace std;

monitor_gui::monitor_gui()
{
   initscr();

   if (has_colors()) {
        start_color();
        init_pair(COLORPAIR_PATH, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLORPAIR_EMPTY, COLOR_RED, COLOR_BLACK);
   }

   timeout(1000);
}

monitor_gui::~monitor_gui()
{
   endwin();
}

bool monitor_gui::display(vector<directory_information> infos) {
    using namespace boost::posix_time;
    static ptime start_time = second_clock::local_time();

    clear();

    int ymax, xmax;
    getmaxyx(stdscr, ymax, xmax);

    // Calculate how many items can be shown per directory, based on screen size
    unsigned int max_files_to_display = ((ymax - 2) / infos.size()) - 1;

    // Print directories information
    for(vector<directory_information>::iterator info = infos.begin(); info != infos.end(); info++) {
        attron(A_BOLD);
        printw("%s   ", info->description().c_str());
        attroff(A_BOLD);

        attron(COLOR_PAIR(COLORPAIR_PATH));
        printw("%s\n", info->path().c_str());
        attroff(COLOR_PAIR(COLORPAIR_PATH));

        vector<directory_information::file_info>files = info->files();

        if (files.empty()) {
            attron(COLOR_PAIR(COLORPAIR_EMPTY));
            printw("   (no files)\n");
            attroff(COLOR_PAIR(COLORPAIR_EMPTY));
        } else {
            sort(files.begin(), files.end(), ReverseDateTimeOrderComparator());

            unsigned int i;
            for (i = 0; i < files.size() && i < max_files_to_display; i++) {
                directory_information::file_info f = files[i];
                printw("   %-70s %s   %8s\n", f.name.c_str(), f.date.c_str(), f.size.c_str());
            }
            if (i == max_files_to_display) {
                // Last written item will be erased, to make room for the message
                int y, x;
                getyx(stdscr, y, x);
                move(y-1, 0);
                printw("   (+ %d files)\n", files.size() - max_files_to_display + 1);
            }
        }
    }

    // Print date and time at the bottom

    ptime now = second_clock::local_time();
    time_duration ellapsed(time_period(start_time, now).length());
    const char* now_str = to_simple_string(ellapsed).c_str();
    mvprintw(ymax - 1, xmax - 9,"%s", now_str);

    mvprintw(ymax -1, 1, "Press 'q' to quit ");
    refresh();
    char quit_char = getch();
    if (quit_char == 'q' || quit_char == 'Q') {
        return false;
    }

    return true;
}

bool monitor_gui::ReverseDateTimeOrderComparator::operator()(const directory_information::file_info& f1, const directory_information::file_info& f2) {
    // Comparing with greater sign because we want reverse order!
    return f1.date > f2.date;
};
