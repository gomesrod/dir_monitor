#include "dir_configuration.h"

#include <fstream>
#include <boost/program_options/parsers.hpp>

using namespace std;

dir_configuration::dir_configuration()
{
}

void dir_configuration::load(string path) throw(std::runtime_error)
{
    using namespace boost::program_options;

    ifstream config_file(path.c_str());

    if (config_file.fail()) {
       string errmsg = string("Could not read configuration file ") + path;
       throw runtime_error(errmsg);
    }

    options_description opts_desc;

    try {
        parsed_options parsed_opts = parse_config_file(config_file, opts_desc, true);

        for (vector<basic_option<char> >::iterator iter = parsed_opts.options.begin(); iter != parsed_opts.options.end(); iter++) {
            _values.push_back(make_pair(iter->string_key, iter->value[0]));
        }
    } catch (boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::program_options::invalid_config_file_syntax> >& ex) {
        throw runtime_error(ex.what());
    }
}

dir_configuration::~dir_configuration()
{
    //dtor
}
