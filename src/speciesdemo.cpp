
#include <speciesdemo.h>

template <typename T>
SpeciesDemo<T>::SpeciesDemo(
    Data::File::metas_t metas,
    cmd_options_t cmdopts) : m_maxrow(DISP_MAXROW),
                             m_dataset_metas(metas),
                             m_opts(cmdopts)
{
    initColormap(&m_colors);
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
void SpeciesDemo<T>::hydrate()
{
    Data::File::metas_t metas = m_dataset->metas();
    m_dataset->load();
    metas = m_dataset->metas();
    m_fix.nbcol = metas.cols;
    m_fix.nbrow = metas.rows;
    const ui_t nbItems = m_fix.nbcol * m_fix.nbrow;
    m_fix.values = m_dataset->buffer();
    m_fix.header = m_dataset->metas().header;
    m_fix.delimiter = m_dataset->metas().delimiter;
    m_fix.filename = m_dataset->metas().filename;
    m_fix.cmdopts.d1c = m_opts.d1c;
    m_fix.cmdopts.d2c = m_opts.d2c;
}

template <typename T>
void SpeciesDemo<T>::pcadetail()
{
    alglib::real_2d_array df;
    m_result.src.setcontent(m_fix.nbrow, m_fix.nbcol, m_fix.values.data());
    Pca<T> *pca = new Pca<T>(m_result.src);
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
        m_result = pca->results();
        m_result.header = m_fix.header;
        m_result.delimiter = m_fix.delimiter;
        m_result.filename = m_fix.filename;
        m_result.opts.d1c = m_opts.d1c;
        m_result.opts.d2c = m_opts.d2c;
    }
    delete pca;
}

template <typename T>
void SpeciesDemo<T>::initColormap(colormap_t *colormap)
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
    m_datatree->addValue(_DTREE_FILENAME_, m_result.filename);
    m_datatree->addValue(_DTREE_HEADERS_, m_fix.header);
    m_datatree->addValue(_DTREE_DELIMITER_, m_fix.delimiter);
    m_datatree->addValue(_DTREE_COLS_, m_result.cols);
    m_datatree->addValue(_DTREE_ROWS_, m_result.rows);
    m_datatree->addValue(_DTREE_OPTS_D1_, m_result.opts.d1c);
    m_datatree->addValue(_DTREE_OPTS_D2_, m_result.opts.d2c);
    m_datatree->addMatrix(_DTREE_SRC_, m_result.src);
    m_datatree->addVector(_DTREE_EIG_VALUES_, m_result.eig_values);
    m_datatree->addVector(_DTREE_EXP_VARIANCE_, m_result.exp_variance);
    m_datatree->addMatrix(_DTREE_EIG_VECTS_, m_result.eig_vectors);
    m_datatree->addMatrix(_DTREE_COV_, m_result.cov);
    m_datatree->addMatrix(_DTREE_COR_, m_result.cor);
    m_datatree->addMatrix(_DTREE_PROJ_, m_result.proj);
    m_datatree->save(fname);
}

static bool isSpeciesFile(std::string filename)
{
    return (filename == FIXT_CSV_FILE_SPECIES);
}

/**
 * @brief run demo
 * @todo refacto gplot thread to use a common instance
 * @todo https://stackoverflow.com/questions/6179314/casting-pointers-and-the-ternary-operator-have-i-reinvented-the-wheel
 * 
 * @tparam T 
 */
template <typename T>
void SpeciesDemo<T>::run(void)
{
    const std::string &title = "Dataset loaded from " + m_dataset_metas.filename;
    m_disp->title(title);
    m_dataset->setMetas(m_dataset_metas);
    hydrate();
    m_dataset->reset();
    pcadetail();
    boost::asio::thread_pool pool(3);

    boost::asio::post(pool, [this]
                      {
        const ui_t &c = m_result.cols;
        const ui_t &r = m_result.rows;
        m_disp->mat(FIXTURE_DATA_TITLE, m_result.src, r, c, m_maxrow);
        m_disp->mat(COV_MAT_TITLE, m_result.cov, c, c, 0);
        m_disp->mat(COR_MAT_TITLE, m_result.cor, c, c, 0);
        m_disp->mat(EIGEN_VECTORS_TITLE, m_result.eig_vectors, c, c);
        m_disp->vec(EIGEN_VALUES_TITLE, m_result.eig_values, c);
        m_disp->vec(EXPLAINED_VARIANCE_TITLE, m_result.exp_variance, c);
        m_disp->mat(PROJECTMAT_TITLE, m_result.proj, r, c, m_maxrow);
        return static_cast<ui_t>(0); });

    boost::asio::post(pool, [this]
                      {
        std::string baseFilename = m_dataset_metas.filename.substr(
            m_dataset_metas.filename.find_last_of("/\\") + 1);
        std::string::size_type const p(baseFilename.find_last_of('.'));
        std::string const &jtitle = baseFilename.substr(0, p);
        savePcaResult(jtitle, JSON_PCA_RESULT_FILENAME);
        return static_cast<ui_t>(0); });

    
    boost::asio::post(pool, [this]
                      {
            if (true== isSpeciesFile(m_dataset_metas.filename)){
                auto *gpw = new GplotSpecies<double>(m_result);
                gpw->scatter(PNG_SCATTER_FILENAME);
                gpw->corcricle(PNG_CORCIRCLE_FILENAME);
                gpw->heatmap(PNG_HEATMAP_FILENAME);
                gpw->boxwiskers(PNG_BOXWISK_FILENAME);
                delete gpw;
            } else {
                auto *gpw = new GplotGeneric<double>(m_result);
                gpw->scatter(PNG_SCATTER_FILENAME);
                gpw->corcricle(PNG_CORCIRCLE_FILENAME);
                gpw->heatmap(PNG_HEATMAP_FILENAME);
                gpw->boxwiskers(PNG_BOXWISK_FILENAME);
                delete gpw;
            }
            return static_cast<ui_t>(0); });
    pool.join();
}

template class SpeciesDemo<double>;
