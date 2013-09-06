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

    FileIO:
    set of static methods for performing File IO

*******************************************************************************/

#ifndef FILEIO_H
#define FILEIO_H

#include <QString>
#include <QFileInfo>
#include <QDir>

class FileIO
{
public:
    enum FileType { ARCHIVE, IMAGE, DIR, INVALID };
    static QString openFileDialogue(QString directory);
    static FileType getFileType(QFileInfo *file);
};

#endif // FILEIO_H
