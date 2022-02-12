
#include <boost/test/unit_test.hpp>
#include "test/fixtures/f_datatree.h"

BOOST_AUTO_TEST_SUITE(TestSuiteDatatree)

namespace tt = boost::test_tools;

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoad,
    DataTreeDefaultFixt<std::string>)
{
    const std::string &key = "fruits";
    const std::vector<std::string> &expected = {"apple", "raspberry", "orange"};
    std::vector<std::string> fruits;
    for (pt::ptree::value_type &v : m_datatree->getRoot().get_child(key))
        fruits.push_back(v.second.data());
    ui_t c;
    for (c = 0; c < fruits.size(); c++)
        BOOST_CHECK_EQUAL(fruits[c], expected[c]);
    fruits.clear();
    err = m_datatree->vector(key, fruits);
    BOOST_CHECK_EQUAL(err, 0);
    for (c = 0; c < fruits.size(); c++)
        BOOST_CHECK_EQUAL(fruits[c], expected[c]);
    fruits.clear();
    alglib::real_1d_array a1d;
    err = m_datatree->vector(key, a1d);
    BOOST_REQUIRE(err == 1);

    alglib::real_2d_array a2d;
    err = m_datatree->matrix("matrix", a2d);
    BOOST_REQUIRE(err == 1);
    //BOOST_CHECK_EQUAL(a2d.cols(), 3);
    //BOOST_CHECK_EQUAL(a2d.rows(), 3);

    fruits.clear();
    const std::string &keyvadd = "fruitsstrvector";
    BOOST_REQUIRE(m_datatree->hasChild(keyvadd) == false);
    err = m_datatree->addVector(keyvadd, expected);
    BOOST_REQUIRE(err == 0);
    BOOST_REQUIRE(m_datatree->hasChild(keyvadd) == true);
    err = m_datatree->vector(keyvadd, fruits);
    BOOST_CHECK_EQUAL(err, 0);
    for (c = 0; c < fruits.size(); c++)
        BOOST_CHECK_EQUAL(fruits[c], expected[c]);

    const std::string &keyversion = "version";
    BOOST_REQUIRE(m_datatree->hasChild(keyversion) == false);
    const std::string &expver = "1.0.0";
    err = m_datatree->addValue(keyversion, expver);
    BOOST_REQUIRE(err == 0);
    std::string verstr;
    err = m_datatree->value(keyversion, verstr);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(verstr, expver);

    const std::string &keypi = "pi";
    BOOST_REQUIRE(m_datatree->hasChild(keypi) == false);
    const double &expi = 3.1416;
    err = m_datatree->addValue(keypi, expi);
    BOOST_REQUIRE(err == 0);
    double pidbl;
    err = m_datatree->value(keypi, pidbl);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(pidbl, expi);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultBadKeyVectors,
    DataTreePcaFixt<double>)
{
    const std::string &key = "bad_key";
    std::vector<double> vd;
    err = m_datatree->vector(key, vd);
    BOOST_REQUIRE(err == 1);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultColsRows,
    DataTreePcaFixt<double>)
{
    const double expcols = 2;
    const double exprows = 3;
    BOOST_CHECK_EQUAL(m_datatree->cols(), expcols);
    BOOST_CHECK_EQUAL(m_datatree->rows(), exprows);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultVectorEigenValues,
    DataTreePcaFixt<double>)
{
    std::vector<double> vd;
    alglib::real_1d_array ad;
    const double &expsize = 2;
    const std::string &key = "eig_values";

    err = m_datatree->vector(key, vd);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(vd.size(), expsize);
    BOOST_CHECK_EQUAL(vd[0], exp1);
    BOOST_CHECK_EQUAL(vd[1], exp2);
    vd.clear();

    err = m_datatree->vector(key, ad);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(ad.length(), expsize);
    BOOST_CHECK_EQUAL(ad[0], exp1);
    BOOST_CHECK_EQUAL(ad[1], exp2);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultVectorExplainedVariance,
    DataTreePcaFixt<double>)
{
    std::vector<double> vd;
    alglib::real_1d_array ad;
    const double &expsize = 2;
    const std::string &key = "exp_variance";

    err = m_datatree->vector(key, vd);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(vd.size(), expsize);
    BOOST_CHECK_EQUAL(vd[0], exp1);
    BOOST_CHECK_EQUAL(vd[1], exp2);
    vd.clear();

    err = m_datatree->vector(key, ad);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(ad.length(), expsize);
    BOOST_CHECK_EQUAL(ad[0], exp1);
    BOOST_CHECK_EQUAL(ad[1], exp2);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultMatrixEigenVectors,
    DataTreePcaFixt<double>)
{

    const double &expsize = 4;
    const std::string &key = "eig_vectors";

    std::vector<double> vd;
    ui_t cols, rows;
    err = m_datatree->matrix(key, cols, rows, vd);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(vd.size(), expsize);
    BOOST_CHECK_EQUAL(vd[0], exp1);
    BOOST_CHECK_EQUAL(vd[1], exp2);
    BOOST_CHECK_EQUAL(vd[2], exp3);
    BOOST_CHECK_EQUAL(vd[3], exp4);
    vd.clear();

    const ui_t &nbcol = 2;
    const ui_t &nbrow = 2;
    alglib::real_2d_array ad;
    err = m_datatree->matrix(key, ad);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(ad.cols(), nbcol);
    BOOST_CHECK_EQUAL(ad.rows(), nbrow);
    BOOST_CHECK_EQUAL(ad[0][0], exp1);
    BOOST_CHECK_EQUAL(ad[0][1], exp2);
    BOOST_CHECK_EQUAL(ad[1][0], exp3);
    BOOST_CHECK_EQUAL(ad[1][1], exp4);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultMatrixCorrelation,
    DataTreePcaFixt<double>)
{
    const std::string &key = "cor";
    const ui_t &nbcol = m_datatree->cols();
    BOOST_REQUIRE(nbcol == 2);
    const double &expsize = nbcol * nbcol;

    std::vector<double> vd;
    ui_t cols, rows;
    err = m_datatree->matrix(key, cols, rows, vd);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(vd.size(), expsize);
    BOOST_CHECK_EQUAL(vd[0], exp1);
    BOOST_CHECK_EQUAL(vd[1], exp2);
    BOOST_CHECK_EQUAL(vd[2], exp3);
    BOOST_CHECK_EQUAL(vd[3], exp4);
    vd.clear();

    alglib::real_2d_array ad;
    err = m_datatree->matrix(key, ad);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(ad.cols(), nbcol);
    BOOST_CHECK_EQUAL(ad.rows(), nbcol);
    BOOST_CHECK_EQUAL(ad[0][0], exp1);
    BOOST_CHECK_EQUAL(ad[0][1], exp2);
    BOOST_CHECK_EQUAL(ad[1][0], exp3);
    BOOST_CHECK_EQUAL(ad[1][1], exp4);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultMatrixCovariance,
    DataTreePcaFixt<double>)
{
    const std::string &key = "cov";
    const ui_t &nbcol = m_datatree->cols();
    BOOST_REQUIRE(nbcol == 2);
    const double &expsize = nbcol * nbcol;

    std::vector<double> vd;
    ui_t cols, rows;
    err = m_datatree->matrix(key, cols, rows, vd);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(vd.size(), expsize);
    BOOST_CHECK_EQUAL(vd[0], exp1);
    BOOST_CHECK_EQUAL(vd[1], exp2);
    BOOST_CHECK_EQUAL(vd[2], exp3);
    BOOST_CHECK_EQUAL(vd[3], exp4);
    vd.clear();

    alglib::real_2d_array ad;
    err = m_datatree->matrix(key, ad);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(ad.cols(), nbcol);
    BOOST_CHECK_EQUAL(ad.rows(), nbcol);
    BOOST_CHECK_EQUAL(ad[0][0], exp1);
    BOOST_CHECK_EQUAL(ad[0][1], exp2);
    BOOST_CHECK_EQUAL(ad[1][0], exp3);
    BOOST_CHECK_EQUAL(ad[1][1], exp4);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultMatrixProjection,
    DataTreePcaFixt<double>)
{
    const std::string &key = "proj";
    const ui_t &nbcol = m_datatree->cols();
    BOOST_REQUIRE(nbcol == 2);
    const ui_t &nbrow = m_datatree->rows();
    BOOST_REQUIRE(nbrow == 3);
    const double &expsize = nbcol * nbrow;

    std::vector<double> vd;
    ui_t cols, rows;
    err = m_datatree->matrix(key, cols, rows, vd);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(vd.size(), expsize);
    BOOST_CHECK_EQUAL(vd[0], exp1);
    BOOST_CHECK_EQUAL(vd[1], exp2);
    BOOST_CHECK_EQUAL(vd[2], exp3);
    BOOST_CHECK_EQUAL(vd[3], exp4);
    BOOST_CHECK_EQUAL(vd[4], exp5);
    BOOST_CHECK_EQUAL(vd[5], exp6);
    vd.clear();

    alglib::real_2d_array ad;
    err = m_datatree->matrix(key, ad);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(ad.cols(), nbcol);
    BOOST_CHECK_EQUAL(ad.rows(), nbrow);
    BOOST_CHECK_EQUAL(ad[0][0], exp1);
    BOOST_CHECK_EQUAL(ad[0][1], exp2);
    BOOST_CHECK_EQUAL(ad[1][0], exp3);
    BOOST_CHECK_EQUAL(ad[1][1], exp4);
    BOOST_CHECK_EQUAL(ad[2][0], exp5);
    BOOST_CHECK_EQUAL(ad[2][1], exp6);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultAddStrValue,
    DataTreePcaFixt<double>)
{
    const std::string &keyversion = "version";
    BOOST_REQUIRE(m_datatree->hasChild(keyversion) == false);
    const std::string &expver = "1.0.0";
    err = m_datatree->addValue(keyversion, expver);
    BOOST_REQUIRE(err == 0);
    std::string verstr;
    err = m_datatree->value(keyversion, verstr);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(verstr, expver);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultAddDoubleValue,
    DataTreePcaFixt<double>)
{
    const std::string &keypi = "pi";
    BOOST_REQUIRE(m_datatree->hasChild(keypi) == false);
    const double &expi = 3.1416;
    err = m_datatree->addValue(keypi, expi);
    BOOST_REQUIRE(err == 0);
    double pidbl;
    err = m_datatree->value(keypi, pidbl);
    BOOST_REQUIRE(err == 0);
    BOOST_CHECK_EQUAL(pidbl, expi);
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultAddStrVector,
    DataTreePcaFixt<double>)
{
    const std::vector<std::string> &expected = {"I", "push", "some", "datas"};
    const std::string &keyvadd = "vectorstr";
    ui_t c;
    BOOST_REQUIRE(m_datatree->hasChild(keyvadd) == false);
    err = m_datatree->addVector(keyvadd, expected);
    BOOST_REQUIRE(err == 0);
    BOOST_REQUIRE(m_datatree->hasChild(keyvadd) == true);
    std::vector<std::string> vs;
    err = m_datatree->vector(keyvadd, vs);
    BOOST_CHECK_EQUAL(err, 0);
    for (c = 0; c < vs.size(); c++)
        BOOST_CHECK_EQUAL(vs[c], expected[c]);
    vs.clear();
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultAddDoubleVector,
    DataTreePcaFixt<double>)
{
    const std::vector<double> &expected = {0.1, 0.2, 0.3, 0.4};
    const std::string &keyvadd = "vectordouble";
    ui_t c;
    BOOST_REQUIRE(m_datatree->hasChild(keyvadd) == false);
    err = m_datatree->addVector(keyvadd, expected);
    BOOST_REQUIRE(err == 0);
    BOOST_REQUIRE(m_datatree->hasChild(keyvadd) == true);
    std::vector<double> vd;
    err = m_datatree->vector(keyvadd, vd);
    BOOST_CHECK_EQUAL(err, 0);
    for (c = 0; c < vd.size(); c++)
        BOOST_CHECK_EQUAL(vd[c], expected[c]);
    vd.clear();
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultAddAlglib1dArray,
    DataTreePcaFixt<double>)
{
    const std::vector<double> &expected = {0.1, 0.2, 0.3, 0.4};
    const std::string &keyvadd = "ar1d";
    ui_t c;
    alglib::real_1d_array ad;
    std::vector<double> vd;
    ad.setcontent(expected.size(), expected.data());
    BOOST_REQUIRE(m_datatree->hasChild(keyvadd) == false);
    err = m_datatree->addVector(keyvadd, ad);
    BOOST_REQUIRE(err == 0);
    err = m_datatree->vector(keyvadd, vd);
    BOOST_REQUIRE(err == 0);
    for (c = 0; c < vd.size(); c++)
        BOOST_CHECK_EQUAL(vd[c], expected[c]);
    vd.clear();
}

