
#ifndef FIXTURE_H
#define FIXTURE_H

#include <string>
#include <vector>
#include <options.h>

typedef unsigned int ui_t;

template <typename T>
struct fixtv_s
{
    ui_t nbcol;
    ui_t nbrow;
    std::string filename;
    std::string header;
    std::string delimiter;
    std::vector<T> values;
    cmd_options_t cmdopts;
};

#endif
