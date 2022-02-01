
#ifndef FIXTURE_H
#define FIXTURE_H

#include <array>
#include <vector>

typedef unsigned int ui_t;

template <typename T, ui_t NC, ui_t NR>
struct fixt_s
{
    ui_t nbcol = NC;
    ui_t nbrow = NR;
    std::array<T, NC * NR> values;
};

#endif
