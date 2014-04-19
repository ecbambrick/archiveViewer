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

#include "directoryimagelist.h"
#include <QDir>
#include <QFileInfoList>
#include <QDebug>

DirectoryImageList::DirectoryImageList(const QString &path)
{
    // Initialize
    QFileInfo file(path);
    QStringList filters;
    _directoryPath = file.absolutePath();
    _directoryName = file.dir().dirName();
    filters << "*.jpg" << "*.jpeg" << "*.gif" << "*.png";

    // Generate the list of images from the directory
    foreach (QString fileName, file.dir().entryList(filters)) {
        if (file.fileName() == fileName) {
            _initialIndex = this->size();
        }
        this->append(new ImageInfo(_directoryPath + "/" + fileName));
    }
}

/* Extraction Functions ----------------------------------------------------- */

/**
    TODO: generate thumbnails
*/
int DirectoryImageList::open()
{
    return _initialIndex;
}

/**
    TODO: delete thumbnails
*/
void DirectoryImageList::close()
{
}

/* Event Functions ---------------------------------------------------------- */

DirectoryImageList::~DirectoryImageList()
{
    close();
    for (int i = 0; i < this->size(); ++i) {
        delete this->at(i);
    }
}

/* Settings/Getters --------------------------------------------------------- */

QString DirectoryImageList::listName() { return _directoryName; }

