#include "FileDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTranslator>
#include <QCoreApplication>

QString FileDialog::openFileName()
{
  return QFileDialog::getOpenFileName(nullptr, tr("Open image"), QString(), tr("Image files (*.jpg *.jpeg *.png *.bmp)"));
}

QString FileDialog::safeFileName()
{
  return QFileDialog::getSaveFileName(nullptr, tr("Save image"), QString(), tr("Image files (*.jpg *.jpeg *.png *.bmp)"));
}

TSaveImageResponse FileDialog::askToSave()
{
  QMessageBox::StandardButton response = QMessageBox::question(
    nullptr,
    QCoreApplication::applicationName(),
    tr("The image has been modified.\nDo you want to save your changes?"),
    QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
    QMessageBox::Cancel
  );

  switch (response)
  {
  case QMessageBox::Yes:
    return TSaveImageResponse::Save;
  case QMessageBox::No:
    return TSaveImageResponse::DontSave;
  case QMessageBox::Cancel:
    return TSaveImageResponse::Cancel;

  default:
    Q_ASSERT(false);
    return TSaveImageResponse::Cancel;
  }
}
