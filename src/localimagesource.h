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
/// a local directory.
///
class LocalImageSource : public ImageSource
{
public:

    ///
    /// \brief Constructor.
    /// \param filePath The file path to the image.
    ///
    explicit LocalImageSource(const QString &filePath);

public slots:

    ///
    /// \brief Indicates that an image needs to be viewed.
    /// \param id The ID of the image.
    ///
    void imageNeeded(ImageInfo *image);
};

#endif // LOCALIMAGESOURCE_H
