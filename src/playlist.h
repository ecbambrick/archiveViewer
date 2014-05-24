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
#include <exception>
#include <QList>
#include "imagesource.h"

///
/// \brief The Playlist class represents a traversable list of ImageInfo objects
/// that are queried from an image source.
///
class Playlist
{
public:

    ///
    /// \brief Constructs a playlist from the given image source.
    ///
    Playlist(ImageSource *source);

    ///
    /// Destructor.
    ///
    ~Playlist();

    ///
    /// \brief Returns the item at the current index. The playlist must not be
    /// empty. call isEmpty() before calling this function.
    /// \return The item at the current index.
    ///
    ImageInfo current();

    ///
    /// \brief Returns the current index.
    /// \return The current index.
    ///
    int index();

    ///
    /// \brief Returns true if the playlist is empty; otherwise, false.
    /// \return True of the playlist is empty; otherwise, false.
    ///
    bool isEmpty();

    ///
    /// \brief Returns true if traversal of the list loops; otherwise, false.
    /// \return True if traversal of the list loops; otherwise, false.
    ///
    bool loops();

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
    /// \brief Moves the index to the given position.
    /// \param position The new value for the current index.
    ///
    void index(int position);

    ///
    /// \brief Sets whether or not traversal of the list loops.
    /// \param value True if traversal of the list loops; otherwise, false.
    ///
    void loops(bool value);

    ///
    /// \brief Increment the index by the given value.
    /// \param steps The number of steps to increment. Defaults to 1.
    ///
    void next(int steps = 1);

    ///
    /// \brief Decrement the index by the given value.
    /// \param steps The number of steps to decrement. Defaults to 1.
    ///
    void previous(int steps = 1);

signals:

    ///
    /// \brief Signals that the current index has changed.
    /// \param position The new current index.
    ///
    void indexChanged(int position);

private:

    ///
    /// \brief Reloads the list of images from the image source.
    ///
    void reload();

    ///< The current index of the playlist.
    int _index;

    ///< The list structure containing the items.
    QList<ImageInfo> _list;

    ///< True if traversal of the list loops; otherwise, false.
    bool _loops;

    /// asd.
    ImageSource::OrderType _orderBy;

    /// asd.
    ImageSource::SortType _sortBy;

    ///< The source of items to query from.
    ImageSource *_source;
};

#endif // PLAYLIST_H
