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
    along with this program.  If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

#ifndef IMAGE_H
#define IMAGE_H
#include <QFileInfo>

///
/// \brief The ImageInfo class represents the meta data of an image file.
///
class ImageInfo : public QFileInfo
{
public:

    ///
    /// \brief Constructor.
    /// \param path The path to the image file.
    /// \param relativePath The path relative to the image source.
    ///
    ImageInfo(const QString &path, const QString &relativePath = "");

    ///
    /// \brief Sets the ID of the image information.
    /// \param value the ID of the image information.
    ///
    void id(int value);

    ///
    /// \brief Gets the ID of the image information.
    /// \return The ID of the image information.
    ///
    int id() const;

    ///
    /// \brief Returns the path relative to the image source including the
    /// file name.
    /// \return The path relative to the image source including the file name.
    ///
    QString relativeFilePath() const;

    /// \brief Returns the path relative to the image source.
    /// \return The path relative to the image source.
    ///
    QString relativePath() const;

private:

    /// The path of the image file, relative to the image source.
    QString _relativePath;

    /// The ID of the image information. Used to track the true position of
    /// an image file when in a shuffled list. It actually doesn't make sense
    /// for this to be in this class, but I'm putting it here out of laziness
    /// for the time being.
    int _id;
};

#endif // IMAGE_H
