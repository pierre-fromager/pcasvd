
#ifndef DATFTREE_H
#define DATATREE_H

#include <string>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <type_traits>
#include <alglib/ap.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <regex>

#define JSON_NUMERIC_OUT_FMT "\\\"([-+]?[0-9]+\\.{0,1}[0-9]*)\\\""
#define _COLS "cols"
#define _ROWS "rows"

typedef unsigned int ui_t;

namespace Data
{

namespace File
{

namespace pt = boost::property_tree;

template <typename T>
class Tree
{
public:
    explicit Tree();
    ~Tree();
    void reset(void);
    pt::ptree getRoot(void);
    ui_t rows();
    ui_t cols();
    bool hasChild(const std::string &entry);
    void load(const std::string &filename);
    void save(const std::string &filename);
    void save(std::ostream &stream);
    ui_t vector(const std::string &entry, std::vector<double> &dstv);
    ui_t vector(const std::string &entry, std::vector<std::string> &dstv);
    ui_t vector(const std::string &entry, alglib::real_1d_array &dstv);
    ui_t matrix(const std::string &entry, ui_t &cols, ui_t &rows, std::vector<T> &t);
    ui_t matrix(const std::string &entry, alglib::real_2d_array &m);
    ui_t value(const std::string &entry, std::string &s);
    ui_t value(const std::string &entry, double &d);
    ui_t addValue(const std::string &entry, const std::string &s);
    ui_t addValue(const std::string &entry, const double &d);
    ui_t addVector(const std::string &entry, std::vector<std::string> vs);
    ui_t addVector(const std::string &entry, std::vector<double> vd);
    ui_t addVector(const std::string &entry, alglib::real_1d_array ad);
    ui_t addMatrix(const std::string &entry, ui_t cols, ui_t rows, std::vector<double> vd);
    ui_t addMatrix(const std::string &entry, alglib::real_2d_array m);

private:
    ui_t m_cols, m_rows, err;
    pt::ptree m_root;
};

} // namespace File

} // namespace Data

#endif
