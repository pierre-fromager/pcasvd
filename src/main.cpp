
#include <iostream>
#include <iomanip>
#include "colors.h"
#include "alglib/statistics.h"
#include "alglib/dataanalysis.h"

#define TAB "\t"
#define STAR "*"
#define SPACE " "
#define DASH "-"

static void dispm(
    std::string title,
    alglib::real_2d_array matvect,
    unsigned int nbrow,
    unsigned int nbcol)
{
    unsigned int i, j;
    std::cout << title << std::endl;
    for (j = 0; j < nbcol; j++)
    {
        for (i = 0; i < nbcol; i++)
            std::cout << matvect[i][j] << TAB;
        std::cout << std::endl;
    }
}

/**
 * @brief https://www.itl.nist.gov/div898/handbook/pmc/section5/pmc552.htm
 * 
 */
static void pca()
{
    unsigned int i, j;
    /*
    const unsigned int nbcol = 3;
    const unsigned int nbrow = 6;
    double values[nbcol * nbrow] = {1.0, 2.0, 3.0,
                                    1.0, 2.0, 3.0,
                                    1.0, 2.0, 3.0,
                                    1.0, 2.0, 3.0,
                                    1.0, 2.0, 3.0,
                                    1.0, 2.0, 3.0};
*/
    const unsigned int nbcol = 2;
    const unsigned int nbrow = 12;
    double values[nbcol * nbrow] = {
        33,
        80,
        33,
        82.5,
        34,
        100.8,
        42,
        90,
        29,
        67,
        19,
        60,
        50,
        77,
        55,
        77,
        31,
        87,
        46,
        70,
        36,
        57,
        48,
        64,
    };
    alglib::real_2d_array ptInput, mcov;
    ptInput.setcontent(nbrow, nbcol, values);
    covm(ptInput, nbrow, nbcol, mcov);
    dispm("CovMatrix", mcov, nbrow, nbcol);
    alglib::ae_int_t info;
    alglib::real_1d_array eigValues;
    alglib::real_2d_array eigVectors;
    pcabuildbasis(ptInput, nbrow, nbcol, info, eigValues, eigVectors);
    dispm("EigenBasis", eigVectors, nbrow, nbcol);
}

int main(int argc, char **argv)
{
    pca();
    return 0;
}
