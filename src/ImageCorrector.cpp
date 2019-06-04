#include "ImageCorrector.h"
#include "ImageHandler.h"
#include "HistogramKeeper.h"
#include "Converter.h"
#include <QDebug>

#define MAKE_OPTION_CALLBACK_PAIR \
    std::make_pair<ImageCorrector::Options, void(ImageCorrector::*)(QVariant&&)>

ImageCorrector::ImageCorrector(QObject * _parent_ptr) : QObject(_parent_ptr), handler_ptr(ImageHandler::instance()), caretaker(handler_ptr),
    callbacks{MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_BRIGHTNESS_CONTRAST, &ImageCorrector::handlerChangeBrightnessContrast),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_HUE_SATURATION,      &ImageCorrector::handlerChangeHueSaturation),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_GAUSSIAN_BLUR,       &ImageCorrector::handlerGaussianBlur),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_AUTO_CONTRAST,       &ImageCorrector::handlerAutoContrast),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_FLIP_VERTICAL,       &ImageCorrector::handlerFlipVertical),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_FLIP_HORIZONTAL,     &ImageCorrector::handlerFlipHorizontal),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_TRANSPOSE,           &ImageCorrector::handlerTranspose),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_INVERT_PIXELS,       &ImageCorrector::handlerInvertPixels),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_GRAYSCALE,           &ImageCorrector::handlerGrayscale),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_SEPIA,               &ImageCorrector::handlerSepia),
              MAKE_OPTION_CALLBACK_PAIR(ImageCorrector::Options::OPTION_UNDO,                &ImageCorrector::handlerUndoOperation)}
{
    connect(handler_ptr, &ImageHandler::imageSetted, this, [this] {
        caretaker.eraseSnapshots();
        imageWasEdited = true; // in order to create a snapshot for just opened image
    });

    connect(handler_ptr, &ImageHandler::imageRemoved, this, [this] {
        caretaker.eraseSnapshots();
    });
}

ImageCorrector *ImageCorrector::instance()
{
    static ImageCorrector inst;
    return &inst;
}

void ImageCorrector::editImage(ImageCorrector::Options _option, QVariant&& _value)
{
    std::chrono::time_point<std::chrono::system_clock> started_ = std::chrono::system_clock::now();

    (this->*callbacks[_option])(std::move(_value));
    emit imageEdited();

    std::chrono::time_point<std::chrono::system_clock> now_ = std::chrono::system_clock::now();
    qInfo() << "Algorithm execution time >" << std::chrono::duration_cast<std::chrono::milliseconds>(now_ - started_).count() << "ms";
}

void ImageCorrector::saveState()
{
    if (imageWasEdited)
    {
        caretaker.createSnapshot();
        imageWasEdited = false;
    }
}

void ImageCorrector::handlerChangeBrightnessContrast(QVariant&& _container)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    const auto container = _container.value<QList<float>>();
    const int brightness = static_cast<int>(container[0]);
    const float contrast = container[1];
    QVector<QRgb> pixels(currentImageInfo->pixels);
    std::for_each(pixels.begin(), pixels.end(), [brightness, contrast](QRgb & _rgb) {
        _rgb = qRgb(SAVE_RGB_BORDERS((int)(128 + contrast * (qRed(_rgb)   - 128)) + brightness),
                    SAVE_RGB_BORDERS((int)(128 + contrast * (qGreen(_rgb) - 128)) + brightness),
                    SAVE_RGB_BORDERS((int)(128 + contrast * (qBlue(_rgb)  - 128)) + brightness));
    });
    memmove(handler_ptr->currentImage()->bits(), pixels.data(), currentImageInfo->sizeInBytes);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Brightness/Contrast has been changed";
}

void ImageCorrector::handlerChangeHueSaturation(QVariant&& _container)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    const auto   container = _container.value<QList<int>>();
    const float        hue = container[0] / 360.0f;
    const float saturation = container[1] * 0.01f;
    const float  lightness = container[2] * 0.01f;
    QVector<QRgb> pixels(currentImageInfo->pixels);
    enum {red, green, blue, count};
    enum {Hue, Sat, Light};
    float rgb[count], hsv[count];
    std::for_each(pixels.begin(), pixels.end(), [hue, saturation, lightness, &rgb, &hsv](QRgb & _rgb) {
        Converter::rgbToHsv(qRed(_rgb), qGreen(_rgb), qBlue(_rgb), &hsv[Hue], &hsv[Sat], &hsv[Light]);
        hsv[Hue]   += hue;
        hsv[Sat]   += saturation;
        hsv[Light] += lightness;
        Converter::hsvToRgb(&rgb[red], &rgb[green], &rgb[blue], SAVE_RGBF_BORDERS(hsv[Hue]), SAVE_RGBF_BORDERS(hsv[Sat]), SAVE_RGBF_BORDERS(hsv[Light]));
        _rgb = qRgb(rgb[red], rgb[green], rgb[blue]);
    });
    memmove(handler_ptr->currentImage()->bits(), pixels.data(), currentImageInfo->sizeInBytes);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Hue/Saturation has been changed";
}

void ImageCorrector::handlerGaussianBlur(QVariant&&)
{
    /*auto * const currentImageInfo = handler_ptr->imageInfo();
    const int32_t value = _value.toInt();

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;*/
    qInfo() << "Blur has been changed";
}


