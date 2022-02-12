
#ifndef PCA_RESULT_H
#define PCA_RESULT_H

#include <alglib/ap.h>

template <typename T>
struct pca_result_s
{
    ui_t cols = 0;
    ui_t rows = 0;
    alglib::real_2d_array cov, cor, proj, eig_vectors;
    alglib::real_1d_array eig_values, exp_variance;
};

#endif
