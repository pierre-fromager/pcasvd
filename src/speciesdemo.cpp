
#include <speciesdemo.h>

template <typename T, ui_t NC, ui_t NR>
SpeciesDemo<T, NC, NR>::SpeciesDemo() : m_maxrow(DISP_MAXROW)
{
    m_datatree = new Data::File::Tree<T>();
}

template <typename T, ui_t NC, ui_t NR>
SpeciesDemo<T, NC, NR>::~SpeciesDemo()
{
    delete(m_datatree);
}

template <typename T, ui_t NC, ui_t NR>
void SpeciesDemo<T, NC, NR>::hydrate_fix_csv(
    Data::File::Csv<T> *csv, 
    fixt_s<T, NC, NR> *fix)
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
void SpeciesDemo<T, NC, NR>::pcadetail(
    fixt_s<T, NC, NR> fix, 
    Display *disp, 
    pca_result_s<T> &result)
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
        result = pca->results();
        disp->mat(FIXTURE_DATA_TITLE, dataFrame, pca->rows(), pca->cols(), m_maxrow);
    }
    delete pca;
}

template <typename T, ui_t NC, ui_t NR>
void SpeciesDemo<T, NC, NR>::init_colormap(colormap_t *colormap)
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

template <typename T, ui_t NC, ui_t NR>
void SpeciesDemo<T, NC, NR>::savePcaResult(
    const std::string &title,
    const std::string &filename,
    pca_result_s<T> &result)
{
    m_datatree->addValue("version", "1.0.0");
    m_datatree->addValue("title", title);
    m_datatree->addValue("cols", result.cols);
    m_datatree->addValue("rows", result.rows);
    m_datatree->addVector("eig_values", result.eig_values);
    m_datatree->addVector("exp_variance", result.exp_variance);
    m_datatree->addMatrix("eig_vectors", result.eig_vectors);
    m_datatree->addMatrix("cov", result.cov);
    m_datatree->addMatrix("cor", result.cor);
    m_datatree->addMatrix("proj", result.proj);
    m_datatree->save(filename);
}

template <typename T, ui_t NC, ui_t NR>
void SpeciesDemo<T, NC, NR>::run(void)
{
    colormap_t colors;
    init_colormap(&colors);
    Display *disp = new Display(colors);
    pca_result_s<double> result;
    Data::File::metas_t datasetMetas;
    Data::File::Csv<double> *dataset = new Data::File::Csv<double>();
    datasetMetas.sep = SEMICOLON;
    datasetMetas.skip = 1;
    disp->title(FIXT_IRIS_TITLE);
    fixt_s<double, 4, 150> fixspecies;
    datasetMetas.sep = COMA;
    datasetMetas.filename = FIXT_CSV_FILE_SPECIES;
    dataset->setMetas(datasetMetas);
    hydrate_fix_csv(dataset, &fixspecies);
    delete (dataset);
    pcadetail(fixspecies, disp, result);
    boost::asio::thread_pool pool(3);
    boost::asio::post(pool, [this, &result, &disp] {
        disp->mat(COV_MAT_TITLE, result.cov, result.cols, result.cols, 0);
        disp->mat(COR_MAT_TITLE, result.cor, result.cols, result.cols, 0);
        disp->mat(EIGEN_VECTORS_TITLE, result.eig_vectors, result.cols, result.cols);
        disp->vec(EIGEN_VALUES_TITLE, result.eig_values, result.cols);
        disp->vec(EXPLAINED_VARIANCE_TITLE, result.exp_variance, result.cols);
        disp->mat(PROJECTMAT_TITLE, result.proj, result.rows, result.cols, m_maxrow);
        return static_cast<ui_t>(0);
    });
    boost::asio::post(pool, [this, &result] {
        savePcaResult(FIXT_IRIS_TITLE, "pca_results.json", result);
        return static_cast<ui_t>(0);
    });
    boost::asio::post(pool, [&result] {
        auto *gpw = new GplotSpecies<double>(result);
        gpw->scatter("pca_scatter.png");
        gpw->corcricle("pca_corcircle.png");
        gpw->heatmap("pca_heatmapcor.png");
        gpw->boxwiskers("pca_boxwiskers.png", FIXT_CSV_FILE_SPECIES, COMA);
        delete (gpw);
        return static_cast<ui_t>(0);
    });
    pool.join();
    delete (disp);
}

template class SpeciesDemo<double, 4, 150>;
