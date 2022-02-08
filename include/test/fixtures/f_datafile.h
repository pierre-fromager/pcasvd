
#ifndef TEST_DATAFILE_H
#define TEST_DATAFILE_H

#include "datafile.h"
#include "f_common.h"

#define DATA_FILENAME_LOAD_SPECIES_CSV "./test/datasets/species.csv"

typedef unsigned int ui_t;

template <typename T>
struct TestDatafileCsvFixt : public TestBaseFixt
{
    explicit TestDatafileCsvFixt()
    {
        BOOST_TEST_MESSAGE(m_setup);
        m_dataset = new Data::File::Csv<double>();
    }
    ~TestDatafileCsvFixt()
    {
        BOOST_TEST_MESSAGE(m_teardown);
        delete (m_dataset);
    }

    std::vector<T> m_buffer;
    Data::File::metas_t m_metas;
    Data::File::Csv<T> *m_dataset;
};

template <typename T>
struct SpeciesCsvFixt : public TestDatafileCsvFixt<T>
{
    const double tol = 0.00001;
    Data::File::metas_t tmpmet;
    explicit SpeciesCsvFixt() : TestDatafileCsvFixt<T>()
    {
        BOOST_TEST_PASSPOINT();
    }
};

#endif