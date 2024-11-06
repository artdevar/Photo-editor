#include "HistogramProvider.h"
#include <algorithm>
#include <cstring>

HistogramProvider::HistogramArray HistogramProvider::histogramRGB(const QVector<QRgb> & imagePixels)
{
  HistogramArray histogram(HISTOGRAM_DATA_SIZE, 0);

  const float modifier = 1.0f / 3.0f;
  std::for_each(imagePixels.cbegin(), imagePixels.cend(), [&histogram, modifier](QRgb rgb) {
    ++histogram[(int)((qRed(rgb) + qGreen(rgb) + qBlue(rgb)) * modifier)];
  });

  return histogram;
}

HistogramProvider::HistogramArray HistogramProvider::histogramRed(const QVector<QRgb> & imagePixels)
{
  HistogramArray histogram(HISTOGRAM_DATA_SIZE, 0);

  std::for_each(imagePixels.cbegin(), imagePixels.cend(), [&histogram](QRgb rgb) {
    ++histogram[qRed(rgb)];
  });

  return histogram;
}

HistogramProvider::HistogramArray HistogramProvider::histogramGreen(const QVector<QRgb> & imagePixels)
{
  HistogramArray histogram(HISTOGRAM_DATA_SIZE, 0);

  std::for_each(imagePixels.cbegin(), imagePixels.cend(), [&histogram](QRgb rgb) {
    ++histogram[qGreen(rgb)];
  });

  return histogram;
}

HistogramProvider::HistogramArray HistogramProvider::histogramBlue(const QVector<QRgb> & imagePixels)
{
  HistogramArray histogram(HISTOGRAM_DATA_SIZE, 0);

  std::for_each(imagePixels.cbegin(), imagePixels.cend(), [&histogram](QRgb rgb) {
    ++histogram[qBlue(rgb)];
  });

  return histogram;
}
