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

#include <QDir>
#include "utility.h"
#include "localimagesource.h"

LocalImageSource::LocalImageSource(const QString &filePath)
{
    QFileInfo file(filePath);
    QDir dir(file.dir());
    this->_name = dir.dirName();
    this->_images = QList<ImageInfo>();

    int i = 0;
    for (const QFileInfo &file : dir.entryInfoList(Utility::imageFileFilter())) {
        auto image = ImageInfo(file.absoluteFilePath());
        image.id(++i);
        this->_images.append(image);
    }
}

void LocalImageSource::imageNeeded(ImageInfo *image)
{
    // Nothing needs to be done.
    image;
}
