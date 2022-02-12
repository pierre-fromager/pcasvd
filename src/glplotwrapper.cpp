
#include <gplotwrapper.h>

template <typename T>
GplotWrapper<T>::GplotWrapper(pca_result_s<T> &result) : m_result(result)
{
}

template <typename T>
GplotWrapper<T>::~GplotWrapper()
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

template <typename T>
void GplotWrapper<T>::scatter(const std::string &filename)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.width = 1024;
    gparams.height = 768;
    gparams.title = "Scatter plot (Individual factor map) : Iris species";
    gparams.xlabel = "PC1 Petal.W(" + std::to_string(m_result.exp_variance[0] * 100) + "%)";
    gparams.ylabel = "PC2 Petal.L(" + std::to_string(m_result.exp_variance[1] * 100) + "%)";
    gparams.legend = "Setosa Versicolor Virginica";
    std::vector<T> col1, col2;
    getCol<T>(m_result.proj, 0, col1);
    const T margin = 0.5;
    gparams.lxrange = (*min_element(col1.begin(), col1.end())) - margin;
    gparams.hxrange = (*max_element(col1.begin(), col1.end())) + margin;
    getCol<T>(m_result.proj, 1, col2);
    gparams.lyrange = (*min_element(col2.begin(), col2.end())) - margin;
    gparams.hyrange = (*max_element(col2.begin(), col2.end())) + margin;
    T category;
    ui_t coSize = col1.size();
    for (ui_t c = 0; c < coSize; c++)
    {
        if (c < 52)
            category = 0;
        else if (c < 102)
            category = 1;
        else
            category = 2;
        gparams.serie_xyc.emplace_back(col1[c], col2[c], category);
    }
    col1.clear();
    col2.clear();
    Gplot<T> *gpl = new Gplot<T>();
    gpl->setParams(gparams);
    gpl->drawScatter();
    gparams.serie_xyc.clear();
    delete (gpl);
}

template <typename T>
void GplotWrapper<T>::corcricle(const std::string &filename)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.height = 1024;
    gparams.width = gparams.height + 100;
    gparams.title = "Variable factor map : Iris (4 components)";
    gparams.xlabel = "PC1 Petal.W(" + std::to_string(m_result.exp_variance[0] * 100) + "%)";
    gparams.ylabel = "PC2 Petal.L(" + std::to_string(m_result.exp_variance[1] * 100) + "%)";
    gparams.legend = "SepLen SepWid PetLen PetWid";
    const T margin = 0.1;
    std::vector<T> col1, col2;
    getCol<T>(m_result.eig_vectors, 0, col1);
    getCol<T>(m_result.eig_vectors, 1, col2);
    gparams.lxrange = gparams.lyrange = -1 - margin;
    gparams.hxrange = gparams.hyrange = 1 + margin;
    T category;
    ui_t coSize = col1.size();
    for (ui_t c = 0; c < coSize; c++)
        gparams.serie_ooxyc.emplace_back(0, 0, col1[c], col2[c], c);
    col1.clear();
    col2.clear();
    Gplot<T> *gpl = new Gplot<T>();
    gpl->setParams(gparams);
    gpl->drawCorCircle();
    gparams.serie_ooxyc.clear();
    delete (gpl);
}

template <typename T>
void GplotWrapper<T>::heatmap(const std::string &filename)
{
    struct gplot_params_s<T> gparams;
    gparams.filename = filename;
    gparams.height = gparams.width = 800;
    gparams.title = "Heatmap Correlation : Iris";
    gparams.lxrange = gparams.lyrange = -1;
    gparams.hxrange = gparams.hyrange = 1;
    gparams.xlabel = gparams.ylabel = "Components";
    gparams.legend = "Sepal.L,Sepal.W,Petal.L,Petal.W";
    gparams.mat = m_result.cor;
    Gplot<T> *gpl = new Gplot<T>();
    gpl->setParams(gparams);
    gpl->drawHeatmap();
    delete (gpl);
}

template class GplotWrapper<double>;