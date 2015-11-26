#include "monitor_gui.h"

#include <iostream>

#include <boost/date_time.hpp>
#include <curses.h>

using namespace std;

monitor_gui::monitor_gui()
{
   initscr();
   timeout(1000);
}

monitor_gui::~monitor_gui()
{
   endwin();
}

bool monitor_gui::display(vector<directory_information> infos) {
    clear();

    // Print directories information
    for(vector<directory_information>::iterator info = infos.begin(); info != infos.end(); info++) {
        printw(">> %-20s %s\n", info->description().c_str(), info->path().c_str());

        vector<directory_information::file_info>files = info->files();

        sort(files.begin(), files.end(), ReverseDateTimeOrderComparator());

        for (vector<directory_information::file_info>::iterator fileiter = files.begin(); fileiter != files.end(); fileiter++) {
            printw("   %-60s %s   %8s\n", fileiter->name.c_str(), fileiter->date.c_str(), fileiter->size.c_str());
        }
    }

    // Print date and time at the bottom
    int ymax, xmax;

    getmaxyx(stdscr, ymax, xmax);

    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    const char* now_str = to_simple_string(now).c_str();
    mvprintw(ymax - 1, xmax - 20,"%s", now_str);

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
