#ifndef MONITOR_GUI_H
#define MONITOR_GUI_H

#include "directory_information.h"
#include <vector>

class monitor_gui
{
    public:
        monitor_gui();
        virtual ~monitor_gui();

        void display(std::vector<directory_information>);
        bool quit_signal_received() {return _quit_signal;};

    private:
        struct ReverseDateTimeOrderComparator {
            bool operator()(const directory_information::file_info& f1, const directory_information::file_info& f2);
        };
        bool _quit_signal;
};

#endif // MONITOR_GUI_H
