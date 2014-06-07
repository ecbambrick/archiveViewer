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

#ifndef PLAYLIST_H
#define PLAYLIST_H
#include <memory>
#include <QList>
#include "imagesource.h"

///
/// \brief The Playlist class represents a traversable list of image information
/// that is queried from an image source.
///
class Playlist : public QObject
{
    Q_OBJECT

public:

    ///
    /// \brief Constructs a playlist from the given image source.
    ///
    explicit Playlist(std::shared_ptr<ImageSource> source);

    ///
    /// \brief Returns the item at the current index. The playlist must not be
    /// empty. Call isEmpty() before calling this function.
    /// \return The item at the current index.
    ///
    std::shared_ptr<ImageInfo> current() const;

    ///
    /// \brief Filters the playlist by file name based on the given filter.
    /// \param filter The filter to filter with.
    ///
    void filter(Filter filter);

    ///
    /// \brief Returns the current index.
    /// \return The current index.
    ///
    int index() const;

    ///
    /// \brief Returns true if the playlist is empty; otherwise, false.
    /// \return True of the playlist is empty; otherwise, false.
    ///
    bool isEmpty() const;

    ///
    /// \brief Returns true if traversal of the list loops; otherwise, false.
    /// \return True if traversal of the list loops; otherwise, false.
    ///
    bool loops() const;

    ///
    /// \brief Returns the number of elements in the playlist after filtering.
    /// \return The number of elements in the playlist after filtering.
    ///
    int size() const;

    ///
    /// \brief Sorts the playlist by the given order.
    /// \param orderBy The order to sort by.
    ///
    void sort(ImageSource::OrderType orderBy);

    ///
    /// \brief Sorts the playlist by the given method.
    /// \param sortBy The method to sort by.
    ///
    void sort(ImageSource::SortType sortBy);

    ///
    /// \brief Sorts the playlist by the given method and order.
    /// \param sortBy The method to sort by.
    /// \param orderBy The order to sort by.
    ///
    void sort(ImageSource::SortType sortBy, ImageSource::OrderType orderBy);

public slots:

    ///
    /// \brief Moves the index to the position of item with the given relative
    /// file name. If no items has the given relative file name, the index is
    /// not changed.
    /// \param relativeFileName The relative file name of the item.
    /// \return True if the relative file name was found; otherwise, false.
    ///
    bool find(const QString &relativeFilePath);

    ///
    /// \brief Sets whether or not traversal of the list loops.
    /// \param value True if traversal of the list loops; otherwise, false.
    ///
    void loops(bool value);

    ///
    /// \brief Increments the index by the given value.
    /// \param steps The number of steps to increment. Defaults to 1.
    ///
    void next(int steps = 1);

    ///
    /// \brief Decrements the index by the given value.
    /// \param steps The number of steps to decrement. Defaults to 1.
    ///
    void previous(int steps = 1);

    ///
    /// \brief Shuffles or unshuffles the playlist.
    /// \param value True if the playlist will be shuffled; false if the
    /// playlist will be returned to its previous sorting method.
    ///
    void shuffle(bool value);

signals:

    ///
    /// \brief Signals that the current index has changed.
    ///
    void indexChanged();

private:

    ///
    /// \brief Reloads the list of images from the image source.
    ///
    void reload();

    ///
    /// \brief Reloads the list of images from the image source.
    /// \param filter The filter to filter file names against.
    /// \param sortBy The order in which items are sorted.
    /// \param orderBy The method by which items are sorted.
    ///
    void reload(const Filter &filter,
                ImageSource::SortType sortBy,
                ImageSource::OrderType orderBy);

    /// The filter to filter file names against.
    Filter _filter;

    /// The current, true index of the playlist. This will differ from the
    /// index of the current item when the list playlist is shuffled.
    int _index;

    /// The list structure containing the items.
    QList<ImageSourceItem> _list;

    /// True if traversal of the list loops; otherwise, false.
    bool _loops;

    /// The order in which items are sorted in the playlist.
    ImageSource::OrderType _orderBy;

    /// The order in which items are sorted before being shuffled.
    ImageSource::OrderType _originalOrderBy;

    /// The method by which items are sorted in the playlist.
    ImageSource::SortType _sortBy;

    ///< The source of items to query from.
    std::shared_ptr<ImageSource> _source;
};

#endif // PLAYLIST_H
