
#ifndef MAIN_H
#define MAIN_H

#include <array>
#include <vector>

#define COV_MAT_TITLE "Covariance (matrix)"
#define COR_MAT_TITLE "Correlation (matrix)"
#define PCA_EIGEN_VECTORS_TITLE "Eigen vectors (matrix)"
#define PCA_EIGEN_VALUES_TITLE "Eigen values (vector)\n"
#define PCA_EXPLAINED_VARIANCE_TITLE "\n\tExplained variance\n"
#define PCA_PROJECTMAT_TITLE "\n\tProjected matrix"
#define FIXTURE_TITLE "Fixture"
#define FIXTURE_DATA_TITLE FIXTURE_TITLE " datas (matrix)"
#define ALGLIB_ERR_MSG "Alglib error : "
#define FIXT_CSV_FILE_GSAW "./script/matlab/gsaw.csv"
#define FIXT_CSV_FILE_BOVIN "./script/matlab/bovin.csv"
#define FIXT_CSV_FILE_SPECIES "./script/python/workspace/species.csv"

typedef unsigned int ui_t;

template <typename T, ui_t NC, ui_t NR>
struct fixt_s
{
    ui_t nbcol = NC;
    ui_t nbrow = NR;
    std::array<T, NC * NR> values;
};

#endif
