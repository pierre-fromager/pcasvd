
#ifndef GPLOT_GENERIC_H
#define GPLOT_GENERIC_H

#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <numeric>
#include <gplot.h>
#include <pca.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#define _SP_ " "
#define _SHARP_ "#"
#define _DIM1_ "Dim1 "
#define _DIM2_ "Dim2 "
#define _OPAR_  " ("
#define _CPAR_  "%)"
#define _COMPS_ "Components"
#define _IND_FAC_MAP_ "Individual factor map : "
#define _VAR_FAC_MAP_ "Variable factor map : "
#define _HEAT_COR_ "Heatmap Correlation : "
#define _BOX_WISK_ "Box and wiskers : "
#define _RATIOS_ "Ratios"

template <typename T>
class GplotGeneric : public Gplot<T>
{
public:
    explicit GplotGeneric(pca_result_s<T> &result);
    ~GplotGeneric();
    void scatter(const std::string &filename);
    void corcricle(const std::string &filename);
    void heatmap(const std::string &filename);
    void boxwiskers(const std::string &filename);

private:
    pca_result_s<T> m_result;
    std::vector<T> col1, col2, colLegend;
    void colsReset(void);
};

#endif
