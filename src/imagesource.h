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

#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H
#include <QUrl>
#include "image.h"

///
/// \brief The ImageSource class represents a source of image files.
///
/// The source can be queried based the file name of each image and the
/// sorting order.
///
class ImageSource
{
public:

    ///
    /// \brief The OrderType enum represents different ways to order images.
    ///
    enum OrderType {
        ascendingOrder          ///< Sort in ascending order.
        ,descendingOrder        ///< Sort in descending order.
        ,randomOrder            ///< Sort in random order.
    };

    ///
    /// \brief The SortType enum represents different ways to sort images.
    ///
    enum SortType {
        sortByFileName          ///< Sort by file name.
        ,sortByLastModifiedDate ///< Sort by the last modified date.
    };

    ///
    /// \brief Returns the list of images located in the source.
    /// \param filter The query to filter on. Defaults to an empty string.
    /// \param sort The sorting method. Defaults to sortByFileName.
    /// \param order The ordering method. Defaults to ascendingOrder.
    /// \return The list of images located in the source.
    ///
    virtual QList<Image*> images(const QString &filter = "",
                                 SortType sort = sortByFileName,
                                 OrderType order = ascendingOrder) = 0;

    ///
    /// \brief Returns the name of the source.
    /// \return The name of the source.
    ///
    virtual QString name() = 0;

public slots:

    ///
    /// \brief Indicates that an image needs to be viewed.
    /// \param id The ID of the image.
    ///
    void imageNeeded(int id) = 0;

signals:

    ///
    /// \brief Signals that an image is ready to be viewed.
    /// \param id The ID of the image.
    ///
    void imageReady(int id);
};

#endif // IMAGESOURCE_H
