
#include "main.h"

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

static void fixture_3x6(fixture_s<double, 3, 6> *fix)
{
    fix->nbcol = 3;
    fix->nbrow = 6;
    fix->values = {1.0, 2.0, 3.0,
                   1.0, 2.0, 3.0,
                   1.0, 2.0, 3.0,
                   1.0, 2.0, 3.0,
                   1.0, 2.0, 3.0,
                   1.0, 2.0, 3.0};
}

static void fixture_2x12(fixture_s<double, 2, 12> *fix)
{
    fix->nbcol = 2;
    fix->nbrow = 12;
    fix->values = {33, 80,
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
}

/**
 * @brief https://www.itl.nist.gov/div898/handbook/pmc/section5/pmc552.htm
 * 
 */
template <typename T, unsigned int NC, unsigned int NR>
static void pca(fixture_s<T, NC, NR> fi)
{
    alglib::real_2d_array ptInput, mcov;
    try
    {
        ptInput.setcontent(fi.nbrow, fi.nbcol, (T *)&fi.values);
        dispm("check", ptInput, fi.nbrow, fi.nbcol);
        covm(ptInput, fi.nbrow, fi.nbcol, mcov);
        dispm(COV_MAT_TITLE, mcov, fi.nbrow, fi.nbcol);
        alglib::ae_int_t info;
        alglib::real_1d_array eigValues;
        alglib::real_2d_array eigVectors;
        pcabuildbasis(ptInput, fi.nbrow, fi.nbcol, info, eigValues, eigVectors);
        dispm(COV_EIGEN_TITLE, eigVectors, fi.nbrow, fi.nbcol);
    }
    catch (alglib::ap_error e)
    {
        printf("error msg: %s\n", e.msg.c_str());
    }
}

int main(int argc, char **argv)
{
    fixture_s<double, 2, 12> fix2x12;
    fixture_2x12(&fix2x12);
    pca(fix2x12);
    return 0;
}
