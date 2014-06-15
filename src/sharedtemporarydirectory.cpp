#include "sharedtemporarydirectory.h"
#include <QDateTime>
#include <QDir>

// The file type extension of a temporary file.
const auto fileType = ".archiveTemp";

// The number of milliseconds to wait before updating the temporary file's
// file name.
const auto interval = 5 * 60 * 1000;

// ------------------------------------------------------------------- public //

SharedTemporaryDirectory::SharedTemporaryDirectory(const QString &path)
    : _path(QDir::tempPath() + "/" + path)
    , _id(tmpnam(NULL))
{
    if (path == NULL || path.isEmpty()) {
        throw std::invalid_argument("Temporary directory path cannot be empty");
    }

    // Clean the ID by removing the trailing period and changing the slash.
    _id.replace(0, 1, '/').chop(1);

    // Create the temporary directory with a temporary file.
    QDir().mkpath(_path);
    _temporaryFile = std::unique_ptr<QFile>(new QFile(_path + _id));
    _temporaryFile->open(QFile::WriteOnly);
    _temporaryFile->close();
    this->updateTemporaryFile();

    // create a timer to update the temporary file's file name.
    _timer.connect(&_timer, &QTimer::timeout, [&]() {
        this->updateTemporaryFile();
    });
    _timer.start(interval);
}

SharedTemporaryDirectory::~SharedTemporaryDirectory()
{
    auto directory = QDir(_path);
    auto currentTime = QDateTime::currentMSecsSinceEpoch();
    auto tempFileTypes = QStringList(QString("*").append(fileType));

    // Remove the temporary file and any other temporary files that have
    // expired. Expired files may be left behind by applications that have
    // unexpectedly quit before being able to clean up.
    _temporaryFile->remove();
    for (QString fileName : directory.entryList(tempFileTypes)) {
        auto timeStamp = fileName.section(".", 1, 1).toLongLong() + interval*2;
        if (timeStamp < currentTime) {
            QFile(_path + "/" + fileName).remove();
        }
    }

    // Only delete the temporary directory of no temporary files exist.
    if (directory.entryList(tempFileTypes).length() <= 0) {
        directory.removeRecursively();
    }
}

// ------------------------------------------------------------------ private //

void SharedTemporaryDirectory::updateTemporaryFile()
{
    auto timeStamp = QString::number(QDateTime::currentMSecsSinceEpoch());

    _temporaryFile->rename(_path + _id + "." + timeStamp + fileType);
}
