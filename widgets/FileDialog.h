#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QObject>

class FileDialog : public QObject
{
    Q_OBJECT

public:
    FileDialog(QObject * = nullptr);

    static QString openFileName();
    static QString safeFileName();
    static   int   askToSave();
};

#endif // FILEDIALOG_H