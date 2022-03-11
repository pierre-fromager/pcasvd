
#ifndef GPLOT_SPECIES_H
#define GPLOT_SPECIES_H

#include <string>
#include <gplot.h>
#include <pca.h>

template <typename T>
class GplotSpecies : public Gplot<T>
{
public:
    explicit GplotSpecies(pca_result_s<T> &result);
    ~GplotSpecies();
    void scatter(const std::string &filename);
    void corcricle(const std::string &filename);
    void heatmap(const std::string &filename);
    void boxwiskers(const std::string &filename);

private:
    pca_result_s<T> m_result;
};

#endif
