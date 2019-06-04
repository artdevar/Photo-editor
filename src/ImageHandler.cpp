#include "ImageHandler.h"
#include <thread>

static const int IMAGE_QUALITY = 100;

// class is contained current image
ImageHandler::ImageHandler() : QObject(), QQuickImageProvider(QQmlImageProviderBase::ImageType::Image)
{}

ImageHandler * ImageHandler::instance()
{
    // ImageProvider should be created dynamically
    // QQmlApplicationEngine cares about freeing the memory
    static ImageHandler * instance_ptr = new ImageHandler;
    return instance_ptr;
}

// returns pointer to current image
QImage * ImageHandler::currentImage()
{
    return image_ptr.get();
}

ImageHandler::Memento ImageHandler::createMemento() const
{
    return ImageHandler::Memento(currentImageInfo); // makes copy of image info
}

void ImageHandler::reinstateMemento(ImageHandler::Memento && _memento)
{
    if (_memento.state.width != image_ptr->width() || _memento.state.height != image_ptr->height())
    {
        image_ptr.reset(new QImage(_memento.state.width, _memento.state.height, QImage::Format_RGB32));
    }
    currentImageInfo = _memento.state;
    memcpy(image_ptr->bits(), currentImageInfo.pixels.data(), currentImageInfo.sizeInBytes);
}

void ImageHandler::updateImageInfo()
{
    memcpy(currentImageInfo.pixels.data(), image_ptr->bits(), currentImageInfo.sizeInBytes);
}

// reimp
QImage ImageHandler::requestImage(const QString & _imageName, QSize * _actualSize, const QSize & _requestedSize)
{
    Q_UNUSED(_imageName);
    Q_UNUSED(_requestedSize);

    if (isImageOpened())
    {
        *_actualSize = image_ptr->size();
        return *image_ptr;
    }
    return QImage();
}

// sets new image
void ImageHandler::setImage(const QString & _imageName)
{
    image_ptr.reset(new QImage(_imageName));
    if (isImageOpened())
    {
        std::thread t(&ImageHandler::fillImageInfo, this, _imageName);
        emit imageSetted();
        t.join();
    }
}

void ImageHandler::resetImage(QImage * _image_ptr)
{
    // emitting a signal is redundant here. method for image corrector
    image_ptr.reset(_image_ptr);
}

// saves image
void ImageHandler::saveImage() const
{
    image_ptr->save(currentImageInfo.name, nullptr, IMAGE_QUALITY);
}

void ImageHandler::saveImageAs(const QString & _fileName) const
{
    image_ptr->save(_fileName, nullptr, IMAGE_QUALITY);
}

ImageHandler::ImageInfo * ImageHandler::imageInfo()
{
    return &currentImageInfo;
}

void ImageHandler::fillImageInfo(const QString _name)
{
    const int width  = image_ptr->width();
    const int height = image_ptr->height();
    const uint32_t imageSize   = width * height;
    const uint64_t sizeInBytes = imageSize * sizeof(QRgb);

    currentImageInfo.width       = width;
    currentImageInfo.height      = height;
    currentImageInfo.size        = imageSize;
    currentImageInfo.sizeInBytes = sizeInBytes;
    currentImageInfo.name        = _name;
    currentImageInfo.imageEdited = false;
    currentImageInfo.pixels.resize(imageSize);
    memcpy(currentImageInfo.pixels.data(), image_ptr->bits(), sizeInBytes);
}

// removes current image
void ImageHandler::removeImage()
{
    image_ptr.reset();
    emit imageRemoved();
}
