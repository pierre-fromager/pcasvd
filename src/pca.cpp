
#include <pca.h>

template <typename T>
Pca<T>::Pca(alglib::real_2d_array values, ui_t cols, ui_t rows) : m_values(values),
                                                                  m_cols(cols),
                                                                  m_rows(rows)
{
}

template <typename T>
Pca<T>::~Pca() {}

template <typename T>
void Pca<T>::process()
{
    const ui_t r = m_result.rows = m_rows;
    const ui_t c = m_result.cols = m_cols;
    const ui_t maxrow = 5;
    alglib::covm(m_values, r, c, m_result.cov);
    alglib::pearsoncorrm(m_values, r, c, m_result.cor);
    alglib::ae_int_t info;
    alglib::pcabuildbasis(
        m_values,
        r,
        c,
        info,
        m_result.eig_values,
        m_result.eig_vectors);
    T eigvaSum = 0;
    for (i = 0; i < c; i++)
        eigvaSum += m_result.eig_values[i];
    m_result.exp_variance = m_result.eig_values;
    for (i = 0; i < c; i++)
        m_result.exp_variance[i] = (m_result.eig_values[i] / eigvaSum);
    T *projMat = new T[c * r];
    m_result.proj.setcontent(r, c, (T *)&projMat);
    projection(m_values, m_result.eig_vectors, m_result.proj);
    delete projMat;
    /*
        alglib::real_1d_array w;
        alglib::fisherlda(ptInput, r, c, c, info, w);
        disp->vec("Lda", w, 2);
        */
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