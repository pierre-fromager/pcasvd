
#ifndef TEST_PCA_FIXTURE_H
#define TEST_PCA_FIXTURE_H

#include "f_common.h"
#include "f_datasets.h"
#include "pca.h"

template <typename T, ui_t NC, ui_t NR>
struct TestPcaFixt : public TestBaseFixt
{
    explicit TestPcaFixt(std::vector<T> v) : m_v(v)
    {
        BOOST_TEST_MESSAGE(m_setup);
        m_m.setcontent(NR, NC, m_v.data());
        pca = new Pca<T>(m_m);
        try
        {
            pca->process();
            res = pca->results();
        }
        catch (alglib::ap_error e)
        {
            BOOST_TEST_MESSAGE("Error Pca Process");
        }
    }
    ~TestPcaFixt()
    {
        BOOST_TEST_MESSAGE(m_teardown);
        m_v.clear();
        delete pca;
    }
    std::vector<T> m_v;
    alglib::real_2d_array m_m;
    Pca<T> *pca;
    pca_result_s<T> res;
};

template <typename T>
struct Pca2x12Fixt : public TestPcaFixt<T, 2, 12>
{
    const double tol = 0.00001;
    ui_t i, j;
    Pca2x12Fixt() : TestPcaFixt<T, 2, 12>(DS2X12)
    {
        BOOST_TEST_PASSPOINT();
    }
};

#endif