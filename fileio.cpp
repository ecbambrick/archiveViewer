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

#include "fileio.h"
#include "QFileDialog"

QString FileIO::openFileDialogue(QString directory)
{
    QString filter = "Images and Archives (*.jpg *.jpeg *.png *.gif *.cbz)";
    return QFileDialog::getOpenFileName(0, "Open File", directory, filter);
}

FileIO::FileType FileIO::getFileType(QString path)
{
    QRegExp imageExtensions = QRegExp("^.*\\.(png|gif|jpeg|jpg)$");
    QRegExp archiveExtensions = QRegExp("^.*\\.(cbz|cbr|cb7|zip|rar|7z)$");
    imageExtensions.setCaseSensitivity(Qt::CaseInsensitive);
    archiveExtensions.setCaseSensitivity(Qt::CaseInsensitive);

    bool isImage = path.contains(imageExtensions);
    bool isArchive = path.contains(archiveExtensions);

    if (isArchive) return FileIO::ARCHIVE;
    else return FileIO::IMAGE;
}
