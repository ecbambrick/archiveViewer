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

#include "imageinfo.h"

ImageInfo::ImageInfo(const QString &path, const QString &relativePath)
    : QFileInfo(path)
    , _id(0)
    , _relativePath(relativePath)
{
}

void ImageInfo::id(int value)
{
    if (value <= 0) {
        throw std::out_of_range("value");
    }

    _id = value;
}

int ImageInfo::id() const
{
    return _id;
}

QString ImageInfo::relativeFilePath() const
{
    return this->_relativePath + this->fileName();
}

QString ImageInfo::relativePath() const
{
    return this->_relativePath;
}
