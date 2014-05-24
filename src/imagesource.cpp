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

#include <Qdatetime>
#include <random>
#include "imagesource.h"

ImageSource::~ImageSource()
{
}

QList<ImageInfo> ImageSource::images()
{
    return _images;
}

QList<ImageInfo> ImageSource::images(const Filter &filter,
                                      SortType sort,
                                      OrderType order)
{
    // Filter images.
    QList<ImageInfo> filteredImages;
    for (const ImageInfo &image : _images) {
        if (filter.match(image.fileName())) {
            filteredImages.append(image);
        }
    }

    // Shuffle images.
    if (order == RandomOrder) {
        std::random_shuffle(filteredImages.begin(), filteredImages.end());

    // Sort images.
    } else {
        std::function<bool(ImageInfo, ImageInfo)> sortFunction;
        if (sort == SortByFileName && order == AscendingOrder) {
            sortFunction = [](ImageInfo a, ImageInfo b) {
                return a.absoluteFilePath() < b.absoluteFilePath();
            };
        } else if (sort == SortByFileName && order == DescendingOrder) {
            sortFunction = [](ImageInfo a, ImageInfo b) {
                return a.absoluteFilePath() > b.absoluteFilePath();
            };
        } else if (sort == SortByLastModifiedDate && order == AscendingOrder) {
            sortFunction = [](ImageInfo a, ImageInfo b) {
                return a.lastModified() < b.lastModified();
            };
        } else if (sort == SortByLastModifiedDate && order == DescendingOrder) {
            sortFunction = [](ImageInfo a, ImageInfo b) {
                return a.lastModified() > b.lastModified();
            };
        }
        std::sort(filteredImages.begin(), filteredImages.end(), sortFunction);
    }

    return filteredImages;
}

QString ImageSource::name()
{
    return _name;
}
