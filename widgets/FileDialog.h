#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QString>
#include <QWidget>

enum TSaveImageResponse
{
  Save,
  DontSave,
  Cancel
};

class FileDialog : public QWidget
{
  Q_OBJECT

public:

    static QString openFileName();
    static QString safeFileName();
    static TSaveImageResponse askToSave();
};

#endif // FILEDIALOG_H
