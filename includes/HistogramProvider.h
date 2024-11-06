#ifndef HISTOGRAMPROVIDER_H
#define HISTOGRAMPROVIDER_H

#include <QRgb>
#include <QVector>
#include <QVarLengthArray>

class HistogramProvider
{
public:

  static constexpr inline int HISTOGRAM_DATA_SIZE = 256;

  using HistogramArray = QVarLengthArray<int32_t, HISTOGRAM_DATA_SIZE>;

  static HistogramArray histogramRGB(const QVector<QRgb> & imagePixels);
  static HistogramArray histogramRed(const QVector<QRgb> & imagePixels);
  static HistogramArray histogramGreen(const QVector<QRgb> & imagePixels);
  static HistogramArray histogramBlue(const QVector<QRgb> & imagePixels);


};

#endif // HISTOGRAMPROVIDER_H
