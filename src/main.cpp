
#include "main.h"

static void dispm(
    std::string title,
    alglib::real_2d_array v,
    unsigned int nbrow,
    unsigned int nbcol)
{
    unsigned int i, j;
    const Colors::Define c_title(Colors::Id::FG_CYAN);
    const Colors::Define c_value(Colors::Id::FG_WHITE);
    const Colors::Define c_reset(Colors::Id::RESET);
    std::cout << c_title << title << c_reset << std::endl;
    std::cout << c_value;
    for (j = 0; j < nbrow; j++)
    {
        for (i = 0; i < nbcol; i++)
            std::cout << v[j][i] << TAB;
        std::cout << std::endl;
    }
    std::cout << c_reset;
}

static void fix_3x6(fixt_s<double, 3, 6> *fix)
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

static void fix_2x12(fixt_s<double, 2, 12> *fix)
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
static void pca(fixt_s<T, NC, NR> fix)
{
    try
    {
        alglib::real_2d_array ptInput, mcov;
        ptInput.setcontent(fix.nbrow, fix.nbcol, (T *)&fix.values);
        dispm(FIXTURE_TITLE, ptInput, fix.nbrow, fix.nbcol);
        alglib::covm(ptInput, fix.nbrow, fix.nbcol, mcov);
        dispm(COV_MAT_TITLE, mcov, fix.nbcol, fix.nbcol);
        alglib::ae_int_t info;
        alglib::real_1d_array eigValues;
        alglib::real_2d_array eigVectors;
        alglib::pcabuildbasis(ptInput, fix.nbrow, fix.nbcol, info, eigValues, eigVectors);
        dispm(COV_EIGEN_TITLE, eigVectors, fix.nbcol, fix.nbcol);
    }
    catch (alglib::ap_error e)
    {
        printf(ALGLIB_ERR_MSG, e.msg.c_str());
    }
}

int main(int argc, char **argv)
{
    fixt_s<double, 2, 12> fix2x12;
    fix_2x12(&fix2x12);
    pca(fix2x12);
    return 0;
}
