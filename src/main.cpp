
#include "main.h"

static void dispm(
    std::string title,
    alglib::real_2d_array v,
    ui_t nbrow,
    ui_t nbcol,
    colormap_t colors)
{
    ui_t i, j;
    std::cout << TAB << colors.sub_title << title << colors.reset << std::endl;
    std::cout << colors.values;
    for (j = 0; j < nbrow; j++)
    {
        for (i = 0; i < nbcol; i++)
            std::cout << std::setw(11) << std::left << TAB << v[j][i];
        std::cout << std::endl;
    }
    std::cout << colors.reset;
}

static void dispv(
    std::string title,
    alglib::real_1d_array v,
    ui_t nbcol,
    colormap_t colors)
{
    ui_t i;
    std::cout << TAB << colors.sub_title << title << colors.reset << std::endl;
    std::cout << colors.values;
    for (i = 0; i < nbcol; i++)
        std::cout << std::setw(11) << std::left << TAB << v[i];
    std::cout << colors.reset << std::endl;
}

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

template <typename T, ui_t NC, ui_t NR>
static void pca(fixt_s<T, NC, NR> fix, colormap_t colors)
{
    try
    {
        const ui_t r = fix.nbrow;
        const ui_t c = fix.nbcol;
        alglib::real_2d_array ptInput, mcov, mcorr;
        ptInput.setcontent(r, c, (T *)&fix.values);
        dispm(FIXTURE_DATA_TITLE, ptInput, r, c, colors);
        alglib::covm(ptInput, r, c, mcov);
        dispm(COV_MAT_TITLE, mcov, c, c, colors);
        alglib::pearsoncorrm(ptInput, r, c, mcorr);
        dispm(COR_MAT_TITLE, mcorr, c, c, colors);

        alglib::ae_int_t info;
        alglib::real_1d_array eigValues;
        alglib::real_2d_array eigVectors;
        alglib::pcabuildbasis(ptInput, r, c, info, eigValues, eigVectors);
        dispm(PCA_EIGEN_VECTORS_TITLE, eigVectors, c, c, colors);
        dispv(PCA_EIGEN_VALUES_TITLE, eigValues, c, colors);

        T eigvaSum = 0;
        for (ui_t i = 0; i < c; i++)
            eigvaSum += eigValues[i];
        std::cout << TAB << colors.sub_title
                  << "Explained variance (%)"
                  << colors.reset << std::endl;
        for (ui_t i = 0; i < c; i++)
            std::cout << TAB TAB << "P" << i << SPACE
                      << (eigValues[i] * 100 / eigvaSum)
                      << std::endl;

        alglib::real_1d_array w;
        alglib::fisherlda(ptInput, r, c, c, info, w);
        dispv("Lda", w, c, colors);
    }
    catch (alglib::ap_error e)
    {
        printf(ALGLIB_ERR_MSG, e.msg.c_str());
    }
}

static void init_colormap(colormap_t *colormap)
{
    const Colors::Define c_title(Colors::Id::FG_GREEN);
    const Colors::Define c_sub_title(Colors::Id::FG_CYAN);
    const Colors::Define c_values(Colors::Id::FG_WHITE);
    const Colors::Define c_reset(Colors::Id::RESET);
    Colors::Define &ctitle = const_cast<Colors::Define &>(c_title);
    Colors::Define &csubtitle = const_cast<Colors::Define &>(c_sub_title);
    Colors::Define &cvalues = const_cast<Colors::Define &>(c_values);
    Colors::Define &creset = const_cast<Colors::Define &>(c_reset);
    colormap->main_title = ctitle.to_str();
    colormap->sub_title = csubtitle.to_str();
    colormap->values = cvalues.to_str();
    colormap->reset = creset.to_str();
}

int main(int argc, char **argv)
{
    colormap_t colors;
    init_colormap(&colors);

    std::cout << colors.main_title << FIXTURE_TITLE << SPACE << "2x12" << std::endl;
    fixt_s<double, 2, 12> fix2x12;
    fix_2x12(&fix2x12);
    pca(fix2x12, colors);

    return 0;
}
