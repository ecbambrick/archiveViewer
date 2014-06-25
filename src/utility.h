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
    /// \brief Scales a copy of the given image to half its size and returns it.
    /// This function has similar results to using smooth scaling, but is much
    /// faster.
    /// \param image The image to scale.
    /// \return A copy of the image, scaled to half size.
    ///
    QImage fastHalfScale(const QImage &image);

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
}

#endif // UTILITY_H
