#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <memory>

class ImageHandler : public QObject, public QQuickImageProvider
{
    Q_OBJECT

signals:
    void imageSetted();
    void imageRemoved();

private:
    struct ImageInfo {
        QVector<QRgb> pixels;
        int           width;
        int           height;
        uint32_t      size;
        uint64_t      sizeInBytes;
        QString       name;
        bool          imageEdited;
    };
public:
    class Memento {
        friend class ImageHandler;
        const ImageHandler::ImageInfo state;
    public:
        Memento(const ImageHandler::ImageInfo & _snapshot) : state(_snapshot) {}
    };
public:
    static ImageHandler * instance();

    void setImage(const QString &);
    void resetImage(QImage *);
    void removeImage();
    void saveImage() const;
    void saveImageAs(const QString &) const;

    void updateImageInfo();
    ImageInfo * imageInfo();
    QImage * currentImage();
    inline bool isImageOpened() const { return image_ptr && !image_ptr->isNull(); }

    Memento createMemento() const;
    void reinstateMemento(Memento &&);

private:
    ImageHandler();
    QImage requestImage(const QString &, QSize *, const QSize &) override;
    void fillImageInfo(const QString);

private:
    ImageInfo currentImageInfo;
    std::unique_ptr<QImage> image_ptr;
};

#endif // IMAGEHANDLER_H