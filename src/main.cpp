
#include <main.h>
#include <speciesdemo.h>

static bool findDelimiter(std::string filename, std::string &delimiter)
{
    std::ifstream file(filename.c_str());
    const bool &fileExist = file.is_open();
    if (fileExist)
    {
        std::string l;
        std::istream &in = std::getline(file, l);
        if (in)
        {
            std::cout << l << std::endl;
            if (std::find(l.begin(), l.end(), ',') != l.end())
                delimiter = COMA;
            else if (std::find(l.begin(), l.end(), ';') != l.end())
                delimiter = SEMICOLON;
            else if (std::find(l.begin(), l.end(), ' ') != l.end())
                delimiter = SP;
        }
        file.close();
    }
    return fileExist;
}

int main(int argc, char **argv)
{
    Data::File::metas_t metas;
    metas.skip = 1;
    metas.sep = COMA;
    metas.filename = FIXT_CSV_FILE_SPECIES;
    if (argc == 2)
    {
        const std::string &filename(*(argv + 1));
        metas.filename = filename;
        if (false == findDelimiter(metas.filename, metas.sep))
        {
            return 1;
        };
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
