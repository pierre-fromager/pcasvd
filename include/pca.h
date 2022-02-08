
#ifndef PCA_H
#define PCA_H

#include <fixture.h>
#include <alglib/statistics.h>
#include <alglib/dataanalysis.h>

template <typename T>
struct pca_result_s
{
    ui_t cols = 0;
    ui_t rows = 0;
    alglib::real_2d_array cov, cor, proj, eig_vectors;
    alglib::real_1d_array eig_values, exp_variance;
};

template <typename T>
class Pca
{
public:
    explicit Pca(alglib::real_2d_array values);
    ~Pca();
    ui_t cols(void);
    ui_t rows(void);
    void process();
    pca_result_s<T> results();

private:
    alglib::real_2d_array m_values;
    std::string error_msg;
    pca_result_s<T> m_result;
    ui_t i, j;
    alglib::real_2d_array mcov, mcorr, mproj, resproj;
    void projection(alglib::real_2d_array &a,
                    alglib::real_2d_array &b,
                    alglib::real_2d_array &result);
};

#endif
