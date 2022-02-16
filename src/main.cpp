
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <main.h>
#include <datafile.h>
#include <datatree.h>
#include <colors.h>
#include <colormap.h>
#include <display.h>
#include <pca.h>
#include <gplot.h>
#include <gplotwrapper.h>

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
    alglib::real_2d_array dataFrame;
    dataFrame.setcontent(NR, NC, fix.values.data());
    Pca<T> *pca = new Pca<T>(dataFrame);
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
        const ui_t &maxrow = 5;
        const ui_t &rows = pca->rows();
        const ui_t &cols = pca->cols();
        pca_result_s<T> r = result = pca->results();
        disp->mat(FIXTURE_DATA_TITLE, dataFrame, rows, cols, maxrow);
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
static void savePcaResult(
    const std::string &title,
    const std::string &filename,
    pca_result_s<T> &result)
{
    Data::File::Tree<T> *datatree = new Data::File::Tree<T>();
    datatree->addValue("version", "1.0.0");
    datatree->addValue("title", title);
    datatree->addValue("cols", result.cols);
    datatree->addValue("rows", result.rows);
    datatree->addVector("eig_values", result.eig_values);
    datatree->addVector("exp_variance", result.exp_variance);
    datatree->addMatrix("eig_vectors", result.eig_vectors);
    datatree->addMatrix("cov", result.cov);
    datatree->addMatrix("cor", result.cor);
    datatree->addMatrix("proj", result.proj);
    datatree->save(filename);
    delete (datatree);
}

int main(int argc, char **argv)
{
    colormap_t colors;
    init_colormap(&colors);
    Display *disp = new Display(colors);
    pca_result_s<double> result;
    Data::File::metas_t datasetMetas;
    Data::File::Csv<double> *dataset = new Data::File::Csv<double>();
    datasetMetas.sep = SEMICOLON;
    datasetMetas.skip = 1;
    const std::string &irisTitle = "Fixture csv iris species 4x150";
    disp->title(irisTitle);
    fixt_s<double, 4, 150> fixspecies;
    datasetMetas.sep = COMA;
    datasetMetas.filename = FIXT_CSV_FILE_SPECIES;
    dataset->setMetas(datasetMetas);
    hydrate_fix_csv(dataset, &fixspecies);
    delete (dataset);
    pcadetail(fixspecies, disp, result);
    boost::asio::thread_pool pool(3);
    boost::asio::post(pool, [&result, &disp] {
        disp->mat(COV_MAT_TITLE, result.cov, result.cols, result.cols, 0);
        disp->mat(COR_MAT_TITLE, result.cor, result.cols, result.cols, 0);
        disp->mat(EIGEN_VECTORS_TITLE, result.eig_vectors, result.cols, result.cols);
        disp->vec(EIGEN_VALUES_TITLE, result.eig_values, result.cols);
        disp->vec(EXPLAINED_VARIANCE_TITLE, result.exp_variance, result.cols);
        disp->mat(PROJECTMAT_TITLE, result.proj, result.rows, result.cols, 5);
        return static_cast<ui_t>(0);
    });
    boost::asio::post(pool, [&result, irisTitle] {
        savePcaResult(irisTitle, "pca_results.json", result);
        return static_cast<ui_t>(0);
    });
    boost::asio::post(pool, [&result] {
        GplotWrapper<double> *gpw = new GplotWrapper<double>(result);
        gpw->scatter("pca_scatteresult.png");
        gpw->corcricle("pca_corcircle.png");
        gpw->heatmap("pca_heatmapcoresult.png");
        gpw->boxwiskers("pca_boxwiskers.png", FIXT_CSV_FILE_SPECIES, COMA);
        delete (gpw);
        return static_cast<ui_t>(0);
    });
    pool.join();
    delete (disp);
    return 0;
}
