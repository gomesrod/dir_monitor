#include "directory_information.h"

directory_information::directory_information(std::string description, std::string path)
    : _description(description), _path(path)
{
    //ctor
}

directory_information::~directory_information()
{
    //dtor
}

void directory_information::add_file(std::string name, std::string date, std::string size) {
    file_info f;
    f.name = name;
    f.date = date;
    f.size = size;
    _files.push_back(f);
}
