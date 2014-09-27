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

#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H
#include <memory>
#include <QPair>
#include "filter.h"
#include "imageinfo.h"

///
/// \typedef The image source item represents image information paired with an
/// index.
///
/// The index is used to determine the position of the image in a list. This is
/// necessary because the index must remain unchanged when the list is shuffled.
///
typedef QPair<int, std::shared_ptr<ImageInfo>> ImageSourceItem;

///
/// \brief The ImageSource abstract class represents a source of image files
/// that can be queried.
///
class ImageSource : public QObject
{
    Q_OBJECT

public:

    ///
    /// \brief The OrderType enum represents methods to order images with.
    ///
    enum OrderType {
        AscendingOrder,         ///< Sort in ascending order.
        DescendingOrder,        ///< Sort in descending order.
        RandomOrder,            ///< Sort in random order.
    };

    ///
    /// \brief The SortType enum represents methods to sort images with.
    ///
    enum SortType {
        SortByFileName,         ///< Sort by file name.
        SortByLastModifiedDate, ///< Sort by the last modified date.
    };

    ///
    /// \brief Returns the list of images located in the source.
    /// \return The list of image information.
    ///
    virtual QList<ImageSourceItem> images() const;

    ///
    /// \brief Returns the list of images located in the source.
    /// \param filter The pattern to filter with.
    /// \param sort The sorting method. Defaults to SortByFileName.
    /// \param order The ordering method. Defaults to AscendingOrder.
    /// \return The filtered list of image information.
    ///
    virtual QList<ImageSourceItem> images(const Filter &filter,
                                          SortType sort = SortByFileName,
                                          OrderType order = AscendingOrder) const;

    ///
    /// \brief Returns the name of the source.
    /// \return The name of the source.
    ///
    virtual QString name() const;

public slots:

    ///
    /// \brief Indicates that an image needs to be viewed.
    /// \param image The image information.
    ///
    virtual void imageNeeded(std::shared_ptr<ImageInfo> image) = 0;

signals:

    ///
    /// \brief Signals that an image is ready to be viewed.
    /// \param relativeFilePath the relative file path of the image.
    ///
    void imageReady(const QString &relativeFilePath);

protected:

    /// The list of image information contained within the source.
    QList<ImageSourceItem> _images;

    /// The name of the source.
    QString _name;
};

#endif // IMAGESOURCE_H
