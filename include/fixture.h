
#ifndef FIXTURE_H
#define FIXTURE_H

#include <vector>

typedef unsigned int ui_t;

template <typename T>
struct fixtv_s
{
    ui_t nbcol;
    ui_t nbrow;
    std::vector<T> values;
};

#endif
