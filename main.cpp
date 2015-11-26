#include "dir_configuration.h"
#include "monitor_core.h"
#include "monitor_gui.h"
#include "directory_information.h"

#include <iostream>

using namespace std;

void _sleep(int millis);

int main()
{
    dir_configuration cfg;

    try {
        cfg.load("dir_monitor.conf");
    } catch (runtime_error& re) {
        cerr << re.what() << endl;
        return 1;
    }

    monitor_core core(cfg.values());
    try {
        core.validate();
    } catch (runtime_error& re) {
        cerr << re.what() << endl;
        return 1;
    }

    monitor_gui gui;
    while(true) {
        vector<directory_information> info = core.updated_status();
        if (!gui.display(info)) {
            break;
        }
    }

    return 0;
}
