
#include "main.h"

static void
dispm(
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

fixure_d_t fixture_3x6(void)
{
    double values[18] = {1.0, 2.0, 3.0,
                         1.0, 2.0, 3.0,
                         1.0, 2.0, 3.0,
                         1.0, 2.0, 3.0,
                         1.0, 2.0, 3.0,
                         1.0, 2.0, 3.0};
    fixure_d_t fix = {3, 6, values};
    return fix;
}

fixure_d_t fixture_2x12(void)
{
    double values[24] = {33, 80,
                         33, 82.5,
                         34, 100.8,
                         42, 90,
                         29, 67,
                         19, 60,
                         50, 77,
                         55, 77,
                         31, 87,
                         46, 70,
                         36, 57,
                         48, 64};
    fixure_d_t fix = {2, 12, values};
    return fix;
}

/**
 * @brief https://www.itl.nist.gov/div898/handbook/pmc/section5/pmc552.htm
 * 
 */
static void pca()
{
    unsigned int i, j;
    alglib::real_2d_array ptInput, mcov;
    fixure_d_t fix2x12 = fixture_2x12();
    ptInput.setcontent(fix2x12.nbrow, fix2x12.nbcol, fix2x12.values);
    covm(ptInput, fix2x12.nbrow, fix2x12.nbcol, mcov);
    dispm(COV_MAT_TITLE, mcov, fix2x12.nbrow, fix2x12.nbcol);
    alglib::ae_int_t info;
    alglib::real_1d_array eigValues;
    alglib::real_2d_array eigVectors;
    pcabuildbasis(ptInput, fix2x12.nbrow, fix2x12.nbcol, info, eigValues, eigVectors);
    dispm(COV_EIGEN_TITLE, eigVectors, fix2x12.nbrow, fix2x12.nbcol);
}

int main(int argc, char **argv)
{
    pca();
    return 0;
}
