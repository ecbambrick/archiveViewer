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

    DirectoryImageList:
    This class contains a list of images that are within a directory.

*******************************************************************************/

#ifndef DIRECTORYIMAGELIST_H
#define DIRECTORYIMAGELIST_H

#include <imagelist.h>

class DirectoryImageList : public ImageList
{
    Q_OBJECT

public:
    explicit DirectoryImageList(const QString &path);
    ~DirectoryImageList();

    // generate and remove thumbnails
    int open();
    void close();
    QString listName();

private:
    QString _directoryPath;
    QString _directoryName;
    int     _initialIndex;
};

#endif // DIRECTORYIMAGELIST_H
