
#include <speciesdemo.h>

template <typename T>
SpeciesDemo<T>::SpeciesDemo(Data::File::metas_t metas) : m_maxrow(DISP_MAXROW), m_dataset_metas(metas)
{
    init_colormap(&m_colors);
    m_disp = new Display(m_colors);
    m_datatree = new Data::File::Tree<T>();
    m_dataset = new Data::File::Csv<T>();
}

template <typename T>
SpeciesDemo<T>::~SpeciesDemo()
{
    delete (m_disp);
    delete (m_datatree);
    delete (m_dataset);
}

template <typename T>
void SpeciesDemo<T>::hydrate_fix_csv(
    Data::File::Csv<T> *csv,
    fixtv_s<T> *fix)
{
    Data::File::metas_t metas = csv->metas();
    csv->load();
    metas = csv->metas();
    fix->nbcol = metas.cols;
    fix->nbrow = metas.rows;
    const ui_t nbItems = fix->nbcol * fix->nbrow;
    fix->values = csv->buffer();
}

template <typename T>
void SpeciesDemo<T>::pcadetail(fixtv_s<T> fix, pca_result_s<T> &r)
{
    alglib::real_2d_array df;
    df.setcontent(fix.nbrow, fix.nbcol, fix.values.data());
    Pca<T> *pca = new Pca<T>(df);
    bool err = false;
    try
    {
        pca->process();
    }
    catch (alglib::ap_error e)
    {
        err = true;
        m_disp->error(e.msg);
    }
    if (false == err)
    {
        r = pca->results();
        m_disp->mat(FIXTURE_DATA_TITLE, df, pca->rows(), pca->cols(), m_maxrow);
    }
    delete pca;
}

template <typename T>
void SpeciesDemo<T>::init_colormap(colormap_t *colormap)
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
void SpeciesDemo<T>::savePcaResult(
    const std::string &title,
    const std::string &fname)
{
    m_datatree->addValue(_DTREE_VERSION_, DTREE_VERSION);
    m_datatree->addValue(_DTREE_TITLE_, title);
    m_datatree->addValue(_DTREE_COLS_, m_result.cols);
    m_datatree->addValue(_DTREE_ROWS_, m_result.rows);
    m_datatree->addVector(_DTREE_EIG_VALUES_, m_result.eig_values);
    m_datatree->addVector(_DTREE_EXP_VARIANCE_, m_result.exp_variance);
    m_datatree->addMatrix(_DTREE_EIG_VECTS_, m_result.eig_vectors);
    m_datatree->addMatrix(_DTREE_COV_, m_result.cov);
    m_datatree->addMatrix(_DTREE_COR_, m_result.cor);
    m_datatree->addMatrix(_DTREE_PROJ_, m_result.proj);
    m_datatree->save(fname);
}

template <typename T>
void SpeciesDemo<T>::run(void)
{
    const std::string &title = "Dataset loaded from " + m_dataset_metas.filename;
    m_disp->title(title);
    m_dataset->setMetas(m_dataset_metas);
    hydrate_fix_csv(m_dataset, &m_fix_species);
    pcadetail(m_fix_species, m_result);
    boost::asio::thread_pool pool(3);
    boost::asio::post(pool, [this] {
        const ui_t &c = m_result.cols;
        const ui_t &r = m_result.rows;
        m_disp->mat(COV_MAT_TITLE, m_result.cov, c, c, 0);
        m_disp->mat(COR_MAT_TITLE, m_result.cor, c, c, 0);
        m_disp->mat(EIGEN_VECTORS_TITLE, m_result.eig_vectors, c, c);
        m_disp->vec(EIGEN_VALUES_TITLE, m_result.eig_values, c);
        m_disp->vec(EXPLAINED_VARIANCE_TITLE, m_result.exp_variance, c);
        m_disp->mat(PROJECTMAT_TITLE, m_result.proj, r, c, m_maxrow);
        return static_cast<ui_t>(0);
    });
    boost::asio::post(pool, [this] {
        savePcaResult(m_dataset_metas.filename, JSON_PCA_RESULT_FILENAME);
        return static_cast<ui_t>(0);
    });
    if (m_dataset_metas.filename == FIXT_CSV_FILE_SPECIES)
    {
        boost::asio::post(pool, [this] {
            auto *gpw = new GplotSpecies<double>(m_result);
            gpw->scatter(PNG_SCATTER_FILENAME);
            gpw->corcricle(PNG_CORCIRCLE_FILENAME);
            gpw->heatmap(PNG_HEATMAP_FILENAME);
            gpw->boxwiskers(PNG_BOXWISK_FILENAME, FIXT_CSV_FILE_SPECIES, COMA);
            delete (gpw);
            return static_cast<ui_t>(0);
        });
    }
    pool.join();
}

template class SpeciesDemo<double>;
