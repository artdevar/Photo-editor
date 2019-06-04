#ifndef IMAGECORRECTOR_H
#define IMAGECORRECTOR_H

#include "ImageHandler.h"
#include "Caretaker.h"
#include <QObject>
#include <map>

class ImageCorrector : public QObject
{
    Q_OBJECT

signals:
    void imageEdited();

public:
    enum class Options {
        OPTION_BRIGHTNESS_CONTRAST,
        OPTION_HUE_SATURATION,
        OPTION_GAUSSIAN_BLUR,
        OPTION_AUTO_CONTRAST,
        OPTION_FLIP_VERTICAL,
        OPTION_FLIP_HORIZONTAL,
        OPTION_TRANSPOSE,
        OPTION_INVERT_PIXELS,
        OPTION_GRAYSCALE,
        OPTION_SEPIA,
        OPTION_UNDO
    };

    static ImageCorrector * instance();
    void editImage(Options, QVariant&&);
    void saveState();

private:
    ImageCorrector(QObject * = nullptr);

    void handlerChangeBrightnessContrast(QVariant&&);
    void handlerChangeHueSaturation(QVariant&&);
    void handlerGaussianBlur(QVariant&&);
    void handlerAutoContrast(QVariant&&);
    void handlerFlipVertical(QVariant&&);
    void handlerFlipHorizontal(QVariant&&);
    void handlerTranspose(QVariant&&);
    void handlerInvertPixels(QVariant&&);
    void handlerGrayscale(QVariant&&);
    void handlerSepia(QVariant&&);
    void handlerUndoOperation(QVariant&&);

private:
    ImageHandler * const handler_ptr;
    Caretaker<ImageHandler, ImageHandler::Memento> caretaker;
    std::map<ImageCorrector::Options, void(ImageCorrector::*)(QVariant&&)> callbacks;
    bool imageWasEdited;
};

#endif // IMAGECORRECTOR_H