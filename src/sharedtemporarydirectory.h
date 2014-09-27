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

#ifndef SHAREDTEMPORARYDIRECTORY_H
#define SHAREDTEMPORARYDIRECTORY_H
#include <memory>
#include <QString>
#include <QTimer>
#include <QFile>

///
/// \brief The SharedTemporaryDirectory class represents a temporary directory
/// that can be shared amongst objects and processes.
///
/// If no directory at the given file path, one is created during the object's
/// initialization. The directory is removed during the object's construction
/// if no other process or object instance is using the directory. The
/// directory is only removed once all instances have finished using it.
///
class SharedTemporaryDirectory
{
public:

    ///
    /// \brief Constructs a new temporary directory with the given path,
    /// relative to the current user's temporary files directory. The
    /// directory is automatically created.
    /// \param path The path of the directory from within the current user's
    /// temporary files directory.
    ///
    SharedTemporaryDirectory(const QString &path);

    ///
    /// Deletes the temporary directory if no other instance is using it.
    ///
    ~SharedTemporaryDirectory();

    ///
    /// \brief Returns the path to the temporary directory.
    /// \return The path to the temporary directory.
    ///
    QString path();

private:

    ///
    /// \brief Updates the timestamp of the temporary file within the temporary
    /// directory.
    ///
    void updateTemporaryFile();

    /// A temporary file that exists within the temporary directory to indicate
    /// that an instance is currently using the directory.
    std::unique_ptr<QFile> _temporaryFile;

    /// The full path to the temporary directory.
    QString _path;

    /// A timer used to call updateTemporaryFile().
    QTimer _timer;

    /// The unique id of the temporary file. Used to ensure that the file name
    /// is unique.
    QString _id;
};

#endif // SHAREDTEMPORARYDIRECTORY_H
