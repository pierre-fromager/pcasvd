
#include <datafile.h>

namespace Data
{

namespace File
{

template <typename T>
Csv<T>::Csv()
{
    m_metas.filename = "";
    m_metas.cols = m_metas.rows = 0;
}

template <typename T>
Csv<T>::~Csv()
{
    reset();
}

template <typename T>
void Csv<T>::reset(void)
{
    m_lines.clear();
    m_buffer.clear();
}

template <typename T>
void Csv<T>::load(void)
{
    reset();
    std::ifstream is(m_metas.filename);
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
                boost::is_any_of(m_metas.sep),
                boost::token_compress_on);
            const size_t sitemsSize = sitems.size();

            if (nbrow >= m_metas.skip)
            {
                m_lines.push_back(line);
                for (cpt = 0; cpt < sitemsSize; cpt++)
                    if (isFloat)
                        m_buffer.push_back(atof(sitems[cpt].c_str()));
                    else if (isInt)
                        m_buffer.push_back(atoi(sitems[cpt].c_str()));
                m_metas.cols = cpt;
            }
            nbrow++;
        }
        m_metas.rows = m_lines.size();
        is.close();
    } else {
        const std::string &errMsg = "cant open file :" + m_metas.filename;
        throw std::invalid_argument(errMsg); 
    }
}

template <typename T>
void Csv<T>::save(void)
{
    unsigned int i, j;
    std::ofstream os(m_metas.filename);
    std::ostringstream strval;
    m_lines.clear();
    std::string line;
    for (j = 0; j < m_metas.rows; j++)
    {
        strval.str("");
        strval.clear();
        for (i = 0; i < m_metas.cols; i++)
        {
            const T v = m_buffer[i + (j * m_metas.cols)];
            strval << boost::lexical_cast<std::string>(v) << m_metas.sep;
        }
        line = strval.str();
        line.pop_back();
        m_lines.push_back(line);
    }
    if (os.is_open())
    {
        if (m_metas.header.length() > 0)
            os << m_metas.header << std::endl;
        for (j = 0; j < m_metas.rows; j++)
            os << m_lines[j] << std::endl;
        os.close();
    }
}

template <typename T>
void Csv<T>::setBuffer(std::vector<T> buffer)
{
    m_buffer = buffer;
}

template <typename T>
void Csv<T>::setMetas(metas_t metas)
{
    m_metas = metas;
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