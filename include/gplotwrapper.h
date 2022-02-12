
#ifndef GPLOT_WRAPPER_H
#define GPLOT_WRAPPER_H

#include <string>
#include <gplot.h>
#include <pca.h>

template <typename T>
class GplotWrapper : public Gplot<T>
{
public:
    explicit GplotWrapper(pca_result_s<T> &result);
    ~GplotWrapper();
    void scatter(const std::string &filename);
    void corcricle(const std::string &filename);
    void heatmap(const std::string &filename);

private:
    pca_result_s<T> m_result;
};

#endif
