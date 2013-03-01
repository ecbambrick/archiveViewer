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

#include "archivedimagelist.h"
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

ArchivedImageList::ArchivedImageList(Archiver *archiver, const QString &path)
{
    // Initialize
    _archiver = archiver;
    _timeStamp = QString::number(QDateTime::currentMSecsSinceEpoch());
    _extractPath = QDir::tempPath()+"/archiveViewer/"+_timeStamp;
    _archivePath = path;
    _archiveName = QFileInfo(path).completeBaseName();

    // Generate the list of images from the archive
    // note: the images are not actually extracted yet
    foreach (QString fileName, archiver->l(_archivePath)) {
        Image *newImage = new Image();
        newImage->path = _extractPath + "/";
        newImage->name = fileName;
        newImage->active = false;
        this->append(newImage);
    }
}

/* Extraction Functions ----------------------------------------------------- */

/**
    Extract each file from the archive
    TODO: Generate thumbnails
*/
int ArchivedImageList::open()
{
    for (int i = 0; i < this->size(); ++i) {
        _archiver->e(_archivePath, _extractPath, this->at(i)->name);
        this->at(i)->active = true;
    }
    return 0;
}

/**
    Delete each file that was extracted from the archive
    TODO: delete thumbnails
*/
void ArchivedImageList::close()
{
    // Recursively delete extracted files
    removeDir(_extractPath);

    // Mark images as inactive
    for (int i = 0; i < this->size(); ++i) {
        this->at(i)->active = false;
    }
}

/* Event Functions ---------------------------------------------------------- */

ArchivedImageList::~ArchivedImageList()
{
    close();
    for (int i = 0; i < this->size(); ++i) {
        delete this->at(i);
    }
}

/* Helper Functions --------------------------------------------------------- */

/**
    Remove a directory and all of its contents
*/
void ArchivedImageList::removeDir(const QString &path)
{
    // paranoid check
    if (!path.startsWith(QDir::tempPath()+"/archiveViewer/")) {
        QMessageBox msgBox;
        msgBox.setText("Attempted to delete a non-temp folder. Aborting.");
        msgBox.exec();
        return;
    }

    // Delete extracted files
    QDir dir(path);
    foreach(QFileInfo info, dir.entryInfoList(
                QDir::NoDotAndDotDot | QDir::System | QDir::Hidden
                | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (info.isDir()) {
            removeDir(info.absoluteFilePath());
        } else {
            QFile::remove(info.absoluteFilePath());
        }
    }
    dir.rmdir(_extractPath);
}

/* Settings/Getters --------------------------------------------------------- */

QString ArchivedImageList::listName()
{
    return _archiveName;
}
