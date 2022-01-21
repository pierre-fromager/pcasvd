
#ifndef MAIN_H
#define MAIN_H

#include <ostream>
#include <array>
#include <iostream>
#include <iomanip>
#include "colors.h"
#include "alglib/statistics.h"
#include "alglib/dataanalysis.h"

#define TAB "\t"
#define STAR "*"
#define SPACE " "
#define DASH "-"
#define COV_MAT_TITLE "Covariance (matrix)"
#define COR_MAT_TITLE "Correlation (matrix)"
#define PCA_EIGEN_VECTORS_TITLE "Eigen vectors (matrix)"
#define PCA_EIGEN_VALUES_TITLE "Eigen values (vector)"
#define FIXTURE_TITLE "Fixture"
#define FIXTURE_DATA_TITLE FIXTURE_TITLE " datas (matrix)"
#define ALGLIB_ERR_MSG "Alglib error : "

typedef unsigned int ui_t;

typedef struct colormap_s {
    std::string main_title;
    std::string sub_title;
    std::string values;
    std::string error;
    std::string reset;
} colormap_t;

template <typename T, ui_t NC, ui_t NR>
struct fixt_s
{
    ui_t nbcol = NC;
    ui_t nbrow = NR;
    std::array<T, NC * NR> values;
};

#endif
