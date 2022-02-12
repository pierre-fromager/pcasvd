
#include <main.h>
#include <datafile.h>
#include <datatree.h>
#include <colors.h>
#include <colormap.h>
#include <display.h>
#include <pca.h>
#include <gplot.h>

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
        disp->mat(COV_MAT_TITLE, r.cov, cols, cols, 0);
        disp->mat(COR_MAT_TITLE, r.cor, cols, cols, 0);
        disp->mat(EIGEN_VECTORS_TITLE, r.eig_vectors, cols, cols);
        disp->vec(EIGEN_VALUES_TITLE, r.eig_values, cols);
        disp->vec(EXPLAINED_VARIANCE_TITLE, r.exp_variance, cols);
        disp->mat(PROJECTMAT_TITLE, r.proj, rows, cols, maxrow);
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
static void getCol(alglib::real_2d_array a, const ui_t &colnum, std::vector<T> &col)
{
    col.clear();
    ui_t i, j;
    const ui_t cols = a.cols();
    const ui_t rows = a.rows();
    for (j = 0; j < rows; j++)
        for (i = 0; i < cols; i++)
            if (i == colnum)
                col.push_back(a[j][i]);
            else if (i > colnum)
                continue;
}

template <typename T>
static void plotScatterWrapper(std::string filename, pca_result_s<T> &result)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.width = 1024;
    gparams.height = 768;
    gparams.title = "Scatter plot (Individual factor map) : Iris species";
    gparams.xlabel = "PC1 Petal.W(" + to_string(result.exp_variance[0] * 100) + "%)";
    gparams.ylabel = "PC2 Petal.L(" + to_string(result.exp_variance[1] * 100) + "%)";
    gparams.legend = "Setosa Versicolor Virginica";
    std::vector<T> col1, col2;
    getCol<T>(result.proj, 0, col1);
    const T margin = 0.5;
    gparams.lxrange = (*min_element(col1.begin(), col1.end())) - margin;
    gparams.hxrange = (*max_element(col1.begin(), col1.end())) + margin;
    getCol<T>(result.proj, 1, col2);
    gparams.lyrange = (*min_element(col2.begin(), col2.end())) - margin;
    gparams.hyrange = (*max_element(col2.begin(), col2.end())) + margin;
    T category;
    ui_t coSize = col1.size();
    for (ui_t c = 0; c < coSize; c++)
    {
        if (c < 52)
            category = 0;
        else if (c < 102)
            category = 1;
        else
            category = 2;
        gparams.serie_xyc.emplace_back(col1[c], col2[c], category);
    }
    col1.clear();
    col2.clear();
    Gplot<T> *gpl = new Gplot<T>();
    gpl->setParams(gparams);
    gpl->drawScatter();
    gparams.serie_xyc.clear();
    delete (gpl);
}

template <typename T>
static void plotCorCircleWrapper(std::string filename, pca_result_s<T> &result)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.height = 1024;
    gparams.width = gparams.height + 100;
    gparams.title = "Variable factor map : Iris (4 components)";
    gparams.xlabel = "PC1 Petal.W(" + to_string(result.exp_variance[0] * 100) + "%)";
    gparams.ylabel = "PC2 Petal.L(" + to_string(result.exp_variance[1] * 100) + "%)";
    gparams.legend = "SepLen SepWid PetLen PetWid";
    const T margin = 0.1;
    std::vector<T> col1, col2;
    getCol<T>(result.eig_vectors, 0, col1);
    getCol<T>(result.eig_vectors, 1, col2);
    gparams.lxrange = gparams.lyrange = -1 - margin;
    gparams.hxrange = gparams.hyrange = 1 + margin;
    T category;
    ui_t coSize = col1.size();
    for (ui_t c = 0; c < coSize; c++)
        gparams.serie_ooxyc.emplace_back(0, 0, col1[c], col2[c], c);
    col1.clear();
    col2.clear();
    Gplot<T> *gpl = new Gplot<T>();
    gpl->setParams(gparams);
    gpl->drawCorCircle();
    gparams.serie_ooxyc.clear();
    delete (gpl);
}

template <typename T>
static void plotHeatmapWrapper(std::string filename, pca_result_s<T> &result)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.height = gparams.width = 800;
    gparams.title = "Heatmap Correlation : Iris";
    gparams.lxrange = gparams.lyrange = -1;
    gparams.hxrange = gparams.hyrange = 1;
    gparams.xlabel = gparams.ylabel = "Components";
    gparams.legend = "Sepal.L,Sepal.W,Petal.L,Petal.W";
    gparams.mat = result.cor;
    Gplot<T> *gpl = new Gplot<T>();
    gpl->setParams(gparams);
    gpl->drawHeatmap();
    delete (gpl);
}

template <typename T>
static void savePcaResultJson(
    std::string title,
    std::string filename,
    pca_result_s<T> &result)
{
    Data::File::Tree<double> *datatree = new Data::File::Tree<double>();
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
    pcadetail(fixspecies, disp, result);

    savePcaResultJson(irisTitle, "pca_results.json", result);

    plotScatterWrapper<double>("pca_scatter.png", result);
    plotCorCircleWrapper<double>("pca_corcircle.png", result);
    plotHeatmapWrapper<double>("pca_heatmapcor.png", result);

    delete (dataset);
    delete (disp);

    return 0;
}