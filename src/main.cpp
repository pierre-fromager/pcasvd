
#include <main.h>
#include <speciesdemo.h>

int main(int argc, char **argv)
{
    auto *demo = new SpeciesDemo<double, 4, 150>();
    demo->run();
    delete (demo);
    return 0;
}
