
#include <gplot.h>

template <typename T>
Gplot<T>::Gplot()
{
}

template <typename T>
Gplot<T>::~Gplot() {}

template <typename T>
void Gplot<T>::setParams(gplot_params_s<T> params)
{
   m_params = params;
}

template <typename T>
void Gplot<T>::drawScatter(void)
{
   resetSession();
   initPng();
   setTitle();
   setLegendParams(m_params.hxrange - 1, m_params.hyrange - 0.5, 0.05);
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
   resetSession();
   initPng();
   setTitle();
   gp << _SET << "size square" << std::endl;
   setLegendParams(m_params.hxrange, m_params.hyrange - 0.5, 0.05);
   setAxisLabels();
   setRanges();
   gp << SET_CIRCLE_1 << std::endl;
   gp << SET_CIRCLE_2 << std::endl;
   gp << _SET << "arrow 1 from -1,0 to 1,0 nohead lt 0" << std::endl;
   gp << _SET << "arrow 2 from 0,-1 to 0,1 nohead lt 0" << std::endl;
   gp << _PLOT
      << gp.file1d(m_params.serie_ooxyc)
      << USING_CORCIRCLE
      << SET_LEGEND
      << std::endl;
}

template <typename T>
void Gplot<T>::drawHeatmap(void)
{
   resetSession();
   initPng();
   setTitle();
   gp << SET_COLORBOX << std::endl;
   setAxisLabels();
   ui_t i, j;
   const ui_t si = m_params.mat.cols();
   const ui_t sj = m_params.mat.rows();
   const std::string delim = COMA;
   std::vector<std::string> strows;
   splitStr(m_params.legend, delim, strows);
   std::string line;
   if (si == sj)
   {
      gp << _SET << "palette rgbformulae 33,13,10" << std::endl;
      gp << "$heatmap <<" << EOD << std::endl;
      gp << COMA << m_params.legend << std::endl;
      for (j = 0; j < sj; j++)
      {
         line = strows[sj - 1 - j] + COMA;
         for (i = 0; i < si; i++)
            line += std::to_string(m_params.mat[sj - 1 - j][i]) + COMA;
         line.pop_back();
         gp << line << std::endl;
      }
      gp << EOD << std::endl;
      gp << _SET << DATAFILE_SEPARATOR << " comma" << std::endl;
      const std::string heatmapMatrix = "$heatmap matrix rowheaders columnheaders";
      gp << _PLOT
         << heatmapMatrix << " u 1:2:3 w image,"
         << heatmapMatrix << " u 1:2:($3 == 0 ? '' : sprintf(\"%g\",$3)) w labels"
         << std::endl;
      gp << _SET << DATAFILE_SEPARATOR << std::endl;
   }
   strows.clear();
}

template <typename T>
void Gplot<T>::drawBoxAndWiskers(void)
{
   resetSession();
   initPng();
   setTitle();
   gp << "file = '" << m_params.infilename << "'" << std::endl;
   gp << "set datafile separator '" << m_params.delimiter << "'" << std::endl;
   gp << "set key autotitle columnhead" << std::endl;
   gp << "header = '" << m_params.legend << "' " << std::endl;
   gp << "N = words(header)" << std::endl;
   gp << "set xtics ('' 1)" << std::endl;
   gp << "set for [i=1:N] xtics add (word(header, i) i)" << std::endl;
   gp << "set style data boxplot" << std::endl;
   gp << "unset key" << std::endl;
   gp << _PLOT
      << "for [i=1:N] file using (i):i"
      << std::endl;
}

template <typename T>
void Gplot<T>::splitStr(
    std::string str,
    const std::string &delim,
    std::vector<std::string> &vecstr)
{
   auto start = 0U;
   auto end = str.find(delim);
   while (end != std::string::npos)
   {
      vecstr.push_back(str.substr(start, end - start));
      start = end + delim.length();
      end = str.find(delim, start);
   }
   vecstr.push_back(str.substr(start, end));
}

template <typename T>
void Gplot<T>::setLegendParams(T kx, T ky, T kdy)
{
   gp << "labels=" << SQ << m_params.legend << SQ << std::endl;
   gp << _SET << "samples words(labels)" << std::endl;
   gp << "kx=" << kx << std::endl;
   gp << "ky=" << ky << std::endl;
   gp << "kdy=" << kdy << std::endl;
}

template <typename T>
void Gplot<T>::initPng(void)
{
   gp << boost::format(SET_TERMINAL_PNG) % m_params.width % m_params.height << std::endl;
   gp << boost::format(SET_OUTPUT_FILENAME_FMT) % m_params.filename << std::endl;
   gp << "unset key" << std::endl;
   gp << UNSET_COLORBOX << std::endl;
   setDefaultFontsSizes(12, 9, 9, 7);
   setDefaultPalette();
   gp << "NO_ANIMATION = 1" << std::endl;
}

template <typename T>
void Gplot<T>::resetSession()
{
   gp << "reset session" << std::endl;
}

template <typename T>
void Gplot<T>::setDefaultFontsSizes(
    const us_t &title,
    const us_t &xlabel,
    const us_t &ylabel,
    const us_t &tics)
{
   gp << _SET << "title" << _FONT_ << SQ DEFAULT_FONT COMA
      << title << SQ SMC
      << _SET << "xlabel" << _FONT_ << SQ DEFAULT_FONT COMA
      << xlabel << SQ SMC
      << _SET << "ylabel" << _FONT_ << SQ DEFAULT_FONT COMA
      << ylabel << SQ SMC
      << _SET << "tics" << _FONT_ << SQ DEFAULT_FONT COMA
      << tics << SQ
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
   gp << _SET << "title " << SQ << m_params.title << SQ << std::endl;
}

template class Gplot<double>;
template class Gplot<float>;
template class Gplot<int>;