BOOST_FIXTURE_TEST_CASE(
    DatatreeLoadPcaResultAddAlglibMatrix,
    DataTreePcaFixt<double>)
{
    const std::vector<std::vector<double>> &expected = {
        {-84.129678, -20.278000},
        {-86.599817, -19.892755},
        {-104.835332, -18.060819},
        {-95.397115, -27.629521},
        {-70.668564, -18.329051}};
    const std::string &keyvadd = "am";
    const alglib::real_2d_array m = "[\
    [-84.129678 , -20.278000],\
	[-86.599817 , -19.892755],\
	[-104.835332 , -18.060819],\
	[-95.397115 , -27.629521],\
	[-70.668564 , -18.329051]\
    ]";
    ui_t i, j;
    for (j = 0; j < m.rows(); j++)
        for (i = 0; i < m.cols(); i++)
            BOOST_CHECK_EQUAL(m(j, i), expected[j][i]);
    err = m_datatree->addMatrix(keyvadd, m);
    BOOST_REQUIRE(err == 0);
    alglib::real_2d_array md;
    err = m_datatree->matrix(keyvadd, md);
    BOOST_REQUIRE(err == 0);
    for (j = 0; j < md.rows(); j++)
        for (i = 0; i < md.cols(); i++)
            BOOST_CHECK_EQUAL(md(j, i), expected[j][i]);
}

BOOST_AUTO_TEST_SUITE_END() // TestSuiteDatafile