void ImageCorrector::handlerAutoContrast(QVariant&&)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    const auto histogram = HistogramKeeper::histogramRGB();
    float lut[HistogramKeeper::HISTOGRAM_DATA_SIZE];
    int32_t sum = 0;
    const float scaleFactor = (255.0f / float(currentImageInfo->size));
    for (auto ix = 0; ix < HistogramKeeper::HISTOGRAM_DATA_SIZE; ++ix)
    {
        sum += histogram[ix];
        lut[ix] = float(sum) * scaleFactor;
    }

    QVector<QRgb> * const pixels_ptr = &currentImageInfo->pixels;
    std::for_each(pixels_ptr->begin(), pixels_ptr->end(), [&lut](QRgb & _rgb) {
        _rgb = qRgb(lut[qRed(_rgb)], lut[qGreen(_rgb)], lut[qBlue(_rgb)]);
    });
    memcpy(handler_ptr->currentImage()->bits(), pixels_ptr->data(), currentImageInfo->sizeInBytes);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Auto contrast has been performed";
}


void ImageCorrector::handlerFlipVertical(QVariant&&)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    QVector<QRgb> & pixels(currentImageInfo->pixels);
    const auto width  = handler_ptr->currentImage()->width();
    const auto height = handler_ptr->currentImage()->height();
    for (int i = 0, j = height - 1; i < j; ++i, --j)
    {
        for (int x = 0; x < width; ++x)
        {
            std::swap(pixels[i * width + x], pixels[j * width + x]);
        }
    }
    memcpy(handler_ptr->currentImage()->bits(), pixels.data(), currentImageInfo->sizeInBytes);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Vertical flip has been performed";
}

void ImageCorrector::handlerFlipHorizontal(QVariant&&)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    QVector<QRgb> & pixels(currentImageInfo->pixels);
    const auto width  = handler_ptr->currentImage()->width();
    const auto height = handler_ptr->currentImage()->height();
    for (int y = 0; y < height; ++y)
    {
        for (int i = 0, j = width - 1; i < j; ++i, --j)
        {
            std::swap(pixels[y * width + i], pixels[y * width + j]);
        }
    }
    memcpy(handler_ptr->currentImage()->bits(), pixels.data(), currentImageInfo->sizeInBytes);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Horizontal flip has been performed";
}

void ImageCorrector::handlerTranspose(QVariant &&)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    const QVector<QRgb> pixels = currentImageInfo->pixels;
    const uint32_t width  = currentImageInfo->width;
    const uint32_t height = currentImageInfo->height;
    const uint32_t imageSize = currentImageInfo->size;
    QImage * newImage = new QImage(height, width, QImage::Format_RGB32);
    handler_ptr->resetImage(newImage);

    for (uint32_t n = 0, i, j; n < imageSize; ++n)
    {
        i = n / height;
        j = n % height;
        currentImageInfo->pixels[n] = pixels[width * j + i];
    }
    memcpy(handler_ptr->currentImage()->bits(), currentImageInfo->pixels.data(), currentImageInfo->sizeInBytes);
    std::swap(currentImageInfo->width, currentImageInfo->height);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Transpose has been performed";
}

void ImageCorrector::handlerInvertPixels(QVariant&&)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    const int maxChannelValue = 255;
    QVector<QRgb> * const pixels_ptr = &currentImageInfo->pixels;
    std::for_each(pixels_ptr->begin(), pixels_ptr->end(), [maxChannelValue](QRgb & _rgb) {
        _rgb = qRgb(maxChannelValue - qRed(_rgb), maxChannelValue - qGreen(_rgb), maxChannelValue - qBlue(_rgb));
    });
    memcpy(handler_ptr->currentImage()->bits(), pixels_ptr->data(), currentImageInfo->sizeInBytes);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Pixels have been inverted";
}

void ImageCorrector::handlerGrayscale(QVariant&&)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    QVector<QRgb> * const pixels_ptr = &currentImageInfo->pixels;
    enum {red, green, blue, count};
    int rgb[count];
    static const float modifier = 1.0f / 3.0f;
    std::for_each(pixels_ptr->begin(), pixels_ptr->end(), [&rgb](QRgb & _rgb) {
          rgb[red] = rgb[green] = rgb[blue] = (int)((qRed(_rgb) + qGreen(_rgb) + qBlue(_rgb)) * modifier);
          _rgb = qRgb(rgb[red], rgb[green], rgb[blue]);
    });
    memcpy(handler_ptr->currentImage()->bits(), pixels_ptr->data(), currentImageInfo->sizeInBytes);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Grayscale has been performed";
}

void ImageCorrector::handlerSepia(QVariant&&)
{
    auto * const currentImageInfo = handler_ptr->imageInfo();
    QVector<QRgb> * const pixels_ptr = &currentImageInfo->pixels;
    std::for_each(pixels_ptr->begin(), pixels_ptr->end(), [](QRgb & _rgb) {
        _rgb = qRgb(SAVE_RGB_MAX_BORDER((qRed(_rgb) * 0.393f) + (qGreen(_rgb) * 0.769f) + (qBlue(_rgb) * 0.189f)),
                    SAVE_RGB_MAX_BORDER((qRed(_rgb) * 0.349f) + (qGreen(_rgb) * 0.686f) + (qBlue(_rgb) * 0.168f)),
                    SAVE_RGB_MAX_BORDER((qRed(_rgb) * 0.272f) + (qGreen(_rgb) * 0.534f) + (qBlue(_rgb) * 0.131f)));
    });
    memcpy(handler_ptr->currentImage()->bits(), pixels_ptr->data(), currentImageInfo->sizeInBytes);

    currentImageInfo->imageEdited = true;
    imageWasEdited = true;
    qInfo() << "Sepia has been performed";
}

void ImageCorrector::handlerUndoOperation(QVariant&&)
{
    caretaker.undoOperation();

    qInfo() << "Undo has been performed";
}