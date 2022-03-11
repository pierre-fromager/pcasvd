#ifndef GPLOT_PARAMS_H
#define GPLOT_PARAMS_H

#include <alglib/ap.h>

typedef unsigned int ui_t;
typedef unsigned short us_t;

/**
 * @brief Gnuplot wrapper params
 *
 * @tparam T
 */
template <typename T>
struct gplot_params_s
{
    std::string infilename, delimiter, filename, title, legend, xlabel, ylabel, header;
    ui_t width, height;
    T lxrange, hxrange, lyrange, hyrange;
    alglib::real_2d_array mat;
    std::vector<std::tuple<T, T, T>> serie_xyc;
    std::vector<std::tuple<T, T, T, T, T>> serie_ooxyc;
};

#endif