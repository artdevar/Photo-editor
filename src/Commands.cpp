#include "Commands.h"
#include "Editor.h"
#include "ImageProvider.h"
#include "Converter.h"
#include "HistogramProvider.h"

Command::Command(QObject * parent) :
  QObject(parent),
  m_Editor(nullptr),
  m_Description(),
  m_IsSavable(false)
{
  // Empty
}

Command::~Command() = default;

void Command::setEditor(QEditor * editor)
{
  m_Editor = editor;
}

const QString & Command::getDescription() const
{
  return m_Description;
}

bool Command::isSavable() const
{
  return m_IsSavable;
}

CommandImageOpen::CommandImageOpen(QObject * parent) : Command(parent)
{
  m_IsSavable = false;
}

bool CommandImageOpen::execute()
{
  return m_Editor->openImage();
}

void CommandImageOpen::cancel()
{
  // Empty
}

CommandImageSave::CommandImageSave(QObject * parent) : Command(parent)
{
  m_Description = tr("Image saved");
  m_IsSavable = false;
}

bool CommandImageSave::execute()
{
  return m_Editor->saveImage();
}

void CommandImageSave::cancel()
{
  // Empty
}

CommandImageSaveAs::CommandImageSaveAs(QObject * parent) : Command(parent)
{
  m_Description = tr("Image saved");
  m_IsSavable = false;
}

bool CommandImageSaveAs::execute()
{
  return m_Editor->saveImageAs();
}

void CommandImageSaveAs::cancel()
{
  // Empty
}

CommandImageClose::CommandImageClose(QObject * parent) : Command(parent)
{
  m_IsSavable = false;
}

bool CommandImageClose::execute()
{
  return m_Editor->closeImage();
}

void CommandImageClose::cancel()
{
  // Empty
}

CommandChangeBrightnessContrast::CommandChangeBrightnessContrast(QObject * parent) :
  Command(parent),
  m_Brightness(0),
  m_Contrast(0.0f)
{
  m_Description = tr("Brightness/Contrast has been changed");
  m_IsSavable   = true;
}

void CommandChangeBrightnessContrast::setEditor(QEditor * editor)
{
  Command::setEditor(editor);

  m_ImagePixels = editor->getImageProvider()->getImagePixels();
}

void CommandChangeBrightnessContrast::setBrightnessContrast(int brightness, float contrast)
{
  m_Brightness = brightness;
  m_Contrast   = contrast;
}

bool CommandChangeBrightnessContrast::execute()
{
  QVector<QRgb> imagePixels = m_ImagePixels;

  std::for_each(imagePixels.begin(), imagePixels.end(), [this](QRgb & rgb) {
    rgb = qRgb(SAFE_RGB_BORDERS((int)(128 + m_Contrast * (qRed(rgb)   - 128)) + m_Brightness),
               SAFE_RGB_BORDERS((int)(128 + m_Contrast * (qGreen(rgb) - 128)) + m_Brightness),
               SAFE_RGB_BORDERS((int)(128 + m_Contrast * (qBlue(rgb)  - 128)) + m_Brightness));
  });

  m_Editor->getImageProvider()->loadImageFromData(std::move(imagePixels));

  return !imagePixels.isEmpty();
}

void CommandChangeBrightnessContrast::cancel()
{
  const QVector<QRgb> imagePixels = m_ImagePixels;
  m_Editor->getImageProvider()->loadImageFromData(imagePixels);
}

CommandChangeHueSaturationLight::CommandChangeHueSaturationLight(QObject *parent) :
  Command(parent),
  m_Hue(0.0f),
  m_Saturation(0.0f),
  m_Lightness(0.0f)
{
  m_Description = tr("Hue/Saturation/Lightness has been changed");
  m_IsSavable   = true;
}

bool CommandChangeHueSaturationLight::execute()
{
  QVector<QRgb> imagePixels = m_ImagePixels;

  std::for_each(imagePixels.begin(), imagePixels.end(), [this](QRgb & rgb) {
    QColor color(rgb);

    float h, s, v;
    color.getHsvF(&h, &s, &v);

    h += m_Hue;
    s += m_Saturation;
    v += m_Lightness;

    color.setHsvF(SAFE_HSVF_BORDERS(h), SAFE_HSVF_BORDERS(s), SAFE_HSVF_BORDERS(v));
    rgb = color.rgb();
  });

  m_Editor->getImageProvider()->loadImageFromData(std::move(imagePixels));

  return !imagePixels.isEmpty();
}

void CommandChangeHueSaturationLight::cancel()
{
  const QVector<QRgb> imagePixels = m_ImagePixels;
  m_Editor->getImageProvider()->loadImageFromData(imagePixels);
}

void CommandChangeHueSaturationLight::setEditor(QEditor * editor)
{
  Command::setEditor(editor);

  m_ImagePixels = editor->getImageProvider()->getImagePixels();
}

