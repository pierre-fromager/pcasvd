
#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/version.hpp>
#include <options.h>

#define CMD_OPT_USAGE "Usage pca [filename] [options]"
#define CMD_OPT_HLP "help"
#define CMD_OPT_HLP_OPTS "help options"
#define CMD_OPT_D1 "d1"
#define CMD_OPT_D1_HLP "set dim1 col"
#define CMD_OPT_D2 "d2"
#define CMD_OPT_D2_HLP "set dim2 col"

namespace bpo = boost::program_options;

class OptionsParser
{
public:
    explicit OptionsParser(int argc, char **argv);
    ~OptionsParser();
    ui_t parse(cmd_options_t &cmdopts);

private:
    int m_argc;
    char **m_argv;
};

#endif