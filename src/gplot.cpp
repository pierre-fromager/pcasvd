
#include <gplot.h>

template <typename T>
Gplot<T>::Gplot(gplot_params_s<T> params) : m_params(params) {}

template <typename T>
Gplot<T>::~Gplot() {}

template <typename T>
void Gplot<T>::drawScatter(void)
{
   gp << SET_TERMINAL_PNG << std::endl;
   gp << boost::format(SET_OUTPUT_FILENAME_FMT) % m_params.filename << std::endl;
   gp << UNSET_COLORBOX << std::endl;
   gp << "set title '" << m_params.title << "'" << std::endl;
   gp << "labels='" << m_params.legend << "'" << std::endl;
   gp << "set samples words(labels)" << std::endl;
   gp << "kx=" << m_params.hxrange << "-2;" << std::endl;
   gp << "ky=" << m_params.hyrange << "-0.5;" << std::endl;
   gp << "kdy=0.2" << std::endl;
   gp << SET_PALETTE << PALETTE_DEF << std::endl;
   gp << SET_XAXIS_LABEL << std::endl;
   gp << SET_YAXIS_LABEL << std::endl;
   gp << boost::format(SET_XRANGE_FMT) % m_params.lxrange % m_params.hxrange
      << std::endl;
   gp << boost::format(SET_YRANGE_FMT) % m_params.lyrange % m_params.hyrange
      << std::endl;
   gp << PLOT
      << gp.file1d(m_params.xyc)
      << "using 1:2:($3) notitle with points ps 0.5 pt 7 palette,"
      << "'+' using (kx):(ky + $0*kdy):(word(labels, int($0+1))):0 "
      << "with labels left offset 1,-0.1 point pt 7 palette t ''"
      << std::endl;
}

template class Gplot<double>;
template class Gplot<float>;
template class Gplot<int>;