#ifndef DIR_STATUS_H
#define DIR_STATUS_H

#include <string>
#include <vector>

class directory_information
{
    public:
        struct file_info {
            std::string name;
            std::string date;
            std::string size;
        };

        directory_information(std::string description, std::string path);
        ~directory_information();

        void add_file(std::string name, std::string date, std::string size);
        std::string description() {return _description;};
        std::string path() {return _path;};
        std::vector<file_info> files() {return _files;};

    private:
        std::string _description;
        std::string _path;
        std::vector<file_info> _files;
};

#endif // DIR_STATUS_H
