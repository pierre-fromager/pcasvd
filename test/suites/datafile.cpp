
#include <boost/test/unit_test.hpp>
#include "test/fixtures/f_datafile.h"

BOOST_AUTO_TEST_SUITE(TestSuiteDatafile)

namespace tt = boost::test_tools;

BOOST_FIXTURE_TEST_CASE(DatafileSpeciesLoad, CsvFixt<double>)
{
    // Check metas from new dataset instance
    BOOST_CHECK_EQUAL(m_dataset->metas().cols, 0);
    BOOST_CHECK_EQUAL(m_dataset->metas().rows, 0);
    BOOST_CHECK_EQUAL(
        boost::filesystem::exists(DATA_FILENAME_LOAD_SPECIES_CSV),
        true);
    // Check metas from dataset after load
    m_metas.sep = COMA;
    m_metas.skip = 1;
    m_metas.filename = DATA_FILENAME_LOAD_SPECIES_CSV;
    m_dataset->setMetas(m_metas);
    m_dataset->load();
    BOOST_CHECK_EQUAL(m_dataset->metas().cols, 4);
    BOOST_CHECK_EQUAL(m_dataset->metas().rows, 150);
    // Check buffer from dataset
    BOOST_CHECK_EQUAL(
        m_dataset->buffer().size(),
        m_dataset->metas().cols * m_dataset->metas().rows);
    BOOST_CHECK_EQUAL(m_dataset->buffer().front(), 5.1);
    BOOST_CHECK_EQUAL(m_dataset->buffer().back(), 1.8);
}

BOOST_FIXTURE_TEST_CASE(DatafileSpeciesLoadSkip, CsvFixt<double>)
{
    // Check metas from new dataset instance
    BOOST_CHECK_EQUAL(m_dataset->metas().cols, 0);
    BOOST_CHECK_EQUAL(m_dataset->metas().rows, 0);
    BOOST_CHECK_EQUAL(
        boost::filesystem::exists(DATA_FILENAME_LOAD_SPECIES_CSV),
        true);
    // Check metas from dataset after load
    m_metas.sep = COMA;
    m_metas.skip = 71;
    m_metas.filename = DATA_FILENAME_LOAD_SPECIES_CSV;
    m_dataset->setMetas(m_metas);
    m_dataset->load();
    BOOST_CHECK_EQUAL(m_dataset->metas().cols, 4);
    BOOST_CHECK_EQUAL(m_dataset->metas().rows, 80);
    // Check buffer from dataset
    BOOST_CHECK_EQUAL(
        m_dataset->buffer().size(),
        m_dataset->metas().cols * m_dataset->metas().rows);
    BOOST_CHECK_EQUAL(m_dataset->buffer().front(), 5.9);
    BOOST_CHECK_EQUAL(m_dataset->buffer().back(), 1.8);
}

BOOST_FIXTURE_TEST_CASE(DatafileSave, CsvFixt<double>)
{
    boost::filesystem::remove(DATA_FILENAME_SAVE_CSV);
    BOOST_CHECK_EQUAL(
        boost::filesystem::exists(DATA_FILENAME_SAVE_CSV),
        false);
    // Check metas from new dataset instance
    BOOST_CHECK_EQUAL(m_dataset->metas().cols, 0);
    BOOST_CHECK_EQUAL(m_dataset->metas().rows, 0);
    // Set metas to dataset before save
    m_metas.sep = SEMICOLON;
    m_metas.skip = 0;
    m_metas.header = "C1;C2";
    m_metas.rows = 12;
    m_metas.cols = 2;
    m_metas.filename = DATA_FILENAME_SAVE_CSV;
    m_dataset->setMetas(m_metas);
    m_dataset->setBuffer(DS2X12);
    m_dataset->save();
    BOOST_CHECK_EQUAL(
        boost::filesystem::exists(DATA_FILENAME_SAVE_CSV),
        true);
    // Reset metas rows cols before load check
    m_metas.rows = 0;
    m_metas.cols = 0;
    m_dataset->reset();
    m_metas.skip = 1;
    m_dataset->setMetas(m_metas);
    m_dataset->load();
    BOOST_CHECK_EQUAL(m_dataset->metas().cols, 2);
    BOOST_CHECK_EQUAL(m_dataset->metas().rows, 12);
    BOOST_CHECK_EQUAL(
        m_dataset->buffer().size(),
        m_dataset->metas().cols * m_dataset->metas().rows);
    BOOST_CHECK_EQUAL(m_dataset->buffer().front(), 33.0);
    BOOST_CHECK_EQUAL(m_dataset->buffer().back(), 64.0);
    boost::filesystem::remove(DATA_FILENAME_SAVE_CSV);
}

BOOST_AUTO_TEST_SUITE_END() // TestSuiteDatafile