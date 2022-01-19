
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
#define COV_MAT_TITLE "CovMatrix"
#define COV_EIGEN_TITLE "EigenBasis"
#define FIXTURE_TITLE "Fixture"
#define FIXTURE_DATA_TITLE FIXTURE_TITLE " Datas"
#define ALGLIB_ERR_MSG "Alglib err msg: %s\n"

typedef unsigned int ui_t;

template <typename T, ui_t NC, ui_t NR>
struct fixt_s
{
    ui_t nbcol = NC;
    ui_t nbrow = NR;
    std::array<T, NC * NR> values;
};

#endif
