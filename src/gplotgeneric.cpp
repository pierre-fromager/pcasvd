
#include <gplotgeneric.h>

template <typename T>
GplotGeneric<T>::GplotGeneric(pca_result_s<T> &result) : m_result(result)
{
}

template <typename T>
GplotGeneric<T>::~GplotGeneric()
{
}

template <typename T>
static void getCol(alglib::real_2d_array a, const ui_t &colnum, std::vector<T> &col)
{
    col.clear();
    ui_t i, j;
    const ui_t cols = a.cols();
    const ui_t rows = a.rows();
    for (j = 0; j < rows; j++)
        for (i = 0; i < cols; i++)
            if (i == colnum)
                col.push_back(a[j][i]);
            else if (i > colnum)
                continue;
}

static void splitString(std::string str, std::string delim, std::vector<std::string> &vec)
{
    boost::split(vec, str, boost::is_any_of(delim), boost::token_compress_on);
}

template <typename T>
static std::string toStringPrecision(const T v, const int n = 0)
{
    std::ostringstream oss;
    oss.precision(n);
    oss << std::fixed << v;
    return oss.str();
}

static ui_t getLegendColNumber(std::vector<std::string> headers)
{
    ui_t c;
    for (c = 0; c < headers.size(); c++)
        if (headers[c].find(_SHARP_) != std::string::npos)
            return c;
    return 0;
}

template <typename T>
static void reduceScatterLegend(std::vector<T> colLegend, std::vector<std::string> &reduced)
{
    ui_t c;
    std::vector<T> colLegendUniq;
    colLegendUniq = colLegend;
    std::sort(colLegendUniq.begin(), colLegendUniq.end());
    auto lastLegenItem = std::unique(colLegendUniq.begin(), colLegendUniq.end());
    colLegendUniq.erase(lastLegenItem, colLegendUniq.end());
    reduced.clear();
    for (c = 0; c < colLegendUniq.size(); c++)
        reduced.push_back(toStringPrecision(colLegendUniq[c]));
}

template <typename T>
void GplotGeneric<T>::colsReset(void)
{
    col1.clear();
    col2.clear();
    colLegend.clear();
}

template <typename T>
void GplotGeneric<T>::scatter(const std::string &filename)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.header = m_result.header;
    gparams.width = 1024;
    gparams.height = 768;
    ui_t c;
    colsReset();
    std::vector<std::string> headers, reduced;
    splitString(gparams.header, m_result.delimiter, headers);
    ui_t legendColNum = getLegendColNumber(headers);
    getCol<T>(m_result.src, legendColNum, colLegend);
    reduceScatterLegend(colLegend, reduced);
    std::stringstream zelegend;
    const std::string &ldelim = _SP_;
    std::copy(
        reduced.begin(),
        reduced.end(),
        std::ostream_iterator<std::string>(zelegend, ldelim.c_str()));
    gparams.title = _IND_FAC_MAP_ + m_result.filename;
    gparams.xlabel = _DIM1_ + headers[m_result.opts.d1c] + _OPAR_ + std::to_string(m_result.exp_variance[m_result.opts.d1c] * 100) + _CPAR_;
    gparams.ylabel = _DIM2_ + headers[m_result.opts.d2c] + _OPAR_ + std::to_string(m_result.exp_variance[m_result.opts.d2c] * 100) + _CPAR_;
    gparams.legend = zelegend.str();
    gparams.legend = gparams.legend.substr(0, gparams.legend.size() - 1);
    getCol<T>(m_result.proj, m_result.opts.d1c, col1);
    const T margin = 1;
    gparams.lxrange = (*min_element(col1.begin(), col1.end())) - margin;
    gparams.hxrange = (*max_element(col1.begin(), col1.end())) + margin;
    getCol<T>(m_result.proj, m_result.opts.d2c, col2);
    gparams.lyrange = (*min_element(col2.begin(), col2.end())) - margin;
    gparams.hyrange = (*max_element(col2.begin(), col2.end())) + margin;
    ui_t coSize = col1.size();
    for (c = 0; c < coSize; c++)
        gparams.serie_xyc.emplace_back(col1[c], col2[c], colLegend[c]);
    col1.clear();
    col2.clear();
    this->setParams(gparams);
    this->drawScatter();
    gparams.serie_xyc.clear();
}

