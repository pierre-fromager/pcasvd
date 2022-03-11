
#include <optionsparser.h>

namespace bpo = boost::program_options;

OptionsParser::OptionsParser(int argc, char **argv) : m_argc(argc), m_argv(argv)
{
}

OptionsParser::~OptionsParser()
{
}

ui_t OptionsParser::parse(cmd_options_t &cmdopts)
{
    bpo::options_description desc(CMD_OPT_USAGE);
    /*
    desc.add_options()
        (CMD_OPT_HLP, CMD_OPT_HLP_OPTS)
        (CMD_OPT_D1, bpo::value<int>(), CMD_OPT_D1_HLP)
        (CMD_OPT_D2, bpo::value<int>(), CMD_OPT_D2_HLP);*/

    desc.add_options()(CMD_OPT_HLP, CMD_OPT_HLP_OPTS);
    desc.add_options()(CMD_OPT_D1, bpo::value<int>(), CMD_OPT_D1_HLP);
    desc.add_options()(CMD_OPT_D2, bpo::value<int>(), CMD_OPT_D2_HLP);
    bpo::variables_map vm;
    bpo::store(bpo::parse_command_line(m_argc, m_argv, desc), vm);
    bpo::notify(vm);
    if (vm.count(CMD_OPT_HLP))
    {
        std::cout << desc << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count(CMD_OPT_D1))
        cmdopts.d1c = vm[CMD_OPT_D1].as<int>();
    if (vm.count(CMD_OPT_D2))
        cmdopts.d2c = vm[CMD_OPT_D2].as<int>();
    return EXIT_SUCCESS;
}