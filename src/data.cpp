
#include "data.h"

namespace Data
{

namespace File
{

template <typename T>
Csv<T>::Csv(std::string s) : m_separator(s)
{
}

template <typename T>
Csv<T>::~Csv()
{
    m_lines.clear();
    m_buffer.clear();
}

template <typename T>
void Csv<T>::load(std::string f, unsigned int skip)
{
    m_filename = f;
    m_metas.filename = f;
    m_metas.sep = m_separator;
    std::ifstream is(m_filename);
    std::string line;
    std::vector<std::string> sitems;
    size_t cpt;
    unsigned int nbrow = 0, nbcol = 0;
    const bool isFloat = std::is_same<T, double>::value || std::is_same<T, float>::value;
    const bool isInt = std::is_same<T, int>::value;
    if (is.is_open())
    {
        while (getline(is, line))
        {
            boost::split(
                sitems,
                line,
                boost::is_any_of(m_separator),
                boost::token_compress_on);
            const size_t sitemsSize = sitems.size();
            nbrow++;
            if (nbrow > skip)
            {
                m_lines.push_back(line);
                for (cpt = 0; cpt < sitemsSize; cpt++)
                    if (isFloat)
                        m_buffer.push_back(atof(sitems[cpt].c_str()));
                    else if (isInt)
                        m_buffer.push_back(atoi(sitems[cpt].c_str()));
                m_metas.cols = cpt;
            }
        }
        m_metas.rows = m_lines.size();
        is.close();
    }
}

template <typename T>
void Csv<T>::save(std::string f)
{
    m_filename = f;
}

template <typename T>
std::vector<T> Csv<T>::buffer(void)
{
    return m_buffer;
}

template <typename T>
metas_t Csv<T>::metas(void)
{
    return m_metas;
}

template class Csv<double>;
template class Csv<float>;
template class Csv<int>;

} // namespace File
} // namespace Data