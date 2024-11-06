#ifndef COMMANDS_H
#define COMMANDS_H

#include <QObject>
#include <QQmlEngine>
#include <QList>
#include <QRgb>
#include <QSize>
#include <QString>
#include "Editor.h" // the type need to be fully defined

class Command : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QEditor * editorPtr MEMBER m_Editor WRITE setEditor)

public:

  Command(QObject * parent = nullptr);

  virtual ~Command();

  Q_INVOKABLE virtual bool execute() = 0;
  Q_INVOKABLE virtual void cancel() = 0;
              virtual void setEditor(QEditor * editor);

  bool isSavable() const;
  const QString & getDescription() const;

protected:

  QEditor * m_Editor;
  QString   m_Description;
  bool      m_IsSavable;
};

class CommandImageOpen : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandImageOpen(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};

class CommandImageSave : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandImageSave(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};

class CommandImageSaveAs : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandImageSaveAs(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};

class CommandImageClose : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandImageClose(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};

class CommandChangeBrightnessContrast : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandChangeBrightnessContrast(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
  void setEditor(QEditor * editor) override;

  Q_INVOKABLE void setBrightnessContrast(int brightness, float contrast);

protected:

  QVector<QRgb> m_ImagePixels;

  int   m_Brightness;
  float m_Contrast;
};

class CommandChangeHueSaturationLight : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandChangeHueSaturationLight(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
  void setEditor(QEditor * editor) override;

  Q_INVOKABLE void setHueSaturationLightness(int hue, int saturation, int lightness);

protected:

  QVector<QRgb> m_ImagePixels;

  float m_Hue;
  float m_Saturation;
  float m_Lightness;
};

class CommandAutoContrast : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandAutoContrast(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
  void setEditor(QEditor * editor) override;

protected:

  QVector<QRgb> m_ImagePixels;
  QSize         m_ImageSize;
};

class CommandGrayscale : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandGrayscale(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
  void setEditor(QEditor * editor) override;

protected:

  QVector<QRgb> m_ImagePixels;
};

class CommandFlipImageVertical : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandFlipImageVertical(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};

class CommandFlipImageHorizontal : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandFlipImageHorizontal(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};

class CommandInvertPixels : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandInvertPixels(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};

class CommandUndo : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandUndo(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};

class CommandRedo : public Command
{
  Q_OBJECT
  QML_ELEMENT

public:

  CommandRedo(QObject * parent = nullptr);

  bool execute() override;
  void cancel() override;
};


#endif // COMMANDS_H