void CommandChangeHueSaturationLight::setHueSaturationLightness(int hue, int saturation, int lightness)
{
  m_Hue        = hue        / 360.0f;
  m_Saturation = saturation * 0.01f;
  m_Lightness  = lightness  * 0.01f;
}

CommandAutoContrast::CommandAutoContrast(QObject * parent) : Command(parent)
{
  m_Description = tr("Auto constrast has been applied");
  m_IsSavable   = true;
}

void CommandAutoContrast::setEditor(QEditor * editor)
{
  Command::setEditor(editor);

  QImageProvider * provider = editor->getImageProvider();

  m_ImagePixels = provider->getImagePixels();
  m_ImageSize   = provider->getImageSize();
}

bool CommandAutoContrast::execute()
{
  QVector<QRgb> imagePixels = m_ImagePixels;

  const HistogramProvider::HistogramArray histogram = HistogramProvider::histogramRGB(imagePixels);
  float lut[HistogramProvider::HISTOGRAM_DATA_SIZE];
  int32_t sum = 0;
  const float scaleFactor = (255.0f / float(m_ImageSize.width() * m_ImageSize.height()));

  for (int ix = 0; ix < HistogramProvider::HISTOGRAM_DATA_SIZE; ++ix)
  {
    sum += histogram[ix];
    lut[ix] = float(sum) * scaleFactor;
  }

  std::for_each(imagePixels.begin(), imagePixels.end(), [&lut](QRgb & rgb) {
    rgb = qRgb(lut[qRed(rgb)], lut[qGreen(rgb)], lut[qBlue(rgb)]);
  });

  m_Editor->getImageProvider()->loadImageFromData(std::move(imagePixels));

  return !imagePixels.isEmpty();
}

void CommandAutoContrast::cancel()
{
  const QVector<QRgb> imagePixels = m_ImagePixels;
  m_Editor->getImageProvider()->loadImageFromData(imagePixels);
}

CommandGrayscale::CommandGrayscale(QObject * parent) : Command(parent)
{
  m_Description = tr("Grayscale is applied");
  m_IsSavable   = true;
}

bool CommandGrayscale::execute()
{
  QVector<QRgb> imagePixels = m_ImagePixels;

  constexpr float modifier = 1.0f / 3.0f;

  std::for_each(imagePixels.begin(), imagePixels.end(), [](QRgb & rgb) {
    const int val = (int)((qRed(rgb) + qGreen(rgb) + qBlue(rgb)) * modifier);
    rgb = qRgb(val, val, val);
  });

  m_Editor->getImageProvider()->loadImageFromData(std::move(imagePixels));

  return !imagePixels.isEmpty();
}

void CommandGrayscale::cancel()
{
  const QVector<QRgb> imagePixels = m_ImagePixels;
  m_Editor->getImageProvider()->loadImageFromData(imagePixels);
}

void CommandGrayscale::setEditor(QEditor * editor)
{
  Command::setEditor(editor);

  m_ImagePixels = editor->getImageProvider()->getImagePixels();
}

CommandFlipImageVertical::CommandFlipImageVertical(QObject * parent) : Command(parent)
{
  m_Description = tr("Image has been flipped vertically");
  m_IsSavable   = true;
}

bool CommandFlipImageVertical::execute()
{
  m_Editor->getImageProvider()->mirrorImage(Qt::Orientation::Vertical);
  return true;
}

void CommandFlipImageVertical::cancel()
{
  m_Editor->getImageProvider()->mirrorImage(Qt::Orientation::Vertical);
}

CommandFlipImageHorizontal::CommandFlipImageHorizontal(QObject * parent) : Command(parent)
{
  m_Description = tr("Image has been flipped horizontally");
  m_IsSavable   = true;
}

bool CommandFlipImageHorizontal::execute()
{
  m_Editor->getImageProvider()->mirrorImage(Qt::Orientation::Horizontal);
  return true;
}

void CommandFlipImageHorizontal::cancel()
{
  m_Editor->getImageProvider()->mirrorImage(Qt::Orientation::Horizontal);
}

CommandInvertPixels::CommandInvertPixels(QObject * parent) : Command(parent)
{
  m_Description = tr("Image colors have been inverted");
  m_IsSavable = true;
}

bool CommandInvertPixels::execute()
{
  m_Editor->getImageProvider()->invert();
  return true;
}

void CommandInvertPixels::cancel()
{
  m_Editor->getImageProvider()->invert();
}

CommandUndo::CommandUndo(QObject * parent) : Command(parent)
{
  m_Description = tr("Undo has been performed");
  m_IsSavable   = false;
}

bool CommandUndo::execute()
{
  return m_Editor->undo();
}

void CommandUndo::cancel()
{
  // Empty
}

CommandRedo::CommandRedo(QObject * parent) : Command(parent)
{
  m_Description = tr("Redo has been performed");
  m_IsSavable   = false;
}

bool CommandRedo::execute()
{
  return m_Editor->redo();
}

void CommandRedo::cancel()
{
  // Empty
}

