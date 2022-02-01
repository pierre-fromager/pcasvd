
#ifndef DATA_H
#define DATA_H

#include <string>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <type_traits>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#define TAB "\t"
#define COMA ","
#define SEMICOLON ";"
#define SPACE " "

namespace Data
{

namespace File
{

typedef struct metas_s
{
    std::string filename;
    std::string sep;
    unsigned int cols;
    unsigned int rows;
} metas_t;

template <typename T>
class Csv
{
public:
    explicit Csv(std::string separator);
    ~Csv();
    void load(std::string filename, unsigned int skip = 0);
    void save(std::string filename);
    std::vector<T> buffer(void);
    metas_t metas(void);

private:
    std::string m_separator;
    metas_t m_metas;
    std::string m_filename;
    std::vector<std::string> m_lines;
    std::vector<T> m_buffer;
};

} // namespace File

} // namespace Data

#endif
