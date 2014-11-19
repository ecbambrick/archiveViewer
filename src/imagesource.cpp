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

#include <Qdatetime>
#include <random>
#include "imagesource.h"

// ------------------------------------------------------------------- public //

QList<ImageSourceItem> ImageSource::images() const
{
    return _images;
}

QList<ImageSourceItem> ImageSource::images(const Filter &filter,
                                           SortType sort,
                                           OrderType order) const
{
    // Filter images.
    auto i = 0;
    auto filteredImages = QList<ImageSourceItem>();
    for (auto image : _images) {
        if (filter.match(image.second->fileName())) {
            filteredImages.append({++i, image.second});
        }
    }

    // Shuffle images.
    if (order == RandomOrder) {
        std::random_shuffle(filteredImages.begin(), filteredImages.end());
    }

    // Sort images.
    else {
        auto comparison = this->getSortingFunction(sort, order);

        std::sort(filteredImages.begin(), filteredImages.end(), comparison);

        // Reset the image indices to match the new ordering.
        i = 0;
        for (auto &image : filteredImages) {
            image.first = ++i;
        }
    }

    return filteredImages;
}

QString ImageSource::name() const
{
    return _name;
}

// ------------------------------------------------------------------ private //

ImageSource::ImageComparison ImageSource::getSortingFunction(SortType sort,
                                                             OrderType order) const
{
    ImageComparison sortFunction;

    if (sort == SortByFileName && order == AscendingOrder) {
        sortFunction = [](ImageSourceItem a, ImageSourceItem b) {
            auto aVal = a.second->absoluteFilePath();
            auto bVal = b.second->absoluteFilePath();
            return QString::compare(aVal, bVal, Qt::CaseInsensitive) < 0;
        };
    }

    else if (sort == SortByFileName && order == DescendingOrder) {
        sortFunction = [](ImageSourceItem a, ImageSourceItem b) {
            auto aVal = a.second->absoluteFilePath();
            auto bVal = b.second->absoluteFilePath();
            return QString::compare(aVal, bVal, Qt::CaseInsensitive) < 0;
        };
    }

    else if (sort == SortByLastModifiedDate && order == AscendingOrder) {
        sortFunction = [](ImageSourceItem a, ImageSourceItem b) {
            auto aVal = a.second->lastModified();
            auto bVal = b.second->lastModified();
            return aVal < bVal;
        };
    }

    else if (sort == SortByLastModifiedDate && order == DescendingOrder) {
        sortFunction = [](ImageSourceItem a, ImageSourceItem b) {
            auto aVal = a.second->lastModified();
            auto bVal = b.second->lastModified();
            return aVal > bVal;
        };
    }

    return sortFunction;
}
