#include "FileDialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCoreApplication>

FileDialog::FileDialog(QObject * parent) : QObject(parent)
{}

QString FileDialog::openFileName()
{
    return QFileDialog::getOpenFileName(nullptr, tr("Open image"), QString(), tr("Image files (*.jpg *.jpeg *.png *.bmp)"));
}

QString FileDialog::safeFileName()
{
    return QFileDialog::getSaveFileName(nullptr, tr("Save image"), QString(), tr("Image files (*.jpg *.jpeg *.png *.bmp)"));
}

int FileDialog::askToSave()
{
    int ret = QMessageBox::question(nullptr, QCoreApplication::applicationName(),
                                    tr("The image has been modified.\nDo you want to save your changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    switch (ret)
    {
        case QMessageBox::Yes:    ret =  1; break;
        case QMessageBox::No:     ret =  0; break;
        case QMessageBox::Cancel: ret = -1; break;
    }

    return ret;
}
