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
#include "localimagesource.h"
#include "utility.h"

// ------------------------------------------------------------------- public //

LocalImageSource::LocalImageSource(const QString &path)
{
    auto dir = QFileInfo(path).dir();
    dir.setNameFilters(Utility::imageFileFilter());
    dir.setSorting(QDir::IgnoreCase);

    _images = getImageInfoFromDirectory(dir);
    _name = dir.dirName();
}

// ------------------------------------------------------------- public slots //

void LocalImageSource::imageNeeded(std::shared_ptr<ImageInfo> image)
{
    Q_UNUSED(image);
}

// ------------------------------------------------------------------ private //

QList<ImageSourceItem> LocalImageSource::getImageInfoFromDirectory(const QDir &dir)
{
    auto i = 0;
    auto images = QList<ImageSourceItem>();

    for (const auto &file : dir.entryInfoList()) {
        auto image = std::make_shared<ImageInfo>(file.absoluteFilePath());
        images.append({++i, image});
    }

    return images;
}
