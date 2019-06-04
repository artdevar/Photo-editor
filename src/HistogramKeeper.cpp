#include "HistogramKeeper.h"
#include "ImageHandler.h"
#include <algorithm>

static const size_t HISTOGRAM_DATA_SIZE_BYTES = HistogramKeeper::HISTOGRAM_DATA_SIZE * sizeof(int32_t);

std::unique_ptr<int32_t[]> HistogramKeeper::histogramRGB()
{
    std::unique_ptr<int32_t[]> histogram(std::make_unique<int32_t[]>(HISTOGRAM_DATA_SIZE));
    memset(histogram.get(), 0, HISTOGRAM_DATA_SIZE_BYTES);
    const auto imagePixels = ImageHandler::instance()->imageInfo()->pixels;
    const float modifier = 1.0f / 3.0f;
    std::for_each(imagePixels.cbegin(), imagePixels.cend(), [&histogram, modifier](QRgb rgb) {
        ++histogram[(int)((qRed(rgb) + qGreen(rgb) + qBlue(rgb)) * modifier)];
    });
    return histogram;
}

std::unique_ptr<int32_t[]> HistogramKeeper::histogramRed()
{
    std::unique_ptr<int32_t[]> histogram(std::make_unique<int32_t[]>(HISTOGRAM_DATA_SIZE));
    memset(histogram.get(), 0, HISTOGRAM_DATA_SIZE_BYTES);
    const auto imagePixels = ImageHandler::instance()->imageInfo()->pixels;
    std::for_each(imagePixels.cbegin(), imagePixels.cend(), [&histogram](QRgb rgb) {
        ++histogram[qRed(rgb)];
    });
    return histogram;
}

std::unique_ptr<int32_t[]> HistogramKeeper::histogramGreen()
{
    std::unique_ptr<int32_t[]> histogram(std::make_unique<int32_t[]>(HISTOGRAM_DATA_SIZE));
    memset(histogram.get(), 0, HISTOGRAM_DATA_SIZE_BYTES);
    const auto imagePixels = ImageHandler::instance()->imageInfo()->pixels;
    std::for_each(imagePixels.cbegin(), imagePixels.cend(), [&histogram](QRgb rgb) {
        ++histogram[qGreen(rgb)];
    });
    return histogram;
}

std::unique_ptr<int32_t[]> HistogramKeeper::histogramBlue()
{
    std::unique_ptr<int32_t[]> histogram(std::make_unique<int32_t[]>(HISTOGRAM_DATA_SIZE));
    memset(histogram.get(), 0, HISTOGRAM_DATA_SIZE_BYTES);
    const auto imagePixels = ImageHandler::instance()->imageInfo()->pixels;
    std::for_each(imagePixels.cbegin(), imagePixels.cend(), [&histogram](QRgb rgb) {
        ++histogram[qBlue(rgb)];
    });
    return histogram;
}
