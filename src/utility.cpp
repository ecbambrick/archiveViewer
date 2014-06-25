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

#define AVG(a,b) ( ((((a)^(b)) & 0xfefefefeUL) >> 1) + ((a)&(b)) )

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

// From https://blog.qt.digia.com/blog/2009/01/20/50-scaling-of-argb32-image/
QImage Utility::fastHalfScale(const QImage &source)
{
    if (source.width() % 2 != 0 || source.height() % 2 != 0) {
        throw std::invalid_argument("Image must have even dimensions");
    }

    QImage dest(source.size() * 0.5, QImage::Format_ARGB32_Premultiplied);

    const quint32 *src = reinterpret_cast<const quint32*>(source.bits());
    int sx = source.bytesPerLine() >> 2;
    int sx2 = sx << 1;

    quint32 *dst = reinterpret_cast<quint32*>(dest.bits());
    int dx = dest.bytesPerLine() >> 2;
    int ww = dest.width();
    int hh = dest.height();

    for (int y = hh; y; --y, dst += dx, src += sx2) {
        const quint32 *p1 = src;
        const quint32 *p2 = src + sx;
        quint32 *q = dst;
        for (int x = ww; x; --x, q++, p1 += 2, p2 += 2)
            * q = AVG(AVG(p1[0], p1[1]), AVG(p2[0], p2[1]));
    }

    return dest;
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
