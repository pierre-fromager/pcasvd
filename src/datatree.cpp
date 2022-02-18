
#include <datatree.h>

/**
 * @brief Data::File::Tree
 * @todo increase precision (default 6) and keep avoiding scientific notation
 * 
 * https://thispointer.com/c-convert-double-to-string-and-manage-precision-scientific-notation/
 */
namespace Data
{

namespace File
{

template <typename T>
Tree<T>::Tree()
{
}

template <>
Tree<double>::Tree() : m_cols(0), m_rows(0)
{
}

template <>
Tree<std::string>::Tree()
{
}

template <typename T>
Tree<T>::~Tree()
{
    reset();
}

template <typename T>
pt::ptree Tree<T>::getRoot(void)
{
    return m_root;
}

template <typename T>
void Tree<T>::reset(void)
{
}

template <typename T>
void Tree<T>::load(const std::string &filename)
{
    pt::read_json(filename, m_root);
}

template <>
void Tree<double>::load(const std::string &filename)
{
    pt::read_json(filename, m_root);
    m_cols = m_root.get<double>(_COLS, 0);
    m_rows = m_root.get<double>(_ROWS, 0);
}

template <>
ui_t Tree<double>::cols()
{
    return m_cols;
}

template <>
ui_t Tree<double>::rows()
{
    return m_rows;
}

template <typename T>
bool Tree<T>::hasChild(const std::string &entry)
{
    boost::optional<pt::ptree &> child = m_root.get_child_optional(entry);
    if (!child)
        return false;
    return true;
}

static ui_t convVectorStrToDouble(std::vector<std::string> vs, std::vector<double> &vd)
{
    std::transform(vs.begin(), vs.end(), vd.begin(), [](const std::string &v) {
        return std::stod(v);
    });
    return 0;
}

template <>
ui_t Tree<double>::vector(const std::string &entry, std::vector<double> &vd)
{
    if (false == hasChild(entry))
        return 1;
    std::vector<std::string> vs;
    vd.clear();
    for (pt::ptree::value_type &vt : m_root.get_child(entry))
    {
        vs.push_back(vt.second.data());
        vd.push_back(0);
    }
    convVectorStrToDouble(vs, vd);
    vs.clear();
    if (vd.size() > 0)
        return 0;
    return 1;
}

static ui_t vectorStr(
    pt::ptree m_root,
    const std::string &entry,
    std::vector<std::string> &t)
{
    t.clear();
    for (pt::ptree::value_type &elts : m_root.get_child(entry))
        t.push_back(elts.second.data());
    if (t.size() > 0)
        return 0;
    return 1;
}

template <typename T>
ui_t Tree<T>::vector(const std::string &entry, std::vector<std::string> &t)
{
    if (false == hasChild(entry))
        return 1;
    return vectorStr(m_root, entry, t);
}

template <>
ui_t Tree<double>::vector(const std::string &entry, alglib::real_1d_array &a)
{
    std::vector<double> vd;
    err = vector(entry, vd);
    if (err == 0)
        a.setcontent(vd.size(), vd.data());
    return err;
}

template <>
ui_t Tree<std::string>::vector(const std::string &entry, alglib::real_1d_array &a)
{
    return 1;
}

template <>
ui_t Tree<double>::matrix(const std::string &entry, ui_t &cols, ui_t &rows, std::vector<double> &vd)
{
    if (false == hasChild(entry))
        return 1;
    std::vector<std::string> vs;
    vd.clear();
    cols = rows = 0;
    for (pt::ptree::value_type &row : m_root.get_child(entry))
    {
        cols = 0;
        for (pt::ptree::value_type &cell : row.second)
        {
            vs.push_back(cell.second.data());
            vd.push_back(0);
            cols++;
        }
        rows++;
    }
    convVectorStrToDouble(vs, vd);
    if (vd.size() > 0)
        return 0;
    return 1;
}

template <>
ui_t Tree<double>::matrix(const std::string &entry, alglib::real_2d_array &m)
{
    ui_t cols, rows;
    std::vector<double> v;
    err = matrix(entry, cols, rows, v);
    if (err == 0)
        m.setcontent(rows, cols, v.data());
    return err;
}

template <>
ui_t Tree<std::string>::matrix(const std::string &entry, alglib::real_2d_array &m)
{
    return 1;
}

template <typename T>
ui_t Tree<T>::value(const std::string &entry, std::string &s)
{
    if (false == hasChild(entry))
        return 1;
    s = m_root.get<std::string>(entry);
    return 0;
}

template <typename T>
ui_t Tree<T>::value(const std::string &entry, double &d)
{
    if (false == hasChild(entry))
        return 1;
    d = m_root.get<double>(entry);
    return 0;
}

template <typename T>
ui_t Tree<T>::addValue(const std::string &entry, const double &d)
{
    m_root.put<double>(entry, d);
    return 0;
}

template <typename T>
ui_t Tree<T>::addValue(const std::string &entry, const std::string &s)
{
    m_root.put(entry, s);
    return 0;
}

template <typename T>
static void addVectorGeneric(std::vector<T> v, pt::ptree &entries_node)
{
    const std::string &e = "";
    for (auto &elt : v)
    {
        pt::ptree entry_node;
        entry_node.put(e, elt);
        entries_node.push_back(std::make_pair(e, entry_node));
    }
}

template <typename T>
ui_t Tree<T>::addVector(const std::string &entry, std::vector<std::string> vs)
{
    pt::ptree entries_node;
    addVectorGeneric(vs, entries_node);
    m_root.add_child(entry, entries_node);
    return 0;
}

template <typename T>
ui_t Tree<T>::addVector(const std::string &entry, std::vector<double> vd)
{
    pt::ptree entries_node;
    addVectorGeneric(vd, entries_node);
    m_root.add_child(entry, entries_node);
    return 0;
}


/**
 * @brief add vector from alglib::real_1d_array
 * @todo replace by std::ostringstream oss & std::setprecision(P) & oss.str()
 * 
 * @tparam T 
 * @param entry 
 * @param ad 
 * @return ui_t 
 */
template <typename T>
ui_t Tree<T>::addVector(const std::string &entry, alglib::real_1d_array ad)
{
    pt::ptree entries_node;
    ui_t c;
    const ui_t l = ad.length();
    double *adc = ad.getcontent();
    std::vector<std::string> vs;
    vs.clear();
    for (c = 0; c < l; c++)
        vs.push_back(std::to_string(*(adc + c)));
    addVector(entry, vs);
    vs.clear();
    return 0;
}

/**
 * @brief add Matrix from vector of double
 * @todo replace by std::ostringstream oss & std::setprecision(P) & oss.str()
 * 
 * @tparam  
 * @param entry 
 * @param cols 
 * @param rows 
 * @param vd 
 * @return ui_t 
 */
template <>
ui_t Tree<double>::addMatrix(
    const std::string &entry, 
    ui_t cols, 
    ui_t rows, 
    std::vector<double> vd)
{

    if (false == hasChild(entry))
        return 1;
    ui_t i, j;
    pt::ptree matrix_node;
    for (j = 0; j < rows; j++)
    {
        pt::ptree row;
        for (i = 0; i < cols; i++)
        {
            pt::ptree cell;
            cell.put_value(vd[(j * cols) + i]);
            row.push_back(std::make_pair("", cell));
        }
        matrix_node.push_back(std::make_pair("", row));
    }
    if (vd.size() > 0)
    {
        m_root.add_child(entry, matrix_node);
        return 0;
    }

    return 1;
}

/**
 * @brief addMatrix from alglib::real_2d_array
 * @todo replace by std::ostringstream oss & std::setprecision(P) & oss.str()
 * 
 * @tparam  
 * @param entry 
 * @param m 
 * @return ui_t 
 */
template <>
ui_t Tree<double>::addMatrix(const std::string &entry, alglib::real_2d_array m)
{
    ui_t i, j;
    pt::ptree matrix_node;
    for (j = 0; j < m.rows(); j++)
    {
        pt::ptree row;
        for (i = 0; i < m.cols(); i++)
        {
            pt::ptree cell;
            std::to_string(m(j, i)); 
            cell.put_value(std::to_string(m(j, i)));
            row.push_back(std::make_pair("", cell));
        }
        matrix_node.push_back(std::make_pair("", row));
    }
    m_root.add_child(entry, matrix_node);
    return err;
}

static void formatRootJson(pt::ptree m_root, std::string &result)
{
    std::ostringstream oss;
    pt::write_json(oss, m_root);
    std::regex reg(JSON_NUMERIC_OUT_FMT);
    result = std::regex_replace(oss.str(), reg, "$1");
}

template <typename T>
void Tree<T>::save(const std::string &path)
{
    std::ostringstream oss;
    std::string formatedOutput;
    formatRootJson(m_root, formatedOutput);
    std::ofstream file;
    file.open(path);
    file << formatedOutput;
    file.close();
}

template <typename T>
void Tree<T>::save(std::ostream &stream)
{
    std::string formatedOutput;
    formatRootJson(m_root, formatedOutput);
    stream << formatedOutput;
}

template class Tree<double>;
template class Tree<std::string>;
//template class Tree<float>;
//template class Tree<int>;

} // namespace File
} // namespace Data