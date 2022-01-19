
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

template <typename T, unsigned int NC, unsigned int NR>
struct fixture_s
{
    unsigned int nbcol = NC;
    unsigned int nbrow = NR;
    std::array<T, NC * NR> values;
};

//typedef fixture_s<double> fixure_d_t;

#endif
