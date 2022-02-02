
#include <main.h>
#include <data.h>
#include <colors.h>
#include <colormap.h>
#include <display.h>
#include <pca.h>
#include <gplot.h>

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
static void hydrate_fix_csv(Data::File::Csv<T> *csv, fixt_s<T, NC, NR> *fix)
{
    Data::File::metas_t metas = csv->metas();
    csv->load();
    metas = csv->metas();
    fix->nbcol = metas.cols;
    fix->nbrow = metas.rows;
    const ui_t nbItems = fix->nbcol * fix->nbrow;
    std::copy_n(csv->buffer().begin(), nbItems, fix->values.begin());
}

template <typename T, ui_t NC, ui_t NR>
static void pcadetail(fixt_s<T, NC, NR> fix, Display *disp, pca_result_s<T> &result)
{
    const ui_t maxrow = 5;
    alglib::real_2d_array dataFrame;
    dataFrame.setcontent(NR, NC, (T *)&fix.values);
    Pca<T> *pca = new Pca<T>(dataFrame, NC, NR);
    bool err = false;
    try
    {
        pca->process();
    }
    catch (alglib::ap_error e)
    {
        err = true;
        disp->error(e.msg);
    }
    if (false == err)
    {
        pca_result_s<T> r = result = pca->results();
        disp->mat(FIXTURE_DATA_TITLE, dataFrame, NR, NC, maxrow);
        disp->mat(COV_MAT_TITLE, r.cov, NC, NC, 0);
        disp->mat(COR_MAT_TITLE, r.cor, NC, NC, 0);
        disp->mat(EIGEN_VECTORS_TITLE, r.eig_vectors, NC, NC);
        disp->vec(EIGEN_VALUES_TITLE, r.eig_values, NC);
        disp->vec(EXPLAINED_VARIANCE_TITLE, r.exp_variance, NC);
        disp->mat(PROJECTMAT_TITLE, r.proj, NR, NC, maxrow);
    }
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

template <typename T>
static void saveproj(
    Data::File::metas_t &datasetMetas,
    Data::File::Csv<T> *dataset,
    pca_result_s<T> &result)
{
    datasetMetas = dataset->metas();
    datasetMetas.filename = "iris_proj.csv";
    datasetMetas.header = "C1;C2;C3;C4";
    datasetMetas.sep = SEMICOLON;
    std::vector<double> v;
    for (ui_t j = 0; j < result.rows; j++)
        for (ui_t i = 0; i < result.cols; i++)
            v.push_back(result.proj[j][i]);
    dataset->setBuffer(v);
    v.clear();
    dataset->setMetas(datasetMetas);
    dataset->save();
}

template <typename T>
static void getCol(alglib::real_2d_array a, const ui_t &colnum, std::vector<T> &col)
{
    col.clear();
    for (ui_t j = 0; j < a.rows(); j++)
        for (ui_t i = 0; i < a.cols(); i++)
            if (i == colnum)
                col.push_back(a[j][i]);
            else if (i > colnum)
                continue;
}

template <typename T>
static void plotProjWrapper(std::string filename, pca_result_s<T> &result)
{
    struct gplot_params_s<T> gparams;
    gparams.title = "Iris Species";
    gparams.legend = "Setosa Versicolor Virginica";
    gparams.filename = filename;
    std::vector<T> col1, col2;
    getCol<T>(result.proj, 0, col1);
    const T margin = 0.5;
    gparams.lxrange = (*min_element(col1.begin(), col1.end())) - margin;
    gparams.hxrange = (*max_element(col1.begin(), col1.end())) + margin;
    getCol<T>(result.proj, 1, col2);
    gparams.lyrange = (*min_element(col2.begin(), col2.end())) - margin;
    gparams.hyrange = (*max_element(col2.begin(), col2.end())) + margin;
    T category;
    for (ui_t c = 0; c < col1.size(); c++)
    {
        if (c < 52)
            category = 0;
        else if (c < 102)
            category = 1;
        else
            category = 2;
        gparams.xyc.emplace_back(col1[c], col2[c], category);    
    }
    col1.clear();
    col2.clear();
    Gplot<T> *gpl = new Gplot<T>(gparams);
    gpl->drawScatter();
    delete (gpl);
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
    pcadetail(fix2x12, disp, result);
    */
    Data::File::metas_t datasetMetas;
    Data::File::Csv<double> *dataset = new Data::File::Csv<double>();
    datasetMetas.sep = SEMICOLON;
    datasetMetas.skip = 1;
    pca_result_s<double> result;
    /*
    disp->title("Fixture csv gsaw 4x12");
    fixt_s<double, 4, 12> fixgsaw;
    datasetMetas.filename = FIXT_CSV_FILE_GSAW;
    dataset->setMetas(datasetMetas);
    hydrate_fix_csv(dataset, &fixgsaw);
    pcadetail(fixgsaw, disp, result);

    disp->title("Fixture csv bovin 6x23");
    fixt_s<double, 6, 23> fixbovin;
    datasetMetas.filename = FIXT_CSV_FILE_BOVIN;
    dataset->setMetas(datasetMetas);
    hydrate_fix_csv(dataset, &fixbovin);
    pcadetail(fixbovin, disp, result);
    */
    disp->title("Fixture csv iris species 4x150");
    fixt_s<double, 4, 150> fixspecies;
    datasetMetas.sep = COMA;
    datasetMetas.filename = FIXT_CSV_FILE_SPECIES;
    dataset->setMetas(datasetMetas);
    hydrate_fix_csv(dataset, &fixspecies);
    pcadetail(fixspecies, disp, result);

    saveproj<double>(datasetMetas, dataset, result);

    plotProjWrapper<double>("gplot-draw-proj.png", result);

    delete (dataset);
    delete (disp);
    return 0;
}