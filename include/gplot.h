
#ifndef GPLOT_H
#define GPLOT_H

#include <map>
#include <tuple>
#include <vector>
#include <cmath>
#include <boost/format.hpp>
#include <gnuplot-iostream.h>

#define PLOT "plot"
#define SET_TERMINAL_PNG "set terminal png"
#define SET_OUTPUT_FILENAME_FMT "set output '%1%'"
#define SET_XRANGE_FMT "set xrange [%1%:%2%]"
#define SET_YRANGE_FMT "set yrange [%1%:%2%]"
#define UNSET_COLORBOX "unset colorbox"
#define SET_PALETTE "set palette model RGB defined "
#define PALETTE_DEF "( 1 'black', 2 'blue', 3 'green', 4 'red' )"
#define SET_XAXIS_LABEL "set xlabel 'PC1'"
#define SET_YAXIS_LABEL "set ylabel 'PC2'"

template <typename T>
struct gplot_params_s
{
    std::string filename;
    std::string title;
    std::string legend;
    T lxrange, hxrange, lyrange, hyrange;
    std::vector<std::tuple<T, T, T>> xyc;
};

template <typename T>
class Gplot
{
public:
    explicit Gplot(gplot_params_s<T> params);
    ~Gplot();
    /**
     * @brief https://github.com/dstahlke/gnuplot-iostream/wiki/BasicUsage
     * 
     */
    void drawScatter(void);

private:
    Gnuplot gp;
    gplot_params_s<T> m_params;
};

#endif
