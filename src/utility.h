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
#include <QImage>
#include <QStringList>

namespace Utility
{
    ///
    /// \brief Returns the list of supported archive file name filters. An
    /// example of a file name filter would be "*.jpg".
    /// \return The list of supported archive file name filters.
    ///
    QStringList archiveFileFilter();

    ///
    /// \brief Returns the list of supported archive file type extensions. An
    /// example of a file type extension would be "jpg".
    /// \return The list of supported archive file type extensions.
    ///
    QStringList archiveFileTypes();

    ///
    /// \brief Returns the MD5 hash of the provided string as hexidecimal.
    /// \param string The string to hash.
    /// \return The hexidecimal MD5 hash of the string.
    ///
    QByteArray hash(const QString &string);

    ///
    /// \brief Returns the list of supported image file name filters. An
    /// example of a file name filter would be "*.jpg".
    /// \return The list of supported image file name filters.
    ///
    QStringList imageFileFilter();

    ///
    /// \brief Returns the list of supported archive file type extensions. An
    /// example of a file type extension would be "jpg".
    /// \return The list of supported image file type extensions.
    ///
    QStringList imageFileTypes();

    ///
    /// \brief Returns the file type filter for the File Open dialog. An
    /// example of a File Open dialog filter would be "All Files (*.*)".
    /// \return The file type filter for the File Open dialog.
    ///
    QString openDialogFilter();

    ///
    /// \brief Returns whether or not the given arguments are roughly equal.
    /// \param a The first object to compare.
    /// \param b The second object to compare.
    /// \param error The margin of error when comparing.
    /// \return True if the difference between the given arguments is less than
    /// the given error; otherwise, false.
    ///
    template<typename T>
    bool roughlyEquals(T a, T b, int error)
    {
        return std::abs(a-b) < error;
    }
}

#endif // UTILITY_H
