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
    along with this program.  If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

#include <QCryptographicHash>
#include "utility.h"

const QStringList _imageFileTypes {
    "bmp",
    "gif",
    "jpg",
    "jpeg",
    "png",
};

const QStringList _archiveFileTypes {
    "zip",
    "cbz",
};

QStringList Utility::archiveFileFilter()
{
    auto filter = QStringList();
    for (const auto &string : _archiveFileTypes) {
        filter.append("*." + string);
    }

    return filter;
}

QStringList Utility::archiveFileTypes()
{
    return _archiveFileTypes;
}

QByteArray Utility::hash(const QString &string)
{
    auto utf8String = string.toUtf8();
    auto hash = QCryptographicHash::hash(utf8String, QCryptographicHash::Md5);

    return hash.toHex();
}

QStringList Utility::imageFileFilter()
{
    auto filter = QStringList();
    for (const auto &string : _imageFileTypes) {
        filter.append("*." + string);
    }

    return filter;
}

QStringList Utility::imageFileTypes()
{
    return _imageFileTypes;
}

QString Utility::openDialogFilter()
{
    auto images = Utility::imageFileFilter().join(" ");
    auto archives = Utility::archiveFileFilter().join(" ");

    return "All Image and Archive Files (" + images + " " + archives + ");;"
           "Image Files (" + images + ");;"
           "Archive Files (" + archives + ");;"
           "All Files (*.*)";
}
