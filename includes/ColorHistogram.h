#ifndef COLORHISTOGRAM_H
#define COLORHISTOGRAM_H

#include <QQuickPaintedItem>
#include <QMap>
#include "HistogramProvider.h"

class ColorHistogram : public QQuickPaintedItem
{
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(ColorChannel  currentChannel  READ   getChannel       WRITE setChannel NOTIFY channelChanged)
  Q_PROPERTY(QVector<QRgb> imageRawData    MEMBER m_ImageData      WRITE setImageData)
  Q_PROPERTY(    int       histogramHeight MEMBER HISTOGRAM_HEIGHT CONSTANT)
  Q_PROPERTY(    int       histogramWidth  MEMBER HISTOGRAM_WIDTH  CONSTANT)

signals:

  void channelChanged();

public:

  enum ColorChannel { RGB, RED, GREEN, BLUE };
  Q_ENUM(ColorChannel)

  ColorHistogram(QQuickItem * parent = nullptr);
  void paint(QPainter * painter) override;

  ColorChannel getChannel() const;
  void setChannel(int channel);

  void setImageData(const QVector<QRgb> & data);

protected:

  void calculateHistograms();

protected:

  static constexpr int HISTOGRAM_WIDTH  = 512;
  static constexpr int HISTOGRAM_HEIGHT = 340;

  ColorChannel m_Channel = RGB;
  QVector<QRgb> m_ImageData;
  QMap<ColorChannel, HistogramProvider::HistogramArray> m_Histograms;
};

#endif // COLORHISTOGRAM_H
