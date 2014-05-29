/*******************************************************************************

    Copyright (C) 2014 by Cole Bambrick
    cole.bambrick@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

#include <QtConcurrent/QtConcurrent>
#include <QDir>
#include <QFutureWatcher>
#include "utility.h"
#include "quazipimagesource.h"

// ------------------------------------------------------------------- public //

QuaZipImageSource::QuaZipImageSource(const QString &archivePath)
{
    _archive = new QuaZip(archivePath);
    _extractWatcher = new QFutureWatcher<void>();
    _extractPath = QDir::tempPath() + "/archiveViewer/" + Utility::hash(archivePath);
    _images = QList<ImageInfo>();
    _name = QFileInfo(archivePath).completeBaseName();
    bool success;

    // Get the list of images from the archive.
    int i = 0;
    _archive->open(QuaZip::mdUnzip);
    for (bool more = _archive->goToFirstFile(); more; more = _archive->goToNextFile()) {
        ImageInfo image = this->getImageInfo(&success);
        if (success) {
            image.id(++i);
            _images.append(image);
        }
    }
    _archive->close();

    // Extract files in a separate thread.
    QDir(_extractPath).mkpath(_extractPath);
    _extractWatcher->setFuture(QtConcurrent::run(this, &QuaZipImageSource::extractAll));
}

QuaZipImageSource::~QuaZipImageSource()
{
    if (_extractWatcher->isRunning()) {
        _extractWatcher->cancel();
        _extractWatcher->waitForFinished();
    }

    QDir(_extractPath).removeRecursively();
    delete _archive;
    delete _extractWatcher;
}

void QuaZipImageSource::imageNeeded(int id)
{
    // Will eventually be used for updating the position of the extraction loop.
    id = id;
}

// ------------------------------------------------------------------ private //

void QuaZipImageSource::extractAll()
{
    bool success;

    int i = 0;
    _archive->open(QuaZip::mdUnzip);
    for (bool more = _archive->goToFirstFile(); more; more = _archive->goToNextFile()) {
        if (_extractWatcher->isCanceled()) {
            break;
        }
        ImageInfo image = this->getImageInfo(&success);
        if (success) {
            extractImage(image);
            emit imageReady(++i);
        }
    }
    _archive->close();
}

void QuaZipImageSource::extractImage(const ImageInfo &image)
{
    QuaZipFile source(_archive);
    QFile destination(image.absoluteFilePath());

    // Create any directories that might need to exist due to the archive
    // containing sub-directories.
    QDir().mkpath(image.absolutePath());

    // Copy the data from the archive to the file.
    source.open(QIODevice::ReadOnly);
    destination.open(QIODevice::WriteOnly);
    destination.resize(source.size());
    while (!source.atEnd()) {
        destination.write(source.read(256));
    }
    destination.close();
    source.close();
}

ImageInfo QuaZipImageSource::getImageInfo(bool *success)
{
    QuaZipFile file(_archive);
    QFileInfo fileInfo(_extractPath + "/" + file.getActualFileName());
    QString absoluteFilePath("");
    QString relativePath("");

    file.open(QIODevice::ReadOnly);
    if (Utility::imageFileTypes().contains(fileInfo.suffix(), Qt::CaseInsensitive)) {
        absoluteFilePath = fileInfo.absoluteFilePath();
        relativePath = file.getActualFileName();
        relativePath.chop(fileInfo.fileName().length());
        *success = true;
    } else {
        *success = false;
    }
    file.close();

    return ImageInfo(absoluteFilePath, relativePath);
}
