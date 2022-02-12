
#ifndef TEST_GPLOT_H
#define TEST_GPLOT_H

#include <boost/filesystem.hpp>
#include <f_datasets.h>
#include <f_common.h>
#include <gplot.h>

#define GPLOT_FILENAME_SCATTER_PNG "./build/pca_scatter.png"
#define GPLOT_FILENAME_CORCIRCLE_PNG "./build/pca_corcircle.png"
#define GPLOT_FILENAME_HEATMAP_PNG "./build/pca_heatmap.png"

typedef unsigned int ui_t;

template <typename T>
struct TestGplotFixt : public TestBaseFixt
{
    explicit TestGplotFixt()
    {
        BOOST_TEST_MESSAGE(m_setup);
        m_gplot = new Gplot<double>();
    }
    ~TestGplotFixt()
    {
        BOOST_TEST_MESSAGE(m_teardown);
        delete (m_gplot);
    }
    Gplot<T> *m_gplot;
};

template <typename T>
struct GplotParamsFixt : public TestGplotFixt<T>
{
    struct gplot_params_s<T> gparams;
    explicit GplotParamsFixt() : TestGplotFixt<T>()
    {
        BOOST_TEST_PASSPOINT();
    }
};

#endif