#include "ColorHistogram.h"
#include <QBrush>
#include <QPainter>
#include <QTransform>

ColorHistogram::ColorHistogram(QQuickItem * parent) : QQuickPaintedItem(parent)
{}

void ColorHistogram::paint(QPainter * painter)
{
  const HistogramProvider::HistogramArray & histogram = m_Histograms.value(getChannel());

  const qsizetype histogramSize = histogram.size();
  const int32_t   maxItem       = *std::max_element(histogram.cbegin(), histogram.cend());
  const float     point         = maxItem == 0 ? 1.0f : 1.0f / float(maxItem / HISTOGRAM_HEIGHT);

  QPolygon points;
  points.reserve(histogramSize + 3);
  int x = 0;
  for (int ix = 0, columnThickness = 2; ix < histogramSize; ++ix, x += columnThickness)
  {
    points.push_back({x, (int32_t)(histogram[ix] * point)});
  }
  /* makes connect between last and first point */
  points.push_back({x, (int32_t)(histogram[histogramSize - 1] * point)});
  points.push_back({x, 0});
  points.push_back({0, 0});

  const QBrush brush(QColor(QStringLiteral("#D1D1D1")));
  painter->setBrush(brush);
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(Qt::NoPen);
  painter->translate(boundingRect().bottomLeft());
  painter->scale(1.0, -1.0);
  painter->drawPolygon(points);
}

ColorHistogram::ColorChannel ColorHistogram::getChannel() const
{
  return m_Channel;
}

void ColorHistogram::setChannel(int channel)
{
  auto newChannel = static_cast<ColorHistogram::ColorChannel>(channel);
  if (newChannel != m_Channel)
  {
    m_Channel = newChannel;
    emit channelChanged();
  }
}

void ColorHistogram::setImageData(const QVector<QRgb> & data)
{
  m_ImageData = data;
  calculateHistograms();
}

void ColorHistogram::calculateHistograms()
{
  m_Histograms.insert(ColorChannel::RGB,   HistogramProvider::histogramRGB(m_ImageData));
  m_Histograms.insert(ColorChannel::RED,   HistogramProvider::histogramRed(m_ImageData));
  m_Histograms.insert(ColorChannel::GREEN, HistogramProvider::histogramGreen(m_ImageData));
  m_Histograms.insert(ColorChannel::BLUE,  HistogramProvider::histogramBlue(m_ImageData));
}
