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

#ifndef LOCALIMAGESOURCE_H
#define LOCALIMAGESOURCE_H
#include "imagesource.h"

///
/// \brief The LocalImageSource class represents a source of image files within
/// a local directory that can be queried.
///
/// The local image source will find all supported image and archive files that
/// are contained within the directory of the given path. Any other files will
/// be ignored.
///
class LocalImageSource : public ImageSource
{
public:

    ///
    /// \brief Constructs a local image source from the given file path.
    /// \param filePath The file path to the directory.
    ///
    explicit LocalImageSource(const QString &path);

public slots:

    ///
    /// \brief Indicates that an image needs to be viewed.
    /// \param image The image information.
    ///
    void imageNeeded(std::shared_ptr<ImageInfo> image) override;

private:

    ///
    /// \brief Returns the list of image information from the given directory.
    /// \return The list of image information from the given directory.
    ///
    QList<ImageSourceItem> getImageInfoFromDirectory(const QDir &dir);
};

#endif // LOCALIMAGESOURCE_H
