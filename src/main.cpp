
#include <main.h>
#include <speciesdemo.h>

int main(int argc, char **argv)
{
    auto *irisDemo = new SpeciesDemo<double, 4, 150>();
    irisDemo->run();
    delete (irisDemo);
    return 0;
}
