
#include "main.h"
#include "data.h"
#include "display.h"
#include "colors.h"
#include "alglib/statistics.h"
#include "alglib/dataanalysis.h"

template <typename T, ui_t NC, ui_t NR>
struct fixt_s
{
    ui_t nbcol = NC;
    ui_t nbrow = NR;
    std::array<T, NC * NR> values;
};

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
static void pca(fixt_s<T, NC, NR> fix, Display *disp)
{
    try
    {
        const ui_t r = fix.nbrow;
        const ui_t c = fix.nbcol;
        alglib::real_2d_array ptInput, mcov, mcorr;
        ptInput.setcontent(r, c, (T *)&fix.values);
        disp->mat(FIXTURE_DATA_TITLE, ptInput, r, c);
        alglib::covm(ptInput, r, c, mcov);
        disp->mat(COV_MAT_TITLE, mcov, c, c);
        alglib::pearsoncorrm(ptInput, r, c, mcorr);
        disp->mat(COR_MAT_TITLE, mcorr, c, c);

        alglib::ae_int_t info;
        alglib::real_1d_array eigValues;
        alglib::real_2d_array eigVectors;
        alglib::pcabuildbasis(ptInput, r, c, info, eigValues, eigVectors);
        disp->mat(PCA_EIGEN_VECTORS_TITLE, eigVectors, c, c);
        disp->vec(PCA_EIGEN_VALUES_TITLE, eigValues, c);

        T eigvaSum = 0;
        for (ui_t i = 0; i < c; i++)
            eigvaSum += eigValues[i];

        disp->title("Explained variance (%)");
        for (ui_t i = 0; i < c; i++)
            std::cout << TAB TAB << "P" << i << SPACE
                      << (eigValues[i] * 100 / eigvaSum)
                      << std::endl;

        alglib::real_1d_array w;
        alglib::fisherlda(ptInput, r, c, c, info, w);
        disp->vec("Lda", w, c);
    }
    catch (alglib::ap_error e)
    {
        disp->error(ALGLIB_ERR_MSG + e.msg);
    }
}

static void init_colormap(colormap_t *colormap)
{
    const Colors::Define c_title(Colors::Id::FG_GREEN);
    const Colors::Define c_sub_title(Colors::Id::FG_CYAN);
    const Colors::Define c_values(Colors::Id::FG_WHITE);
    const Colors::Define c_error(Colors::Id::FG_RED);
    const Colors::Define c_reset(Colors::Id::RESET);
    Colors::Define &ctitle = const_cast<Colors::Define &>(c_title);
    Colors::Define &csubtitle = const_cast<Colors::Define &>(c_sub_title);
    Colors::Define &cvalues = const_cast<Colors::Define &>(c_values);
    Colors::Define &cerror = const_cast<Colors::Define &>(c_error);
    Colors::Define &creset = const_cast<Colors::Define &>(c_reset);
    colormap->main_title = ctitle.to_str();
    colormap->sub_title = csubtitle.to_str();
    colormap->values = cvalues.to_str();
    colormap->reset = creset.to_str();
}

static void csv_metas(const std::string &filename)
{
    Data::File::Csv<double> *csv = new Data::File::Csv<double>(SEMICOLON);
    csv->load(filename, 1);
    Data::File::metas_t metas = csv->metas();
    std::cout << "filename : " << metas.filename << std::endl
              << "separator : " << metas.sep << std::endl
              << "cols : " << metas.cols << std::endl
              << "rows : " << metas.rows << std::endl;
    delete (csv);
}

int main(int argc, char **argv)
{
    colormap_t colors;
    init_colormap(&colors);
    Display *disp = new Display(colors);
    csv_metas("./script/matlab/gsaw.csv");
    std::cout << colors.main_title << FIXTURE_TITLE << SPACE << "2x12" << std::endl;
    fixt_s<double, 2, 12> fix2x12;
    fix_2x12(&fix2x12);
    pca(fix2x12, disp);
    delete(disp);
    return 0;
}
