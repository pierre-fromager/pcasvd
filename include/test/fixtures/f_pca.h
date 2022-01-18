
#ifndef TEST_PCA_FIXTURE_H
#define TEST_PCA_FIXTURE_H

#include "pca.h"
#include "f_common.h"

struct TestPcaFixt : TestBaseFixt
{
    TestPcaFixt(matrix_t matrix) : m_matrix(matrix)
    {
        BOOST_TEST_MESSAGE(m_setup);
        pca = new Pca(m_matrix);
    }
    ~TestPcaFixt()
    {
        BOOST_TEST_MESSAGE(m_teardown);
        delete pca;
    }
    Pca *pca;
    matrix_t m_matrix;
};

struct Pca2DFixt : public TestPcaFixt
{
    Pca2DFixt() : TestPcaFixt({{5, -5},
                               {-1, 1}}) {}
};

struct Pca3DFixt : public TestPcaFixt
{
    Pca3DFixt() : TestPcaFixt({{5, -5, 1},
                               {-1, 1, 1},
                               {0, 1, 2}}) {}
};

struct Pca4DFixt : public TestPcaFixt
{
    Pca4DFixt() : TestPcaFixt({{5, -5, 1, 0},
                               {0, -1, 1, 1},
                               {1, 0, 1, 2},
                               {1, 2, 3, 4}}) {}
};

#endif