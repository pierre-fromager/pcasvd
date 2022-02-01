
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <iostream>
#include <iomanip>
#include <colors.h>
#include <colormap.h>
#include <alglib/ap.h>

#define TAB "\t"
#define COMA ","
#define SEMICOLON ";"
#define SPACE " "
#define SVOID ""
#define TFOLLOW "..."
#define DISP_PAD_LEFT 10

typedef unsigned int ui_t;

using namespace std;

class Display
{
public:
    explicit Display(colormap_t colors);
    ~Display();
    void mat(string title, alglib::real_2d_array v, ui_t nbrow, ui_t nbcol, ui_t maxrow = 0);
    void vec(string title, alglib::real_1d_array v, ui_t nbcol);
    void title(string title);
    void subtitle(string stitle);
    void error(string title);

private:
    void print(
        std::string color,
        string title,
        bool reset = true,
        bool endl = true);
    colormap_t m_colors;
    void printvalue(double v);
};

#endif
