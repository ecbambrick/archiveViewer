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

#ifndef UTILITY_H
#define UTILITY_H
#include <QStringList>

///
/// \brief The Utility class represents a set of static utility functions.
///
class Utility
{
public:

    ///
    /// \brief Returns the list of supported image file types.
    /// \return The list of supported image file types.
    ///
    static QStringList imageFileTypes();

    ///
    /// \brief Returns the list of supported archive file types.
    /// \return The list of supported archive file types.
    ///
    static QStringList archiveFileTypes();

private:

    /// The list of supported image file types.
    static const QStringList _imageFileTypes;

    /// The list of supported archive file types.
    static const QStringList _archiveFileTypes;

};

#endif // UTILITY_H
