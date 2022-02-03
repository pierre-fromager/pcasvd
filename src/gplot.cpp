
#include <gplot.h>

template <typename T>
Gplot<T>::Gplot(gplot_params_s<T> params) : m_params(params) {}

template <typename T>
Gplot<T>::~Gplot() {}

template <typename T>
void Gplot<T>::drawScatter(void)
{
   initPng();
   setTitle();
   gp << "labels=" << SQ << m_params.legend << SQ << std::endl;
   gp << _SET << "samples words(labels)" << std::endl;
   gp << "kx=" << m_params.hxrange << "-2;" << std::endl;
   gp << "ky=" << m_params.hyrange << "-0.5;" << std::endl;
   gp << "kdy=0.2" << std::endl;
   setAxisLabels();
   setRanges();
   gp << _PLOT
      << gp.file1d(m_params.serie_xyc)
      << USING_SCATTER
      << SET_LEGEND
      << std::endl;
}

template <typename T>
void Gplot<T>::drawCorCircle(void)
{
   initPng();
   setTitle();
   gp << "set size square" << std::endl;
   gp << "labels=" << SQ << m_params.legend << SQ << std::endl;
   gp << _SET << "samples words(labels)" << std::endl;
   gp << "kx=" << m_params.hxrange << std::endl;
   gp << "ky=" << m_params.hyrange << "-0.5;" << std::endl;
   gp << "kdy=0.1" << std::endl;
   setAxisLabels();
   setRanges();
   gp << SET_CIRCLE << std::endl;
   gp << _PLOT
      << gp.file1d(m_params.serie_ooxyc)
      << USING_CORCIRCLE
      << SET_LEGEND
      << std::endl;
}

template <typename T>
void Gplot<T>::initPng(void)
{
   gp << boost::format(SET_TERMINAL_PNG) % m_params.width % m_params.height << std::endl;
   gp << boost::format(SET_OUTPUT_FILENAME_FMT) % m_params.filename << std::endl;
   gp << UNSET_COLORBOX << std::endl;
   setDefaultFontsSizes();
   setDefaultPalette();
}

template <typename T>
void Gplot<T>::setDefaultFontsSizes(void)
{
   gp << _SET << "title font " << SQ DEFAULT_FONT COMA
      << 12 << SQ SMC
      << _SET << "xlabel font " << SQ DEFAULT_FONT COMA
      << 9 << SQ SMC
      << _SET << "ylabel font " << SQ DEFAULT_FONT COMA
      << 9 << SQ SMC
      << _SET << "tics font " << SQ DEFAULT_FONT COMA
      << 7 << SQ
      << std::endl;
}

template <typename T>
void Gplot<T>::setDefaultPalette(void)
{
   gp << SET_PALETTE << PALETTE_DEF << std::endl;
}

template <typename T>
void Gplot<T>::setAxisLabels(void)
{
   gp << SET_XLABEL SQ << m_params.xlabel << SQ << std::endl;
   gp << SET_YLABEL SQ << m_params.ylabel << SQ << std::endl;
}

template <typename T>
void Gplot<T>::setRanges(void)
{
   gp << boost::format(SET_XRANGE_FMT) % m_params.lxrange % m_params.hxrange
      << std::endl;
   gp << boost::format(SET_YRANGE_FMT) % m_params.lyrange % m_params.hyrange
      << std::endl;
}

template <typename T>
void Gplot<T>::setTitle(void)
{
   gp << "set title '" << m_params.title << SQ << std::endl;
}

template class Gplot<double>;
template class Gplot<float>;
template class Gplot<int>;