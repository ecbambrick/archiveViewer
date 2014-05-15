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
#include <QList>

///
/// \brief The Playlist class represents a templated list of items with a
/// traversable index.
///
template<typename T>
class Playlist
{
public:

    ///
    /// \brief Constructs an empty playlist.
    ///
    Playlist();

    ///
    /// Destructor.
    ///
    ~Playlist();

    ///
    /// \brief Returns the item at the current index.
    /// \return The item at the current index.
    ///
    T current();

    ///
    /// \brief Returns the current index.
    /// \return The current index.
    ///
    int index();

    ///
    /// \brief Returns true if the current index is at the end; otherwise, false.
    /// \return True if the current index is at the end; otherwise, false.
    ///
    bool isAtEnd();

    ///
    /// \brief Returns true if the current index is at the beginning; otherwise, false.
    /// \return True if the current index is at the beginning; otherwise, false.
    ///
    bool isAtBeginning();

    ///
    /// \brief Returns true if traversal of the list loops; otherwise, false.
    /// \return True if traversal of the list loops; otherwise, false.
    ///
    bool loop();

public slots:

    ///
    /// \brief Moves the index to the first position.
    ///
    void first();

    ///
    /// \brief Moves the index to the given position.
    /// \param position The new value for the current index.
    ///
    void index(int position);

    ///
    /// \brief Moves the index to the last position.
    ///
    void last();

    ///
    /// \brief Sets whether or not traversal of the list loops.
    /// \param value True if traversal of the list loops; otherwise, false.
    ///
    void loop(bool value);

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

    int _index;      ///< The current index of the playlist.
    QList<T> *_list; ///< The list structure containing the items.
    bool _loop;      ///< True if traversal of the list loops; otherwise, false.
};

#endif // PLAYLIST_H
