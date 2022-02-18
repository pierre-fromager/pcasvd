
#include <pca.h>

template <typename T>
Pca<T>::Pca(alglib::real_2d_array values) : m_values(values)
{
}

template <typename T>
Pca<T>::~Pca() {}

template <typename T>
ui_t Pca<T>::rows()
{
    return m_values.rows();
}

template <typename T>
ui_t Pca<T>::cols()
{
    return m_values.cols();
}

template <typename T>
void Pca<T>::process()
{
    m_result.rows = rows();
    m_result.cols = cols();
    struct tasks pool;
    const ui_t &nbTasks = 3;
    pool.start(nbTasks);
    std::future<ui_t> tcov = pool.queue([this](void) mutable {
        alglib::covm(m_values, m_result.rows, m_result.cols, m_result.cov);
        return static_cast<ui_t>(0);
    });
    std::future<ui_t> tcor = pool.queue([this](void) mutable {
        alglib::pearsoncorrm(m_values, m_result.rows, m_result.cols, m_result.cor);
        return static_cast<ui_t>(0);
    });
    std::future<ui_t> tpca = pool.queue([this](void) {
        alglib::ae_int_t info;
        alglib::pcabuildbasis(
            m_values,
            m_result.rows,
            m_result.cols,
            info,
            m_result.eig_values,
            m_result.eig_vectors);
        return static_cast<ui_t>(info);
    });
    tpca.wait();
    std::future<ui_t> texvar = pool.queue([this](void) mutable {
        T eigvaSum = 0;
        ui_t i;
        for (i = 0; i < m_result.cols; i++)
            eigvaSum += m_result.eig_values[i];
        m_result.exp_variance = m_result.eig_values;
        if (eigvaSum != 0)
            for (i = 0; i < m_result.cols; i++)
                m_result.exp_variance[i] /= eigvaSum;
        return static_cast<ui_t>(0);
    });
    std::future<ui_t> tproj = pool.queue([this](void) mutable {
        std::vector<T> projMat(m_result.cols * m_result.rows, 0);
        m_result.proj.setcontent(m_result.rows, m_result.cols, projMat.data());
        projMat.clear();
        projection(m_values, m_result.eig_vectors, m_result.proj);
        return static_cast<ui_t>(0);
    });
    pool.finish();
}

template <typename T>
pca_result_s<T> Pca<T>::results()
{
    return m_result;
}

template <typename T>
void Pca<T>::projection(
    alglib::real_2d_array &a,
    alglib::real_2d_array &b,
    alglib::real_2d_array &result)
{
    const double alpha = 1.0;
    const alglib::ae_int_t ia = 0;
    const alglib::ae_int_t ja = 0;
    const alglib::ae_int_t optypea = 0;
    const alglib::ae_int_t ib = 0;
    const alglib::ae_int_t jb = 0;
    const alglib::ae_int_t optypeb = 0;
    const double beta = 0.0;
    const alglib::ae_int_t ic = 0;
    const alglib::ae_int_t jc = 0;
    alglib::rmatrixgemm(
        a.rows(),
        b.cols(),
        a.cols(),
        alpha,
        a,
        ia,
        ja,
        optypea,
        b,
        ib,
        jb,
        optypeb,
        beta,
        result,
        ic,
        jc);
}

template class Pca<double>;