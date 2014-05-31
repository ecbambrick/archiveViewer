/*******************************************************************************

    Copyright (C) 2013 by Cole Bambrick
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

#ifndef QUAZIPIMAGESOURCE_H
#define QUAZIPIMAGESOURCE_H
#include <memory>
#include <QFutureWatcher>
#include "imagesource.h"
#include "quazip.h"
#include "quazipfile.h"

///
/// \brief The QuaZipImageSource class represents a source of image files within
/// a zipped archived.
///
/// Uses the QuaZip library to read/write from zip archives.
///
class QuaZipImageSource : public ImageSource
{
public:

    ///
    /// \brief Constructor.
    /// \param archivePath The file path to the zip archive file.
    ///
    explicit QuaZipImageSource(const QString &archivePath);

    ///
    /// \brief Destructor.
    ///
    ~QuaZipImageSource();

public slots:

    ///
    /// \brief Indicates that an image needs to be viewed.
    /// \param id The ID of the image.
    ///
    void imageNeeded(ImageInfo *image) override;

private:

    ///
    /// \brief Extracts all image files from a zip archive to the extract path.
    ///
    void extractAll();

    ///
    /// \brief Extracts a single image file from a zip archive to the absolute
    /// file path of the provided image information.
    /// \param image The image file information.
    ///
    void extractImage(const ImageInfo &image);

    ///
    /// \brief Returns image information describing the current QuaZipFile.
    /// \param success True if the QuaZipFile is valid; otherwise, false.
    /// \return Image information describing the current QuaZipFile.
    ///
    ImageInfo getImageInfo(bool *success);

    /// The zip archive.
    QuaZip *_archive;

    /// The watcher for the extraction process.
    QFutureWatcher<void> *_extractWatcher;

    /// The directory path to extract the contents of the zip archive to.
    QString _extractPath;
};

#endif // QUAZIPIMAGESOURCE_H
