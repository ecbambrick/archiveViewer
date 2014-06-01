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
    : _archive(new QuaZip(archivePath))
    , _currentFileNameChanged(false)
    , _extractPath(QDir::tempPath() + "/archiveViewer/" + Utility::hash(archivePath))
    , _extractWatcher(new QFutureWatcher<void>())
{
    this->loadImageInfoList();
    _name = QFileInfo(archivePath).completeBaseName();
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

// ------------------------------------------------------------- public slots //

void QuaZipImageSource::imageNeeded(ImageInfo *image)
{
    _currentFileName = image->relativeFilePath();
    _currentFileNameChanged = true;
}

// ------------------------------------------------------------------ private //

void QuaZipImageSource::extractAll()
{
    _archive->open(QuaZip::mdUnzip);
    _archive->goToFirstFile();

    // Keep track of the files that have not been extracted yet.
    QStringList fileNames;
    for (const ImageInfo &image : _images) {
        fileNames.append(image.relativeFilePath());
    }

    while (!fileNames.isEmpty() && !_extractWatcher->isCanceled()) {

        // If a specific image is requested to be extracted and has not already
        // been extracted, then move to that position in the archive.
        if (_currentFileNameChanged && fileNames.contains(_currentFileName)) {
            _archive->setCurrentFile(_currentFileName);
            _currentFileNameChanged = false;
        }

        // Do not extract if the file has already been extracted.
        QString fileName = _archive->getCurrentFileName();
        int index = fileNames.indexOf(fileName);
        if (index >= 0) {
            fileNames.removeOne(fileName);
            this->extractImage(fileName);
            emit imageReady(fileName);
        }

        if (!_archive->goToNextFile()) {
            _archive->goToFirstFile();
        }
    }

    _archive->close();
}

void QuaZipImageSource::extractImage(const QString &fileName)
{
    QuaZipFile source(_archive);
    QFileInfo destinationInfo(_extractPath + "/" + fileName);
    QFile destination(destinationInfo.absoluteFilePath());

    // Create any directories that might need to exist due to the archive
    // containing sub-directories.
    QDir().mkpath(destinationInfo.absolutePath());

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

void QuaZipImageSource::loadImageInfoList()
{
    _archive->open(QuaZip::mdUnzip);

    int i = 0;
    QStringList fileNames = _archive->getFileNameList();
    fileNames.sort(Qt::CaseInsensitive);
    for (QString fileName : fileNames) {

        // Get file information.
        QFileInfo fileInfo(_extractPath + "/" + fileName);
        QString suffix = fileInfo.suffix();
        QString relativePath = fileName;
        relativePath.chop(fileInfo.fileName().length());

        // Insert the file into the list of images if it is an image.
        if (Utility::imageFileTypes().contains(suffix, Qt::CaseInsensitive)) {
            ImageInfo image(fileInfo.absoluteFilePath(), relativePath);
            image.id(++i);
            _images.append(image);
        }
    }

    _archive->close();
}
