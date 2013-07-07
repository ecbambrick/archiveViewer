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

    ArchivedImageList:
    This class contains a list of images that are extracted from an archive.

*******************************************************************************/

#ifndef ARCHIVEDIMAGELIST_H
#define ARCHIVEDIMAGELIST_H

#include <archiver.h>
#include <imagelist.h>
#include <QFutureWatcher.h>

class ArchivedImageList : public ImageList
{
    Q_OBJECT

public:
    explicit ArchivedImageList(Archiver *archiver, const QString &path);
    ~ArchivedImageList();

    // extract and remove extracted archive contents and thumbnails
    int open();
    void close();
    void extract(int index);
    QString listName();

private:
    void removeDir(const QString &path);

    QFutureWatcher<void> *_watcher;
    Archiver *_archiver;
    QString _timeStamp;
    QString _archivePath;
    QString _extractPath;
    QString _archiveName;
};
#endif // ARCHIVEDIMAGELIST_H
