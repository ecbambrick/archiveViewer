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

********************************************************************************

    set of static functions for performing file operations

*******************************************************************************/

#include "fileio.h"
#include "QFileDialog"
#include <QDesktopServices>
#include <QUrl>

// private data
QStringList imageSuffixes   = (QStringList() << "png" << "gif" << "jpeg" << "jpg");
QStringList archiveSuffixes = (QStringList() << "cbz" << "cbr" << "cb7" << "zip" << "rar" << "7z");
QString filter              = "Images and Archives (*.jpg *.jpeg *.png *.gif *.cbz *.cbr *.cb7);;"
                              "Any File (*.*)";

/// open file dialogue at the supplied directory
QString FileIO::openFileDialogue(QString directory)
{
    return QFileDialog::getOpenFileName(0, "Open File", directory, filter);
}

/// get the file type of a file (archive file, image file, director, invalid)
FileIO::FileType FileIO::getFileType(QFileInfo *file)
{
    QString suffix = file->suffix();
    if (imageSuffixes.contains(suffix, Qt::CaseInsensitive)) {
        return FileIO::IMAGE;
    } else if (archiveSuffixes.contains(suffix, Qt::CaseInsensitive)) {
        return FileIO::ARCHIVE;
    } else if (file->isDir()) {
        return FileIO::DIR;
    } else {
        return FileIO::INVALID;
    }
}

/// open the file manager at the location of the file
void FileIO::openFileManager(QFileInfo *file)
{
    QDesktopServices::openUrl(QUrl("file:///" + file->absolutePath()));
}
