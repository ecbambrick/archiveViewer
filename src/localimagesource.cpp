#include <QDir>
#include <QDebug>
#include "utility.h"
#include "localimagesource.h"

LocalImageSource::LocalImageSource(const QString &filePath)
{
    QFileInfo file(filePath);
    QDir dir(file.dir());
    this->_name = dir.dirName();
    this->_images = QList<ImageInfo*>();

    for (const QFileInfo &file : dir.entryInfoList(Utility::imageFileTypes())) {
        this->_images.append(new ImageInfo(file.absoluteFilePath()));
    }
}

LocalImageSource::~LocalImageSource()
{
    qDeleteAll(_images);
}

void LocalImageSource::imageNeeded(int id)
{
    // Nothing needs to be done.
    id = id;
}
