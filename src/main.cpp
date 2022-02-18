
#include <main.h>
#include <speciesdemo.h>

int main(int argc, char **argv)
{
    Data::File::metas_t metas;
    metas.filename = FIXT_CSV_FILE_SPECIES;
    metas.skip = 1;
    metas.sep = COMA;
    
    if (argc == 2)
    {
        const std::string &filename(*(argv + 1));
        metas.filename = filename;
    }
    try
    {
        auto *demo = new SpeciesDemo<double>(metas);
        demo->run();
        delete (demo);
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "Error " << e.what() << std::endl;
    }
    return 0;
}
