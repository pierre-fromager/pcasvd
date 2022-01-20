
#include "main.h"

/**
 * @brief disp matrix(2d) with nbrow and nbcol constraints
 * 
 * @param title 
 * @param v 
 * @param nbrow 
 * @param nbcol 
 */
static void dispm(
    std::string title,
    alglib::real_2d_array v,
    ui_t nbrow,
    ui_t nbcol)
{
    ui_t i, j;
    const Colors::Define c_title(Colors::Id::FG_CYAN);
    const Colors::Define c_value(Colors::Id::FG_WHITE);
    const Colors::Define c_reset(Colors::Id::RESET);
    std::cout << TAB << c_title << title << c_reset << std::endl;
    std::cout << c_value;
    for (j = 0; j < nbrow; j++)
    {
        for (i = 0; i < nbcol; i++)
            std::cout << std::setw(11) << std::left << TAB << v[j][i];
        std::cout << std::endl;
    }
    std::cout << c_reset;
}

/**
 * @brief disp vector(1d) with nbcol constraints
 * 
 * @param title 
 * @param v 
 * @param nbcol 
 */
static void dispv(
    std::string title,
    alglib::real_1d_array v,
    ui_t nbcol)
{
    ui_t i;
    const Colors::Define c_title(Colors::Id::FG_CYAN);
    const Colors::Define c_value(Colors::Id::FG_WHITE);
    const Colors::Define c_reset(Colors::Id::RESET);
    std::cout << TAB << c_title << title << c_reset << std::endl;
    std::cout << c_value;
    for (i = 0; i < nbcol; i++)
        std::cout << std::setw(11) << std::left << TAB << v[i];
    std::cout << c_reset << std::endl;
}

/**
 * @brief set a fixture with 2 colums and 12 rows
 * 
 * @param fix 
 */
static void fix_2x12(fixt_s<double, 2, 12> *fix)
{
    fix->nbcol = 2;
    fix->nbrow = 12;
    fix->values = {33.0, 80.0,
                   33.0, 82.5,
                   34.0, 100.8,
                   42.0, 90.0,
                   29.0, 67.0,
                   19.0, 60.0,
                   50.0, 77.0,
                   55.0, 77.0,
                   31.0, 87.0,
                   46.0, 70.0,
                   36.0, 57.0,
                   48.0, 64.0};
}

/**
 * @brief intend to match pca and covm results with script matlhlab
 * @url https://www.itl.nist.gov/div898/handbook/pmc/section5/pmc552.htm
 * @url https://datatab.net/statistics-calculator/factor-analysis
 * @url http://www.jybaudot.fr/Analdonnees/acpvarres.html
 * @url https://www.bytefish.de/blog/pca_lda_with_gnu_octave.html
 * 
 */
template <typename T, ui_t NC, ui_t NR>
static void pca(fixt_s<T, NC, NR> fix)
{
    try
    {
        const ui_t r = fix.nbrow;
        const ui_t c = fix.nbcol;
        alglib::real_2d_array ptInput, mcov, mcorr;
        ptInput.setcontent(r, c, (T *)&fix.values);
        dispm(FIXTURE_DATA_TITLE, ptInput, r, c);
        alglib::covm(ptInput, r, c, mcov);
        dispm(COV_MAT_TITLE, mcov, c, c);
        alglib::pearsoncorrm(ptInput, r, c, mcorr);
        dispm(COR_MAT_TITLE, mcorr, c, c);
        alglib::ae_int_t info;
        alglib::real_1d_array eigValues;
        alglib::real_2d_array eigVectors;
        alglib::pcabuildbasis(ptInput, r, c, info, eigValues, eigVectors);
        dispm(PCA_EIGEN_VECTORS_TITLE, eigVectors, c, c);
        dispv(PCA_EIGEN_VALUES_TITLE, eigValues, c);
    }
    catch (alglib::ap_error e)
    {
        printf(ALGLIB_ERR_MSG, e.msg.c_str());
    }
}

int main(int argc, char **argv)
{
    const Colors::Define c_title(Colors::Id::FG_GREEN);
    const Colors::Define c_reset(Colors::Id::RESET);

    std::cout << c_title << FIXTURE_TITLE << SPACE << "2x12" << std::endl;
    fixt_s<double, 2, 12> fix2x12;
    fix_2x12(&fix2x12);
    pca(fix2x12);

    return 0;
}
