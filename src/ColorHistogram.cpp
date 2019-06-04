#include "ColorHistogram.h"
#include "HistogramKeeper.h"
#include <QBrush>
#include <QPainter>
#include <QTransform>

ColorHistogram::ColorHistogram(QQuickItem * _parent_ptr) : QQuickPaintedItem(_parent_ptr)
{}

void ColorHistogram::paint(QPainter * _painter_ptr)
{
    typedef std::unique_ptr<int32_t[]>(*Histogram)();
    static const Histogram callbacks[] = {
        &HistogramKeeper::histogramRGB,
        &HistogramKeeper::histogramRed,
        &HistogramKeeper::histogramGreen,
        &HistogramKeeper::histogramBlue
    };

    const auto histogram = (*callbacks[currentChannel])();
    const auto arraySize = HistogramKeeper::HISTOGRAM_DATA_SIZE;
    const auto maxItem   = *std::max_element(histogram.get(), histogram.get() + arraySize);
    const auto point     = maxItem == 0 ? 1 : 1.0f / float(maxItem / histogramHeight);

    QPolygon points;
    points.reserve(arraySize + 3);
    int x = 0;
    for (int ix = 0, columnThickness = 2; ix < arraySize; ++ix, x += columnThickness)
    {
        points.push_back({x, (int32_t)(histogram[ix] * point)});
    }
    /* makes connect between last and first point */
    points.push_back({x, (int32_t)(histogram[arraySize - 1] * point)});
    points.push_back({x, 0});
    points.push_back({0, 0});
    /* ------------------------------------------ */
    const QBrush brush(QColor(QStringLiteral("#D1D1D1")));
    _painter_ptr->setBrush(brush);
    _painter_ptr->setRenderHint(QPainter::Antialiasing);
    _painter_ptr->setPen(Qt::NoPen);
    _painter_ptr->translate(boundingRect().bottomLeft());
    _painter_ptr->scale(1.0, -1.0);
    _painter_ptr->drawPolygon(points);
}

ColorHistogram::ColorChannel ColorHistogram::channel() const
{
    return currentChannel;
}

void ColorHistogram::setChannel(int _channel)
{
    auto channel = static_cast<ColorHistogram::ColorChannel>(_channel);
    if (channel != currentChannel)
    {
        currentChannel = channel;
        emit channelChanged();
    }
}