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
    /// \brief Returns the list of supported archive file types as a filter.
    /// \return The list of supported archive file types as a filter.
    ///
    static QStringList archiveFileFilter();

    ///
    /// \brief Returns the list of supported archive file types.
    /// \return The list of supported archive file types.
    ///
    static QStringList archiveFileTypes();

    ///
    /// \brief Returns a file type filter for the File Open dialog.
    /// \return A file type filter for the File Open dialog.
    ///
    static QString openDialogFilter();

    ///
    /// \brief Returns the hash of the provided string.
    /// \param string The string to hash.
    /// \return The hash of the string.
    ///
    static QByteArray hash(const QString &string);

    ///
    /// \brief Returns the list of supported image file types as a filter.
    /// \return The list of supported image file types as a filter.
    ///
    static QStringList imageFileFilter();

    ///
    /// \brief Returns the list of supported image file types.
    /// \return The list of supported image file types.
    ///
    static QStringList imageFileTypes();

private:

    /// The list of supported archive file types.
    static const QStringList _archiveFileTypes;

    /// The list of supported image file types.
    static const QStringList _imageFileTypes;

};

#endif // UTILITY_H
