#include "monitor_core.h"

#include <boost/date_time.hpp>
#include <boost/filesystem.hpp>

#include <algorithm>

using namespace std;
using namespace boost::filesystem;

monitor_core::monitor_core(vector<pair<string, string> > cfg)
    : _configured_dirs(cfg)
{
    //ctor
}

void monitor_core::validate() throw(runtime_error) {
    for (vector<pair<string, string> >::iterator iter = _configured_dirs.begin(); iter != _configured_dirs.end(); iter++) {
        string desc = iter->first;
        string path = iter->second;

        if (!exists(path)) {
            string errmsg = string("Path configured for ") + desc + " (" + path + ") does not exist";
            throw runtime_error(errmsg);
        }

        if (!is_directory(path)) {
            string errmsg = string("Path configured for ") + desc + " (" + path + ") is not a directory";
            throw runtime_error(errmsg);
        }
    }
}

vector<directory_information> monitor_core::updated_status()
{
    vector<directory_information> result;

    for (vector<pair<string, string> >::iterator iter = _configured_dirs.begin(); iter != _configured_dirs.end(); iter++) {
        string desc = iter->first;
        string dir_path = iter->second;

        vector<directory_entry> dir_entries;
        try {
            path p(dir_path);
            copy(directory_iterator(p), directory_iterator(), back_inserter(dir_entries));
        } catch (const filesystem_error& fe) {
            throw runtime_error(fe.what());
        }

        result.push_back(create_dir_information(desc, dir_path, dir_entries));
    }

    return result;
}

directory_information monitor_core::create_dir_information(const string& desc, const string& dirpath, const vector<directory_entry>& dir_entries)
{
    directory_information dirinfo(desc, dirpath);

    for (vector<directory_entry>::const_iterator fileiter = dir_entries.begin(); fileiter != dir_entries.end(); fileiter++) {
        path filepath = fileiter->path();

        if (is_regular_file(filepath)) {
            string name = filepath.filename().string();

            time_t write_time_t = last_write_time(filepath);
            boost::posix_time::ptime write_time = boost::posix_time::from_time_t(write_time_t);
            string write_time_fmt = to_iso_extended_string(write_time);
            write_time_fmt.replace(write_time_fmt.find('T'), 1, " ");

            uintmax_t bytes = file_size(filepath);
            string bytes_fmt = "sz";
            if (bytes > 1024*1024*1024) {
                bytes_fmt = boost::lexical_cast<string>(bytes / (1024*1024*1024)) + " GB";
            } else if (bytes > 1024*1024) {
               bytes_fmt = boost::lexical_cast<string>(bytes / (1024*1024)) + " MB";
            } else if (bytes > 1024) {
                bytes_fmt = boost::lexical_cast<string>(bytes / 1024) + " KB";
            } else {
                bytes_fmt = boost::lexical_cast<string>(bytes) + " B";
            }

            dirinfo.add_file(name, write_time_fmt, bytes_fmt);
        }
    }

    return dirinfo;
}


monitor_core::~monitor_core()
{
    //dtor
}

