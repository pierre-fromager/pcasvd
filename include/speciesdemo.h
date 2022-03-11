
#ifndef SPECIES_DEMO__H
#define SPECIES_DEMO__H

#define COV_MAT_TITLE "Covariance"
#define COR_MAT_TITLE "Correlation"
#define EIGEN_VECTORS_TITLE "Eigen vectors"
#define EIGEN_VALUES_TITLE "Eigen values\n"
#define EXPLAINED_VARIANCE_TITLE "\n\tExplained variance\n\t"
#define PROJECTMAT_TITLE "\n\tProjection sample"
#define FIXTURE_DATA_TITLE "Dataset sample"
#define ALGLIB_ERR_MSG "Alglib error : "
#define FIXT_CSV_FILE_SPECIES "./script/python/workspace/species.csv"
#define DISP_MAXROW 5
#define JSON_PCA_RESULT_FILENAME "pca_results.json"
#define PNG_SCATTER_FILENAME "pca_scatter.png"
#define PNG_CORCIRCLE_FILENAME "pca_corcircle.png"
#define PNG_HEATMAP_FILENAME "pca_heatmapcor.png"
#define PNG_BOXWISK_FILENAME "pca_boxwiskers.png"
#define _DTREE_VERSION_ "version"
#define DTREE_VERSION "1.0.0"
#define _DTREE_TITLE_ "title"
#define _DTREE_FILENAME_ "metas.filename"
#define _DTREE_HEADERS_ "metas.header"
#define _DTREE_DELIMITER_ "metas.delimiter"
#define _DTREE_COLS_ "cols"
#define _DTREE_ROWS_ "rows"
#define _DTREE_OPTS_D1_ "options.d1"
#define _DTREE_OPTS_D2_ "options.d2"
#define _DTREE_SRC_ "src"
#define _DTREE_EIG_VALUES_ "eig_values"
#define _DTREE_EXP_VARIANCE_ "exp_variance"
#define _DTREE_EIG_VECTS_ "eig_vectors"
#define _DTREE_COV_ "cov"
#define _DTREE_COR_ "cor"
#define _DTREE_PROJ_ "proj"

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <datafile.h>
#include <datatree.h>
#include <colors.h>
#include <colormap.h>
#include <display.h>
#include <pca.h>
#include <gplotspecies.h>
#include <gplotgeneric.h>
#include <options.h>

/**
 * @brief Loading dataset from csv file checking delimiters and setting headers if defined.
 * @brief Processing dataset to provide coverage,correlation,pca,projection as result.
 * @brief Saving results as json tree.
 * @brief Rendering graphics scatter,corcircle,heatmap,boxwiskers.
 *
 * @tparam T
 */
template <typename T>
class SpeciesDemo
{
public:
    explicit SpeciesDemo(Data::File::metas_t metas,cmd_options_t cmdopts);
    ~SpeciesDemo();
    void run(void);

private:
    ui_t m_maxrow;
    colormap_t m_colors;
    Data::File::metas_t m_dataset_metas;
    fixtv_s<T> m_fix;
    Display *m_disp;
    Data::File::Csv<T> *m_dataset;
    Data::File::Tree<T> *m_datatree;
    pca_result_s<T> m_result;
    cmd_options_t m_opts;
    void hydrate();
    void pcadetail();
    void initColormap(colormap_t *colormap);
    void savePcaResult(const std::string &title, const std::string &filename);
};

#endif
