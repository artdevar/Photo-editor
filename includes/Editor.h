#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include "CommandsHistory.h"

class QImageProvider;
class Command;

class QEditor : public QObject
{
  Q_OBJECT

signals:

  void commandExecuted(const QString & description);

public:

  QEditor(QImageProvider * imageProvider, QObject * parent = nullptr);
  ~QEditor();

  QImageProvider * getImageProvider() const;

  bool openImage();
  bool closeImage();
  bool saveImage();
  bool saveImageAs();
  bool isImageOpened() const;
  bool isImageDirty() const;

  bool undo();
  bool redo();

  Q_INVOKABLE bool isAllowedToCloseEditor();

  Q_INVOKABLE void executeInstantCommand(Command * command);
  Q_INVOKABLE void executeIntermediateCommand(Command * command);
  Q_INVOKABLE void onIntermediateCommandExecuted(Command * command);

protected:

  void onImageOpened(int width, int height);
  void onImageSaved(const QString & fileName);
  void onImageClosed();

  bool tryToSaveOrCancel();
  bool eventFilter(QObject * _Object, QEvent * _Event) override;

protected:

  QImageProvider * m_ImageProvider;
  CommandsHistory  m_CommandsHistory;
  CommandsHistory::CommandId m_ImageSaveCommandstamp;
};

#endif // EDITOR_H
