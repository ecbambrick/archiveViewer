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
    , _temporaryDirectory("archiveViewer/" + Utility::hash(archivePath))
{
    _images = this->getImageInfoFromArchive(_archive.get(), _temporaryDirectory.path());
    _name = QFileInfo(archivePath).completeBaseName();
    _extractWatcher.setFuture(QtConcurrent::run(this, &QuaZipImageSource::extractAll));
}

QuaZipImageSource::~QuaZipImageSource()
{
    if (_extractWatcher.isRunning()) {
        _extractWatcher.cancel();
        _extractWatcher.waitForFinished();
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

    // Keep track of the images that have not been extracted yet.
    auto images = QList<ImageSourceItem>(_images);
    auto index = 0;

    while (!images.isEmpty() && !_extractWatcher.isCanceled()) {

        // If the currently displayed image has changed and has not yet been
        // extracted, change the index to that image.
        if (_currentFileNameChanged) {
            auto i = std::find_if(images.begin(), images.end(), [&](ImageSourceItem a) {
                return a.second->relativeFilePath() == _currentFileName;
            });

            if (i != images.end()) {
                index = i - images.begin();
            }
        }

        // Loop through the list of images.
        if (index >= images.size()) {
            index = 0;
        }

        // Extract the image.
        auto file = images.at(index).second;
        if (!file->exists()) {
            _archive->setCurrentFile(file->relativeFilePath());
            this->extractImage(_archive.get(), file->absoluteFilePath());
            emit imageReady(file->relativeFilePath());
        }

        // Since the image is removed, the index remains the same.
        images.removeAt(index);
    }

    _archive->close();
}

void QuaZipImageSource::extractImage(QuaZip *archive, const QString &filePath)
{
    QFileInfo destinationInfo(filePath);
    QuaZipFile source(archive);
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

QList<ImageSourceItem> QuaZipImageSource::getImageInfoFromArchive(QuaZip *archive, const QString &extractPath)
{
    archive->open(QuaZip::mdUnzip);

    auto i = 0;
    auto images = QList<ImageSourceItem>();
    auto fileNames = archive->getFileNameList();

    fileNames.sort(Qt::CaseInsensitive);
    for (const auto &fileName : fileNames) {

        // Get the file information.
        auto fileInfo = QFileInfo(extractPath + "/" + fileName);
        auto suffix = fileInfo.suffix();
        auto relativePath = fileName;
        relativePath.chop(fileInfo.fileName().length());

        // Insert the file into the list if it is an image file.
        if (Utility::imageFileTypes().contains(suffix, Qt::CaseInsensitive)) {
            auto extractPath = fileInfo.absoluteFilePath();
            auto image = std::make_shared<ImageInfo>(extractPath, relativePath);
            images.append({++i, image});
        }
    }

    archive->close();

    return images;
}
