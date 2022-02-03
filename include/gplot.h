
#ifndef GPLOT_H
#define GPLOT_H

#include <map>
#include <tuple>
#include <vector>
#include <cmath>
#include <boost/format.hpp>
#include <gnuplot-iostream.h>

#define SQ "'"
#define SMC ";"
#define COMA ","
#define _PLOT "plot"
#define _SET "set "
#define DEFAULT_FONT "Verdana"
#define SET_TERMINAL_PNG _SET "term pngcairo size %1%,%2% font 'Verdana,8'"
#define SET_OUTPUT_FILENAME_FMT _SET "output '%1%'"
#define SET_XRANGE_FMT _SET "xrange [%1%:%2%]"
#define SET_YRANGE_FMT _SET "yrange [%1%:%2%]"
#define UNSET_COLORBOX "unset colorbox"
#define SET_PALETTE _SET "palette model RGB defined "
#define PALETTE_DEF "( 1 'black', 2 'blue', 3 'green', 4 'red' )"
#define SET_XLABEL _SET "xlabel "
#define SET_YLABEL _SET "ylabel "
#define USING_SCATTER "using 1:2:($3) notitle with points ps 0.5 pt 7 palette"
#define USING_CORCIRCLE "using 1:2:3:4:($5) notitle with vectors filled head lw 3 palette"
#define SET_CIRCLE "set object 1 circle front at 0,0 size 1 fillcolor rgb 'black' lw 0.5"
#define LEGEND1 ",'+' using (kx):(ky + $0*kdy):(word(labels, int($0+1))):0 "
#define LEGEND2 "with labels left offset 1,-0.1 point pt 7 palette t ''"
#define SET_LEGEND LEGEND1 LEGEND2

/**
 * @brief Gnuplot wrapper params
 * 
 * @tparam T 
 */
template <typename T>
struct gplot_params_s
{
    std::string filename, title, legend, xlabel, ylabel;
    unsigned int width, height;
    T lxrange, hxrange, lyrange, hyrange;
    std::vector<std::tuple<T, T, T>> serie_xyc;
    std::vector<std::tuple<T, T, T, T, T>> serie_ooxyc;
};

/**
 * @brief Gnuplot wrapper
 * @url https://github.com/dstahlke/gnuplot-iostream/wiki/BasicUsage
 * 
 * @tparam T 
 */
template <typename T>
class Gplot
{
public:
    explicit Gplot(gplot_params_s<T> params);
    ~Gplot();
    void drawScatter(void);
    void drawCorCircle(void);

private:
    Gnuplot gp;
    gplot_params_s<T> m_params;
    void initPng(void);
    void setDefaultFontsSizes(void);
    void setDefaultPalette(void);
    void setAxisLabels(void);
    void setRanges(void);
    void setTitle(void);
};

#endif
