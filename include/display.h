
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <colors.h>
#include "alglib/ap.h"

#define TAB "\t"
#define COMA ","
#define SEMICOLON ";"
#define SPACE " "

typedef unsigned int ui_t;

class Display
{
public:
    Display(colormap_t colors);
    ~Display();
    void mat(std::string title, alglib::real_2d_array v, ui_t nbrow, ui_t nbcol);
    void vec(std::string title, alglib::real_1d_array v, ui_t nbcol);
    void title(std::string title);
    void error(std::string title);

private:
    colormap_t m_colors;
};

#endif
