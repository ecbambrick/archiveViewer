#ifndef SHAREDTEMPORARYDIRECTORY_H
#define SHAREDTEMPORARYDIRECTORY_H
#include <memory>
#include <QString>
#include <QTimer>
#include <QFile>

class SharedTemporaryDirectory
{
public:
    SharedTemporaryDirectory(const QString &path);
    ~SharedTemporaryDirectory();

private:
    void updateTemporaryFile();

    std::unique_ptr<QFile> _temporaryFile;
    QString _path;
    QTimer _timer;
    QString _id;
};

#endif // SHAREDTEMPORARYDIRECTORY_H
