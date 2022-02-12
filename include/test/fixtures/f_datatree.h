
#ifndef TEST_DATATREE_H
#define TEST_DATATREE_H

#include <boost/filesystem.hpp>
#include <f_datasets.h>
#include <f_common.h>
#include <datatree.h>

#define DTREE_FILE_DEF_JSON "./test/datasets/treetest.json"
#define DTREE_FILE_PCAR_JSON "./test/datasets/pcaresult.json"

typedef unsigned int ui_t;
namespace pt = boost::property_tree;

template <typename T>
struct TestDatatreeFixt : public TestBaseFixt
{
    explicit TestDatatreeFixt(const std::string filename = "")
    {
        BOOST_TEST_MESSAGE(m_setup);
        m_datatree = new Data::File::Tree<T>();
        if (filename.length() > 0)
            m_datatree->load(filename);
    }
    ~TestDatatreeFixt()
    {
        BOOST_TEST_MESSAGE(m_teardown);
        delete (m_datatree);
    }
    Data::File::Tree<T> *m_datatree;
};

template <typename T>
struct DataTreeDefaultFixt : public TestDatatreeFixt<T>
{
    ui_t err;
    explicit DataTreeDefaultFixt() : TestDatatreeFixt<T>(DTREE_FILE_DEF_JSON)
    {
        BOOST_TEST_PASSPOINT();
    }
};

template <typename T>
struct DataTreePcaFixt : public TestDatatreeFixt<T>
{
    T exp1 = 1;
    T exp2 = 2;
    T exp3 = 3;
    T exp4 = 4;
    T exp5 = 5;
    T exp6 = 6;
    ui_t err;
    explicit DataTreePcaFixt() : TestDatatreeFixt<T>(DTREE_FILE_PCAR_JSON)
    {
        BOOST_TEST_PASSPOINT();
    }
};

#endif