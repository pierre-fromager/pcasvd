
#include <boost/test/unit_test.hpp>
#include "test/fixtures/f_datafile.h"

BOOST_AUTO_TEST_SUITE(TestSuiteDatafile)

namespace tt = boost::test_tools;

BOOST_FIXTURE_TEST_CASE(DatafileSpeciesLoad, SpeciesCsvFixt<double>)
{
    // Check metas from new dataset instance
    tmpmet = m_dataset->metas();
    BOOST_CHECK_EQUAL(tmpmet.cols, 0);
    BOOST_CHECK_EQUAL(tmpmet.rows, 0);
    // Check metas from dataset after load
    m_metas.sep = COMA;
    m_metas.skip = 1;
    m_metas.filename = DATA_FILENAME_LOAD_SPECIES_CSV;
    m_dataset->setMetas(m_metas);
    m_dataset->load();
    tmpmet = m_dataset->metas();
    BOOST_CHECK_EQUAL(tmpmet.cols, 4);
    BOOST_CHECK_EQUAL(tmpmet.rows, 150);
    // Check buffer from dataset
    m_buffer = m_dataset->buffer();
    BOOST_CHECK_EQUAL(m_buffer.size(), tmpmet.cols * tmpmet.rows);
    BOOST_CHECK_EQUAL(m_buffer.front(), 5.1);
    BOOST_CHECK_EQUAL(m_buffer.back(), 1.8);
}

BOOST_AUTO_TEST_SUITE_END() // TestSuiteDatafile