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
#include "utility.h"
#include "quazipimagesource.h"

// ------------------------------------------------------------------- public //

QuaZipImageSource::QuaZipImageSource(const QString &archivePath)
    : _archive(new QuaZip(archivePath))
    , _currentFileNameChanged(false)
    , _extractWatcher(new QFutureWatcher<void>())
    , _temporaryDirectory("archiveViewer/" + Utility::hash(archivePath))
{
    _images = this->getImageInfoFromArchive();
    _name = QFileInfo(archivePath).completeBaseName();
    _extractWatcher->setFuture(QtConcurrent::run(this, &QuaZipImageSource::extractAll));
}

QuaZipImageSource::~QuaZipImageSource()
{
    if (_extractWatcher->isRunning()) {
        _extractWatcher->cancel();
        _extractWatcher->waitForFinished();
    }
}

// ------------------------------------------------------------- public slots //

void QuaZipImageSource::imageNeeded(std::shared_ptr<ImageInfo> image)
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
    auto fileNames = QStringList();
    for (const auto &image : _images) {
        fileNames.append(image.second->relativeFilePath());
    }

    while (!fileNames.isEmpty() && !_extractWatcher->isCanceled()) {

        // If a specific image is requested to be extracted and has not already
        // been extracted, then move to that position in the archive.
        if (_currentFileNameChanged && fileNames.contains(_currentFileName)) {
            _archive->setCurrentFile(_currentFileName);
            _currentFileNameChanged = false;
        }

        // Do not extract if the file has already been extracted.
        auto fileName = _archive->getCurrentFileName();
        auto destinationInfo = QFileInfo(_temporaryDirectory.path() + "/" + fileName);
        auto isExtracted = destinationInfo.exists();
        auto index = fileNames.indexOf(fileName);
        if (index >= 0) {
            fileNames.removeOne(fileName);
            if (!isExtracted) {
                this->extractImage(fileName);
                emit imageReady(fileName);
            }
        }

        if (!_archive->goToNextFile()) {
            _archive->goToFirstFile();
        }
    }

    _archive->close();
}

void QuaZipImageSource::extractImage(const QString &fileName)
{
    QFileInfo destinationInfo(_temporaryDirectory.path() + "/" + fileName);
    QuaZipFile source(_archive.get());
    QFile destination(destinationInfo.absoluteFilePath());

    // Create any directories that would need to exist.
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

QList<ImageSourceItem> QuaZipImageSource::getImageInfoFromArchive()
{
    _archive->open(QuaZip::mdUnzip);

    auto i = 0;
    auto images = QList<ImageSourceItem>();
    auto fileNames = _archive->getFileNameList();

    fileNames.sort(Qt::CaseInsensitive);
    for (const auto &fileName : fileNames) {

        // Get the file information.
        auto fileInfo = QFileInfo(_temporaryDirectory.path() + "/" + fileName);
        auto suffix = fileInfo.suffix();
        auto relativePath = fileName;
        relativePath.chop(fileInfo.fileName().length());

        // Insert the file into the list if it is an image file.
        if (Utility::imageFileTypes().contains(suffix, Qt::CaseInsensitive)) {
            auto path = fileInfo.absoluteFilePath();
            auto image = std::make_shared<ImageInfo>(path, relativePath);
            images.append({++i, image});
        }
    }

    _archive->close();

    return images;
}
