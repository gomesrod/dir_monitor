#ifndef MONITOR_CORE_H
#define MONITOR_CORE_H

#include "directory_information.h"

#include <stdexcept>
#include <vector>
#include <string>

namespace boost{namespace filesystem{class directory_entry;}}

class monitor_core
{
    public:
        monitor_core(std::vector<std::pair<std::string, std::string> >);
        ~monitor_core();

        void validate() throw(std::runtime_error);
        std::vector<directory_information> updated_status();

    private:
        std::vector<std::pair<std::string, std::string> > _configured_dirs;

        directory_information create_dir_information(const std::string& desc, const std::string& dirpath,
                                                     const std::vector<boost::filesystem::directory_entry>& dir_entries);
};

#endif // MONITOR_CORE_H
