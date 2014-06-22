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

#ifndef QUAZIPIMAGESOURCE_H
#define QUAZIPIMAGESOURCE_H
#include <memory>
#include <QFutureWatcher>
#include "imagesource.h"
#include "quazip.h"
#include "quazipfile.h"
#include "sharedtemporarydirectory.h"

///
/// \brief The QuaZipImageSource class represents a source of image files within
/// a zipped archived that can be queried.
///
/// Uses the QuaZip library to read/write from zip archives. The QuaZip image
/// source will find all supported image files that are contained within the
/// given archive and extract them to a temporary directory. All other files
/// will be ignored.
///
/// The contents of the archive will be checked recursively for sub-directories
/// but sub-archives will not be extracted.
///
class QuaZipImageSource : public ImageSource
{
public:

    ///
    /// \brief Constructs a QuaZip image source from the given zip archive.
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
    /// \param image The image information.
    ///
    void imageNeeded(std::shared_ptr<ImageInfo> image) override;

private:

    ///
    /// \brief Extracts all image files from a zip archive to the extract path.
    ///
    void extractAll();

    ///
    /// \brief Extracts a single image file from a zip archive to the absolute
    /// file path of the provided image information.
    /// \param fileName the file name of the image to extract.
    ///
    void extractImage(const QString &fileName);

    ///
    /// \brief Loads the image information from files within the zip archive.
    /// \return The list of image information from within the zip archive.
    ///
    QList<ImageSourceItem> getImageInfoFromArchive();

    /// The zip archive.
    std::unique_ptr<QuaZip> _archive;

    /// The file name of the image that needs to be extracted next if a request
    /// was made through the ImageNeeded slot.
    QString _currentFileName;

    /// True if a specific image is needed to be extracted next; otherwise, false.
    bool _currentFileNameChanged;

    /// The watcher for the extraction process.
    std::unique_ptr<QFutureWatcher<void>> _extractWatcher;

    /// The temporary directory to extract the zip archives contents to.
    SharedTemporaryDirectory _temporaryDirectory;
};

#endif // QUAZIPIMAGESOURCE_H
