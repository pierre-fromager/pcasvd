
#include <main.h>
#include <data.h>
#include <colors.h>
#include <colormap.h>
#include <display.h>
#include <pca.h>

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

static void fix_csv_4x12(std::string filename, fixt_s<double, 4, 12> *fix)
{
    Data::File::Csv<double> *csv = new Data::File::Csv<double>(SEMICOLON);
    csv->load(filename, 1);
    Data::File::metas_t metas = csv->metas();
    fix->nbcol = metas.cols;
    fix->nbrow = metas.rows;
    const ui_t nbItems = fix->nbcol * fix->nbrow;
    std::copy_n(csv->buffer().begin(), nbItems, fix->values.begin());
    delete (csv);
}

static void fix_csv_bovin_6x23(std::string filename, fixt_s<double, 6, 23> *fix)
{
    Data::File::Csv<double> *csv = new Data::File::Csv<double>(SEMICOLON);
    csv->load(filename, 1);
    Data::File::metas_t metas = csv->metas();
    fix->nbcol = metas.cols;
    fix->nbrow = metas.rows;
    const ui_t nbItems = fix->nbcol * fix->nbrow;
    std::copy_n(csv->buffer().begin(), nbItems, fix->values.begin());
    delete (csv);
}

static void fix_csv_species_4x150(std::string filename, fixt_s<double, 4, 150> *fix)
{
    Data::File::Csv<double> *csv = new Data::File::Csv<double>(COMA);
    csv->load(filename, 1);
    Data::File::metas_t metas = csv->metas();
    fix->nbcol = metas.cols;
    fix->nbrow = metas.rows;
    const ui_t nbItems = fix->nbcol * fix->nbrow;
    std::copy_n(csv->buffer().begin(), nbItems, fix->values.begin());
    delete (csv);
}

template <typename T, ui_t NC, ui_t NR>
static void pcadetail(fixt_s<T, NC, NR> fix, Display *disp)
{
    const ui_t maxrow = 5;
    alglib::real_2d_array dataFrame;
    dataFrame.setcontent(NR, NC, (T *)&fix.values);
    Pca<T> *pca = new Pca<T>(dataFrame, NC, NR);
    pca->process();
    pca_result_s<T> r = pca->results();
    disp->mat(FIXTURE_DATA_TITLE, dataFrame, NR, NC, maxrow);
    disp->mat(COV_MAT_TITLE, r.cov, NC, NC);
    disp->mat(COR_MAT_TITLE, r.cor, NC, NC);
    disp->mat(PCA_EIGEN_VECTORS_TITLE, r.eig_vectors, NC, NC);
    disp->vec(PCA_EIGEN_VALUES_TITLE, r.eig_values, NC);
    disp->vec(PCA_EXPLAINED_VARIANCE_TITLE, r.exp_variance, NC);
    disp->mat(PCA_PROJECTMAT_TITLE, r.proj, NR, NC, maxrow);
    delete pca;
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

int main(int argc, char **argv)
{

    colormap_t colors;
    init_colormap(&colors);
    Display *disp = new Display(colors);

    /*
    disp->title("Fixture 2x12");
    fixt_s<double, 2, 12> fix2x12;
    fix_2x12(&fix2x12);
    pcadetail(fix2x12, disp);
 
    disp->title("Fixture csv gsaw 4x12");
    fixt_s<double, 4, 12> fixcsv4x12;
    fix_csv_4x12(FIXT_CSV_FILE_GSAW, &fixcsv4x12);
    pcadetail(fixcsv4x12, disp);

    disp->title("Fixture csv bovin 6x23");
    fixt_s<double, 6, 23> fixcsvbovin6x23;
    fix_csv_bovin_6x23(FIXT_CSV_FILE_BOVIN, &fixcsvbovin6x23);
    pcadetail(fixcsvbovin6x23, disp);
    */

    disp->title("Fixture csv iris species 4x150");
    fixt_s<double, 4, 150> fixcsvspecies4x150;
    fix_csv_species_4x150(FIXT_CSV_FILE_SPECIES, &fixcsvspecies4x150);
    pcadetail(fixcsvspecies4x150, disp);
    delete (disp);
    return 0;
}