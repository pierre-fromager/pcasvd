
#ifndef MAIN_H
#define MAIN_H

#include <ostream>

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

template <typename T>
struct fixture_s
{
    unsigned int nbcol;
    unsigned int nbrow;
    T *values;
};

typedef fixture_s<double> fixure_d_t;

#endif
