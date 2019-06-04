#ifndef HISTOGRAMKEEPER_H
#define HISTOGRAMKEEPER_H

#include <memory>

class HistogramKeeper
{
public:
    static std::unique_ptr<int32_t[]> histogramRGB();
    static std::unique_ptr<int32_t[]> histogramRed();
    static std::unique_ptr<int32_t[]> histogramGreen();
    static std::unique_ptr<int32_t[]> histogramBlue();

    enum { HISTOGRAM_DATA_SIZE = 256 };
};

#endif // HISTOGRAMKEEPER_H
