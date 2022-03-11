
#include <main.h>
#include <optionsparser.h>
#include <speciesdemo.h>

int main(int argc, char **argv)
{
    Data::File::metas_t metas;
    metas.filename = FIXT_CSV_FILE_SPECIES;
    metas.skip = 1;
    metas.delimiter = COMA;
    cmd_options_t cmdopts;
    ui_t rcopt;
    try
    {
        auto *op = new OptionsParser(argc, argv);
        rcopt = op->parse(cmdopts);
        delete op;
    }
    catch (const bpo::error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    if (rcopt == EXIT_FAILURE)
        return 1;
    if (argc >= 2)
    {
        const std::string &filename(*(argv + 1));
        metas.filename = filename;
    }
    try
    {
        auto *demo = new SpeciesDemo<double>(metas, cmdopts);
        demo->run();
        delete demo;
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Error " << e.what() << std::endl;
    }
    return 0;
}
