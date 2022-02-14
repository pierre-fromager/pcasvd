
#ifndef GPLOT_H
#define GPLOT_H

#include <map>
#include <tuple>
#include <vector>
#include <cmath>
#include <boost/format.hpp>
#include <gnuplot-iostream.h>
#include <alglib/ap.h>

#define SQ "'"
#define SMC ";"
#define COMA ","
#define _PLOT "plot "
#define _SET "set "
#define EOD "EOD"
#define DEFAULT_FONT "Verdana"
#define _FONT_ " font "
// transparent
#define SET_TERMINAL_PNG _SET "term pngcairo enhanced size %1%,%2% font 'Verdana,8'"
#define SET_OUTPUT_FILENAME_FMT _SET "output '%1%'"
#define SET_XRANGE_FMT _SET "xrange [%1%:%2%]"
#define SET_YRANGE_FMT _SET "yrange [%1%:%2%]"
#define UNSET_COLORBOX "unset colorbox"
#define SET_COLORBOX "set colorbox"
#define SET_PALETTE _SET "palette model RGB defined "
#define PALETTE_DEF "( 1 'black', 2 'blue', 3 'green', 4 'red' )"
#define SET_XLABEL _SET "xlabel "
#define SET_YLABEL _SET "ylabel "
#define USING_SCATTER "using 1:2:($3) notitle with points ps 0.5 pt 7 palette"
#define USING_CORCIRCLE "using 1:2:3:4:($5) notitle with vectors filled head lw 3 palette"
#define SET_CIRCLE_1 _SET "object 1 circle front at 0,0 size 1 fillcolor rgb 'black' lw 0.5"
#define SET_CIRCLE_2 _SET "object 2 circle front at 0,0 size 0.5 fillcolor rgb 'black' lw 0.5"
#define LEGEND1 ",'+' using (kx):(ky + $0*kdy):(word(labels, int($0+1))):0 "
#define LEGEND2 "with labels left offset 1,-0.1 point pt 7 palette t ''"
#define SET_LEGEND LEGEND1 LEGEND2
#define DATAFILE_SEPARATOR "datafile separator"

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
    std::string infilename, delimiter, filename, title, legend, xlabel, ylabel;
    ui_t width, height;
    T lxrange, hxrange, lyrange, hyrange;
    alglib::real_2d_array mat;
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
    explicit Gplot();
    ~Gplot();
    void setParams(gplot_params_s<T> params);
    void drawScatter(void);
    void drawCorCircle(void);
    void drawHeatmap(void);
    void drawBoxAndWiskers(void);

protected:
    Gnuplot gp;
    void resetSession();

private:
    gplot_params_s<T> m_params;
    void initPng(void);
    void setLegendParams(T kx, T ky, T kdy);
    void setDefaultFontsSizes(const us_t &title,
                              const us_t &xlabel,
                              const us_t &ylabel,
                              const us_t &tics);
    void setDefaultPalette(void);
    void setAxisLabels(void);
    void setRanges(void);
    void setTitle(void);
    void splitStr(
        std::string str,
        const std::string &delim,
        std::vector<std::string> &vecstr);
};

#endif
