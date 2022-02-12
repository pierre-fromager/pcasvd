
#include <boost/test/unit_test.hpp>
#include "test/fixtures/f_gplot.h"

BOOST_AUTO_TEST_SUITE(TestSuiteGplot)

namespace tt = boost::test_tools;

BOOST_FIXTURE_TEST_CASE(GplotDrawScatter, GplotParamsFixt<double>)
{
    gparams.filename = GPLOT_FILENAME_SCATTER_PNG;
    gparams.width = 1024;
    gparams.height = 768;
    gparams.title = "Scatter plot (Individual factor map) : Test";
    gparams.xlabel = "PC1 Petal.W(X%)";
    gparams.ylabel = "PC2 Petal.L(Y%)";
    gparams.legend = "Setosa Versicolor Virginica";
    std::vector<T> col1, col2;
    getCol<T>(result.proj, 0, col1);
    const T margin = 0.5;
    gparams.lxrange = (*min_element(col1.begin(), col1.end())) - margin;
    gparams.hxrange = (*max_element(col1.begin(), col1.end())) + margin;
    getCol<T>(result.proj, 1, col2);
    gparams.lyrange = (*min_element(col2.begin(), col2.end())) - margin;
    gparams.hyrange = (*max_element(col2.begin(), col2.end())) + margin;
    T category;
    ui_t coSize = col1.size();
    for (ui_t c = 0; c < coSize; c++)
    {
        if (c < 52)
            category = 0;
        else if (c < 102)
            category = 1;
        else
            category = 2;
        gparams.serie_xyc.emplace_back(col1[c], col2[c], category);
    }
    col1.clear();
    col2.clear();
    //Gplot<T> *gpl = new Gplot<T>();
    m_gplot->setParams(gparams);
    m_gplot->drawScatter();
    gparams.serie_xyc.clear();
}

BOOST_FIXTURE_TEST_CASE(GplotDrawCorcircle, GplotParamsFixt<double>)
{
}

BOOST_FIXTURE_TEST_CASE(GplotDrawHeatmap, GplotParamsFixt<double>)
{
}

BOOST_AUTO_TEST_SUITE_END() // TestSuiteDatafile