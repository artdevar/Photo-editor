#include "Editor.h"
#include "ImageProvider.h"
#include "Commands.h"
#include "CommandsHistory.h"
#include "FileDialog.h"
#include <QKeyEvent>
#include <QApplication>
#include <QList>

QEditor::QEditor(QImageProvider * imageProvider, QObject * parent) :
  QObject(parent),
  m_ImageProvider(imageProvider),
  m_CommandsHistory(this),
  m_ImageSaveCommandstamp()
{
  connect(imageProvider, &QImageProvider::imageOpened, this, &QEditor::onImageOpened);
  connect(imageProvider, &QImageProvider::imageSaved,  this, &QEditor::onImageSaved);
  connect(imageProvider, &QImageProvider::imageClosed, this, &QEditor::onImageClosed);

  qApp->installEventFilter(this);
}

QEditor::~QEditor() = default;

QImageProvider *QEditor::getImageProvider() const
{
  return m_ImageProvider;
}

bool QEditor::openImage()
{
  if (tryToSaveOrCancel())
  {
    const QString fileName = FileDialog::openFileName();
    if (!fileName.isEmpty())
      return m_ImageProvider->openImage(fileName);
  }

  return false;
}

bool QEditor::closeImage()
{
  if (tryToSaveOrCancel())
  {
    m_ImageProvider->closeImage();
    return true;
  }

  return false;
}

bool QEditor::saveImage()
{
  return m_ImageProvider->saveImage();
}

bool QEditor::saveImageAs()
{
  const QString fileName = FileDialog::safeFileName();

  if (!fileName.isEmpty())
    return m_ImageProvider->saveImageAs(fileName);

  return false;
}

bool QEditor::isImageOpened() const
{
  return m_ImageProvider->isImageOpened();
}

bool QEditor::isImageDirty() const
{
  return isImageOpened() && m_ImageSaveCommandstamp != m_CommandsHistory.getLastExecutedCommandId();
}

bool QEditor::undo()
{
  if (m_CommandsHistory.isUndoPossible())
  {
    m_CommandsHistory.undo();
    return true;
  }

  return false;
}

bool QEditor::redo()
{
  if (m_CommandsHistory.isRedoPossible())
  {
    m_CommandsHistory.redo();
    return true;
  }

  return false;
}

bool QEditor::isAllowedToCloseEditor()
{
  return tryToSaveOrCancel();
}

void QEditor::executeInstantCommand(Command * command)
{
  const bool isExecuted = command->execute();
  if (isExecuted)
  {
    if (command->isSavable())
      m_CommandsHistory.push(command);

    emit commandExecuted(command->getDescription());
  }
}

void QEditor::executeIntermediateCommand(Command * command)
{
  command->execute();
}

void QEditor::onIntermediateCommandExecuted(Command * command)
{
  if (command->isSavable())
    m_CommandsHistory.push(command);

  emit commandExecuted(command->getDescription());
}

void QEditor::onImageOpened(int width, int height)
{
  Q_UNUSED(width);
  Q_UNUSED(height);

  m_ImageSaveCommandstamp = m_CommandsHistory.getLastExecutedCommandId();
}

void QEditor::onImageSaved(const QString & fileName)
{
  Q_UNUSED(fileName);

  m_ImageSaveCommandstamp = m_CommandsHistory.getLastExecutedCommandId();
}

void QEditor::onImageClosed()
{
  m_CommandsHistory.clear();
}

bool QEditor::tryToSaveOrCancel()
{
  bool notCanceled = true;

  if (isImageDirty())
  {
    switch (FileDialog::askToSave())
    {
    case TSaveImageResponse::Save:
      saveImageAs();
      break;

    case TSaveImageResponse::DontSave:
      break;

    case TSaveImageResponse::Cancel:
      notCanceled = false;
      break;
    }
  }

  return notCanceled;
}

bool QEditor::eventFilter(QObject * _object, QEvent * _event)
{
  return QObject::eventFilter(_object, _event);
}
