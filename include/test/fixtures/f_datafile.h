
#ifndef TEST_DATAFILE_H
#define TEST_DATAFILE_H

#include <boost/filesystem.hpp>
#include <f_datasets.h>
#include <f_common.h>
#include <datafile.h>

#define DATA_FILENAME_LOAD_SPECIES_CSV "./test/datasets/species.csv"
#define DATA_FILENAME_SAVE_CSV "./build/datafiletest.csv"

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
    Data::File::Csv<T> *m_dataset;
};

template <typename T>
struct CsvFixt : public TestDatafileCsvFixt<T>
{
    Data::File::metas_t m_metas;
    explicit CsvFixt() : TestDatafileCsvFixt<T>()
    {
        BOOST_TEST_PASSPOINT();
    }
};

#endif