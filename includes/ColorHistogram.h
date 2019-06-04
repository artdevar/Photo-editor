#ifndef COLORHISTOGRAM_H
#define COLORHISTOGRAM_H

#include <QQuickPaintedItem>

class ColorHistogram : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(ColorChannel currentChannel  READ channel           WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(    int      histogramHeight MEMBER histogramHeight CONSTANT)
    Q_PROPERTY(    int      histogramWidth  MEMBER histogramWidth  CONSTANT)

signals:
    void channelChanged();

public:
    enum ColorChannel { RGB, RED, GREEN, BLUE };
    Q_ENUM(ColorChannel)

    ColorHistogram(QQuickItem * = nullptr);
    void paint(QPainter*) override;

    ColorChannel channel() const;
    void setChannel(int);

private:
    ColorChannel currentChannel = RGB;
    const int   histogramHeight = 340;
    const int   histogramWidth  = 512;
};

#endif // COLORHISTOGRAM_H
