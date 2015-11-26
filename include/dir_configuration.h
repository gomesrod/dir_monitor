#ifndef DIR_CONFIGURATION_H
#define DIR_CONFIGURATION_H

#include <string>
#include <stdexcept>
#include <vector>

class dir_configuration
{
    public:
        dir_configuration();
        ~dir_configuration();

        void load(std::string path) throw(std::runtime_error);
        std::vector<std::pair<std::string, std::string> > values() {return _values;};

    //private:
        std::vector<std::pair<std::string, std::string> > _values;
};

#endif // DIR_CONFIGURATION_H
