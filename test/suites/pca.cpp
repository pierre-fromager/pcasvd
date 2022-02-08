
#include <boost/test/unit_test.hpp>
#include "test/fixtures/f_pca.h"

BOOST_AUTO_TEST_SUITE(TestSuitePca)

namespace tt = boost::test_tools;

BOOST_FIXTURE_TEST_CASE(PcaDatasource_2x12, Pca2x12Fixt<double>)
{
    const ui_t cols = m_m.cols();
    const ui_t rows = m_m.rows();
    BOOST_CHECK_EQUAL(cols, 2);
    BOOST_CHECK_EQUAL(rows, 12);
    BOOST_CHECK_EQUAL(m_m[0][0], 33.0);
    BOOST_CHECK_EQUAL(m_m[0][1], 80.0);
    BOOST_CHECK_EQUAL(m_m[11][0], 48.0);
    BOOST_CHECK_EQUAL(m_m[11][1], 64.0);
}

BOOST_FIXTURE_TEST_CASE(PcaEigenValues_2x12, Pca2x12Fixt<double>)
{
    const alglib::real_1d_array expected_eigen_values = "[171.603721 , 105.149233]";
    const ui_t cols = m_m.cols();
    const ui_t res_eig_values_len = res.eig_values.length();
    BOOST_CHECK_EQUAL(res_eig_values_len, 2);
    for (i = 0; i < res_eig_values_len; i++)
        BOOST_TEST(
            expected_eigen_values[i] == res.eig_values[i],
            tt::tolerance(tol));
}

BOOST_FIXTURE_TEST_CASE(PcaExplainedVariance_2x12, Pca2x12Fixt<double>)
{
    const alglib::real_1d_array expected_exp_var = "[0.620061 , 0.379939]";
    const ui_t cols = m_m.cols();
    const ui_t res_exp_variance_len = res.exp_variance.length();
    BOOST_CHECK_EQUAL(res_exp_variance_len, cols);
    for (i = 0; i < res_exp_variance_len; i++)
        BOOST_TEST(
            expected_exp_var[i] == res.exp_variance[i],
            tt::tolerance(tol));
}

BOOST_FIXTURE_TEST_CASE(PcaEigenVectors_2x12, Pca2x12Fixt<double>)
{
    const alglib::real_2d_array expected_eigen_vectors = "[\
    [-0.154098 , -0.988056],\
    [-0.988056 , 0.154098]\
    ]";
    const ui_t cols = m_m.cols();
    const ui_t rows = m_m.rows();
    BOOST_CHECK_EQUAL(res.eig_vectors.cols(), cols);
    BOOST_CHECK_EQUAL(res.eig_vectors.rows(), cols);
    for (j = 0; j < cols; j++)
        for (i = 0; i < cols; i++)
            BOOST_TEST(
                expected_eigen_vectors[j][i] == res.eig_vectors[j][i],
                tt::tolerance(tol));
}

BOOST_FIXTURE_TEST_CASE(PcaCorrelation_2x12, Pca2x12Fixt<double>)
{
    const alglib::real_2d_array expected_correlation = "[\
    [1 , 0.075112],\
    [0.075112 , 1]\
    ]";
    const ui_t cols = m_m.cols();
    const ui_t rows = m_m.rows();
    BOOST_CHECK_EQUAL(res.cor.cols(), cols);
    BOOST_CHECK_EQUAL(res.cor.rows(), cols);
    for (j = 0; j < cols; j++)
        for (i = 0; i < cols; i++)
            BOOST_TEST(
                expected_correlation[j][i] == res.cor[j][i],
                tt::tolerance(tol));
}

BOOST_FIXTURE_TEST_CASE(PcaCovariance_2x12, Pca2x12Fixt<double>)
{
    const alglib::real_2d_array expected_covariance = "[\
    [106.727273 , 10.118182],\
    [10.118182 , 170.025682]\
    ]";
    const ui_t cols = m_m.cols();
    const ui_t rows = m_m.rows();
    BOOST_CHECK_EQUAL(res.cov.cols(), cols);
    BOOST_CHECK_EQUAL(res.cov.rows(), cols);
    for (j = 0; j < cols; j++)
        for (i = 0; i < cols; i++)
            BOOST_TEST(
                expected_covariance[j][i] == res.cov[j][i],
                tt::tolerance(tol));
}

BOOST_FIXTURE_TEST_CASE(PcaProjection_2x12, Pca2x12Fixt<double>)
{
    const alglib::real_2d_array expected_proj = "[\
    [-84.129678 , -20.278000],\
	[-86.599817 , -19.892755],\
	[-104.835332 , -18.060819],\
	[-95.397115 , -27.629521],\
	[-70.668564 , -18.329051]\
    ]";
    const ui_t cols = m_m.cols();
    const ui_t rows = m_m.rows();
    BOOST_CHECK_EQUAL(res.proj.cols(), cols);
    BOOST_CHECK_EQUAL(res.proj.rows(), rows);
    for (j = 0; j < 5; j++)
        for (i = 0; i < cols; i++)
            BOOST_TEST(
                expected_proj[j][i] == res.proj[j][i],
                tt::tolerance(tol));
}

BOOST_AUTO_TEST_SUITE_END() // TestSuitePca