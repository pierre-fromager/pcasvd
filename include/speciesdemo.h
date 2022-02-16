
#ifndef SPECIES_DEMO__H
#define SPECIES_DEMO__H

#define COV_MAT_TITLE "Covariance"
#define COR_MAT_TITLE "Correlation"
#define EIGEN_VECTORS_TITLE "Eigen vectors"
#define EIGEN_VALUES_TITLE "Eigen values\n"
#define EXPLAINED_VARIANCE_TITLE "\n\tExplained variance\n\t"
#define PROJECTMAT_TITLE "\n\tProjected matrix"
#define FIXTURE_TITLE "Fixture"
#define FIXTURE_DATA_TITLE FIXTURE_TITLE " Dataset"
#define ALGLIB_ERR_MSG "Alglib error : "
#define FIXT_CSV_FILE_SPECIES "./script/python/workspace/species.csv"
#define DISP_MAXROW 5
#define FIXT_IRIS_TITLE "Fixture csv iris species 4x150"

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <datafile.h>
#include <datatree.h>
#include <colors.h>
#include <colormap.h>
#include <display.h>
#include <pca.h>
#include <gplotspecies.h>

/**
 * @brief SpeciesDemo works on the well known (numpy users) Iris species dataset.
 * @brief Loading dataset from csv file.
 * @brief Processing dataset to provide coverage, correlation,pca,projection as result.
 * @brief Saving results as json tree.
 * @brief Rendering graphics scatter,corcircle,heatmap,boxwiskers through gnuplot.
 * 
 * @tparam T 
 * @tparam NC 
 * @tparam NR 
 */
template <typename T, ui_t NC, ui_t NR>
class SpeciesDemo
{
public:
    explicit SpeciesDemo();
    ~SpeciesDemo();
    void run(void);

private:
    ui_t m_maxrow;
    Data::File::Tree<T> *m_datatree;
    void hydrate_fix_csv(Data::File::Csv<T> *csv, fixt_s<T, NC, NR> *fix);
    void pcadetail(fixt_s<T, NC, NR> fix, Display *disp, pca_result_s<T> &result);
    void init_colormap(colormap_t *colormap);
    void savePcaResult(
        const std::string &title,
        const std::string &filename,
        pca_result_s<T> &result);
};

#endif
