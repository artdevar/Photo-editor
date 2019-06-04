#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ImageHandler.h"
#include "ImageCorrector.h"
#include <QObject>

class MainWindow : public QObject
{
    Q_OBJECT

public:
    MainWindow(QObject * = nullptr);

    // File
    Q_INVOKABLE void actionOpen();
    Q_INVOKABLE void actionSave();
    Q_INVOKABLE void actionSaveAs();
    Q_INVOKABLE void actionClose();
    // Edit
    Q_INVOKABLE void actionUndo();
    Q_INVOKABLE void actionRedo();
    Q_INVOKABLE void actionFlipVertical();
    Q_INVOKABLE void actionFlipHorizontal();
    Q_INVOKABLE void actionTranspose();
    // Correction
    Q_INVOKABLE void actionBrightnessContrast(int, float);
    Q_INVOKABLE void actionHueSaturation(int, int, int);
    Q_INVOKABLE void actionAutoContrast();
    Q_INVOKABLE void actionInversion();
    Q_INVOKABLE void actionGrayscale();
    Q_INVOKABLE void actionSepia();
    // Filter
    Q_INVOKABLE void actionGaussianBlur(int);
    // --- //
    Q_INVOKABLE void saveState();
    Q_INVOKABLE void applyChanges();
    Q_INVOKABLE void openFile(const QString &);

private:
    bool eventFilter(QObject*, QEvent*) override;
    bool askToSave();

private:
      ImageHandler * const handler_ptr;
    ImageCorrector * const corrector_ptr;
};

#endif // MAINWINDOW_H