template <typename T>
void GplotGeneric<T>::corcricle(const std::string &filename)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.header = m_result.header;
    gparams.height = 1024;
    gparams.width = gparams.height + 100;
    ui_t c;
    colsReset();
    std::vector<std::string> headers, reduced;
    splitString(gparams.header, m_result.delimiter, headers);
    ui_t legendColNum = getLegendColNumber(headers);
    getCol<T>(m_result.src, legendColNum, colLegend);
    reduceScatterLegend(colLegend, reduced);
    std::stringstream zelegend;
    const std::string &ldelim = _SP_;
    std::copy(
        reduced.begin(),
        reduced.end(),
        std::ostream_iterator<std::string>(zelegend, ldelim.c_str()));
    gparams.title = _VAR_FAC_MAP_ + m_result.filename;
    gparams.xlabel = _DIM1_ + headers[m_result.opts.d1c] + _OPAR_ + std::to_string(m_result.exp_variance[m_result.opts.d1c] * 100) + _CPAR_;
    gparams.ylabel = _DIM2_ + headers[m_result.opts.d2c] + _OPAR_ + std::to_string(m_result.exp_variance[m_result.opts.d2c] * 100) + _CPAR_;
    gparams.legend = zelegend.str();
    gparams.legend = gparams.legend.substr(0, gparams.legend.size() - 1);
    const T margin = 0.1;
    colsReset();
    getCol<T>(m_result.eig_vectors, m_result.opts.d1c, col1);
    getCol<T>(m_result.eig_vectors, m_result.opts.d2c, col2);
    gparams.lxrange = gparams.lyrange = -1 - margin;
    gparams.hxrange = gparams.hyrange = 1 + margin;
    T category;
    ui_t coSize = col1.size();
    for (c = 0; c < coSize; c++)
        gparams.serie_ooxyc.emplace_back(0, 0, col1[c], col2[c], c);
    col1.clear();
    col2.clear();
    this->setParams(gparams);
    this->drawCorCircle();
    gparams.serie_ooxyc.clear();
}

template <typename T>
void GplotGeneric<T>::heatmap(const std::string &filename)
{
    colsReset();
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.header = m_result.header;
    
    gparams.title = _HEAT_COR_ + m_result.filename;
    gparams.lxrange = gparams.lyrange = -1;
    gparams.hxrange = gparams.hyrange = 1;
    gparams.xlabel = gparams.ylabel = _COMPS_;
    std::vector<std::string> headers;
    splitString(gparams.header, m_result.delimiter, headers);
    gparams.height = gparams.width = 800 + ((headers.size() - 2) * 70);
    const std::string &ldelim = ",";
    std::stringstream zelegend;
    std::copy(
        headers.begin(),
        headers.end(),
        std::ostream_iterator<std::string>(zelegend, ldelim.c_str()));
    gparams.legend = zelegend.str();
    gparams.legend = gparams.legend.substr(0, gparams.legend.size() - 1);
    gparams.mat = m_result.cor;
    this->setParams(gparams);
    this->drawHeatmap();
}

template <typename T>
void GplotGeneric<T>::boxwiskers(const std::string &filename)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.header = m_result.header;
    gparams.infilename = m_result.filename;
    gparams.delimiter = m_result.delimiter;
    gparams.height = gparams.width = 2048;
    gparams.title = _BOX_WISK_ + gparams.infilename;
    gparams.xlabel = _COMPS_;
    gparams.ylabel = _RATIOS_;
    std::vector<std::string> headers;
    splitString(gparams.header, m_result.delimiter, headers);
    const std::string &ldelim = " ";
    std::stringstream zelegend;
    std::copy(
        headers.begin(),
        headers.end(),
        std::ostream_iterator<std::string>(zelegend, ldelim.c_str()));
    gparams.legend = zelegend.str();
    gparams.legend = gparams.legend.substr(0, gparams.legend.size() - 1);
    this->setParams(gparams);
    this->drawBoxAndWiskers();
}

template class GplotGeneric<double>;