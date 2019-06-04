#include "MainWindow.h"
#include "FileDialog.h"
#include <QKeyEvent>
#include <QApplication>
#include <QList>


MainWindow::MainWindow(QObject * _parent_ptr) : QObject(_parent_ptr),
                                                handler_ptr(ImageHandler::instance()),
                                                corrector_ptr(ImageCorrector::instance())
{
    qApp->installEventFilter(this);
}

void MainWindow::actionOpen()
{
    if (handler_ptr->isImageOpened())
        if (!askToSave())
            return;
    handler_ptr->setImage(FileDialog::openFileName());
}

void MainWindow::actionSave()
{
    handler_ptr->saveImage();
}

void MainWindow::actionSaveAs()
{
    handler_ptr->saveImageAs(FileDialog::safeFileName());
}

void MainWindow::actionClose()
{
    if (askToSave())
        handler_ptr->removeImage();
}

void MainWindow::actionUndo()
{
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_UNDO, QVariant());
}

void MainWindow::actionRedo()
{
    // need implementation
}

void MainWindow::actionFlipVertical()
{
    corrector_ptr->saveState();
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_FLIP_VERTICAL, QVariant());
}

void MainWindow::actionFlipHorizontal()
{
    corrector_ptr->saveState();
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_FLIP_HORIZONTAL, QVariant());
}

void MainWindow::actionTranspose()
{
    corrector_ptr->saveState();
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_TRANSPOSE, QVariant());
}

void MainWindow::actionBrightnessContrast(int _brightness, float _contrast)
{
    const QList<float> container{static_cast<float>(_brightness), _contrast};
    // snapshot creates from qml code
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_BRIGHTNESS_CONTRAST, QVariant::fromValue(container));
}

void MainWindow::actionHueSaturation(int _hue, int _saturation, int _light)
{
    const QList<int> container{_hue, _saturation, _light};
    // snapshot creates from qml code
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_HUE_SATURATION, QVariant::fromValue(container));
}

void MainWindow::actionAutoContrast()
{
    corrector_ptr->saveState();
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_AUTO_CONTRAST, QVariant());
}

void MainWindow::actionInversion()
{
    corrector_ptr->saveState();
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_INVERT_PIXELS, QVariant());
}

void MainWindow::actionGrayscale()
{
    corrector_ptr->saveState();
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_GRAYSCALE, QVariant());
}

void MainWindow::actionSepia()
{
    corrector_ptr->saveState();
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_SEPIA, QVariant());
}

void MainWindow::actionGaussianBlur(int _value)
{
    // snapshot creates from qml code
    corrector_ptr->editImage(ImageCorrector::Options::OPTION_GAUSSIAN_BLUR, QVariant(_value));
}

void MainWindow::saveState()
{
    corrector_ptr->saveState();
}

void MainWindow::applyChanges()
{
    handler_ptr->updateImageInfo();
}

void MainWindow::openFile(const QString & _fileName)
{
    handler_ptr->setImage(_fileName);
}

bool MainWindow::askToSave()
{
    bool ok = true;
    if (handler_ptr->imageInfo()->imageEdited)
    {
        switch (FileDialog::askToSave())
        {
            case -1: ok = false;     break;
            case  0:                 break;
            case  1: actionSaveAs(); break;
        }
    }
    return ok;
}

bool MainWindow::eventFilter(QObject * _object, QEvent * _event)
{
    bool eventHandled = false;
    switch (_event->type())
    {
        case QEvent::KeyPress:
        {
            auto * keyEvent_ptr = static_cast<QKeyEvent*>(_event);
            if (keyEvent_ptr->modifiers() == Qt::ControlModifier)
            {
                switch (keyEvent_ptr->key())
                {
                    case Qt::Key_O:
                    {
                        actionOpen();
                        eventHandled = true;
                        break;
                    }
                    case Qt::Key_S:
                    {
                        if (handler_ptr->isImageOpened()) 
                            actionSaveAs();
                        eventHandled = true;
                        break;
                    }
                    case Qt::Key_Y:
                    {
                        if (handler_ptr->isImageOpened()) 
                            actionRedo();
                        eventHandled = true;
                        break;
                    }
                    case Qt::Key_Z:
                    {
                        if (handler_ptr->isImageOpened()) 
                            actionUndo();
                        eventHandled = true;
                        break;
                    }
                }
            }
            break; // QEvent::KeyPress
        }
        case QEvent::Close:
        {
            const bool isMainWindow = !_object->parent();
            if (isMainWindow && handler_ptr->isImageOpened()) 
                eventHandled = !askToSave();
            break;
        }
        default: break;
    }
    return eventHandled;
}