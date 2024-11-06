#include "ImageProvider.h"

QImageProvider::QImageProvider() : QQuickImageProvider(QQmlImageProviderBase::ImageType::Image)
{}

QImageProvider::~QImageProvider() = default;

QImage QImageProvider::requestImage(const QString & id, QSize * size, const QSize & requestedSize)
{
  Q_UNUSED(id);
  Q_UNUSED(requestedSize);

  if (isImageOpened())
  {
    *size = m_Image->size();
    return *m_Image;
  }

  return QImage();
}

bool QImageProvider::isImageOpened() const
{
  return m_Image && !m_Image->isNull();
}

bool QImageProvider::openImage(const QString & fileName)
{
  if (isImageOpened())
    closeImage();

  m_Image = std::make_unique<QImage>(fileName);
  if (m_Image->isNull())
    return false;

  fillImageInfo(fileName);
  emit imageOpened(m_Image->width(), m_Image->height());
  return true;
}

bool QImageProvider::saveImage() const
{
  return saveImageAs(m_ImageInfo.name);
}

bool QImageProvider::saveImageAs(const QString & fileName) const
{
  const bool isSaved = m_Image->save(fileName, nullptr);
  if (isSaved)
    emit const_cast<QImageProvider*>(this)->imageSaved(fileName);

  return isSaved;
}

void QImageProvider::closeImage()
{
  m_Image.reset();
  clearImageInfo();
  emit imageClosed();
}

const QVector<QRgb> & QImageProvider::getImagePixels() const
{
  return m_ImageInfo.pixels;
}

QSize QImageProvider::getImageSize() const
{
  return isImageOpened() ? m_Image->size() : QSize();
}

void QImageProvider::loadImageFromData(const QVector<QRgb> & rawData)
{
  Q_ASSERT(isImageOpened());

  memcpy(m_Image->bits(), rawData.data(), m_Image->sizeInBytes());
  onImageEdited();
}

void QImageProvider::loadImageFromData(QVector<QRgb> && rawData)
{
  Q_ASSERT(isImageOpened());

  memmove(m_Image->bits(), rawData.data(), m_Image->sizeInBytes());
  onImageEdited();
}

void QImageProvider::mirrorImage(Qt::Orientation orientation)
{
  Q_ASSERT(isImageOpened());

  if (orientation == Qt::Orientation::Vertical)
    m_Image->mirror(false, true);
  else if (orientation == Qt::Orientation::Horizontal)
    m_Image->mirror(true, false);
  else
    Q_ASSERT(false);

  onImageEdited();
}

void QImageProvider::invert()
{
  Q_ASSERT(isImageOpened());

  m_Image->invertPixels();
  onImageEdited();
}

void QImageProvider::onImageEdited()
{
  updateImageInfo();
  emit imageEdited();
}

void QImageProvider::fillImageInfo(const QString & fileName)
{
  m_ImageInfo.name = fileName;

  m_ImageInfo.pixels.resize(m_Image->size().width() * m_Image->size().height());
  memcpy(m_ImageInfo.pixels.data(), m_Image->bits(), m_Image->sizeInBytes());
}

void QImageProvider::updateImageInfo()
{
  m_ImageInfo.pixels.resize(m_Image->size().width() * m_Image->size().height());
  memcpy(m_ImageInfo.pixels.data(), m_Image->bits(), m_Image->sizeInBytes());
}

void QImageProvider::clearImageInfo()
{
  m_ImageInfo.pixels.clear();
  m_ImageInfo.name.clear();
}
