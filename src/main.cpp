#include "dir_configuration.h"
#include "monitor_core.h"
#include "monitor_gui.h"
#include "directory_information.h"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    dir_configuration cfg;

    string config_path;
    if (argc == 1) {
        config_path = "dir_monitor.conf";
    } else if (string(argv[1]) == "--help") {
        cout << "Usage:" << endl
             << "  dir_monitor                -> Uses default configuration file dir_monitor.conf" << endl
             << "  dir_monitor [config file]  -> Uses specified configuration file" << endl;
        return 0;
    } else {
        config_path = argv[1];
    }

    try {
        cfg.load(config_path);
    } catch (runtime_error& re) {
        cerr << re.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unexpected exception when loading the configuration file!" << endl;
        return 1;
    }

    if (cfg.values().empty()) {
        cerr << "The configuration file does not contain any value [" << config_path << "]" << endl;
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
    while(!gui.quit_signal_received()) {
        vector<directory_information> info = core.updated_status();
        gui.display(info);
    }

    return 0;
}
