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
#include <QtConcurrentRun>
#include <QCryptographicHash>

/* -------------------------------------------------------------------------- */

/**
    Extract files from archiver in seperate thread
*/
void callExtraction(ArchivedImageList *list, QFutureWatcher<void> *watcher)
{
    for (int i = 0; i < list->size(); i++) {
        if (watcher->isCanceled()) {
            break;
        }
        list->extract(i);
    }
}

/* -------------------------------------------------------------------------- */

ArchivedImageList::ArchivedImageList(Archiver *archiver, const QString &path)
{
    // Initialize
    _archiver = archiver;
    _timeStamp = QCryptographicHash::hash(path.toUtf8(), QCryptographicHash::Md5).toHex();
    _extractPath = QDir::tempPath()+"/archiveViewer/"+_timeStamp;
    _archivePath = path;
    _archiveName = QFileInfo(path).completeBaseName();
    _watcher = new QFutureWatcher<void>(this);

    // Generate the list of images from the archive
    // note: the images are not actually extracted yet
    foreach (QString fileName, archiver->l(_archivePath)) {
        Image *newImage = new Image();
        newImage->path = _extractPath + "/";
        newImage->name = fileName;
        newImage->active = false;
        newImage->exists = false;
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
    _watcher->setFuture(QtConcurrent::run(callExtraction, this, _watcher));
    return 0;
}

/**
    Delete each file that was extracted from the archive
    TODO: delete thumbnails
*/
void ArchivedImageList::close()
{
    // stop extraction
    if (_watcher->isRunning()) {
        _watcher->cancel();
        _watcher->waitForFinished();
    }

    // Recursively delete extracted files
    removeDir(_extractPath);

    // Mark images as inactive
    for (int i = 0; i < this->size(); ++i) {
        this->at(i)->active = false;
    }
}

/**
    Extract an image from the archive, mark it as ready and emit a signal
*/
void ArchivedImageList::extract(int index)
{
    Image *image = this->at(index);

    QFile file(_extractPath+"\\"+image->name);
    if (!file.exists()) {
        _archiver->x(_archivePath, _extractPath, image->name);
    }
    image->active = true;
    image->exists = true;
    emit imageReady(index);
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
    dir.rmdir(path);
}

/* Settings/Getters --------------------------------------------------------- */

QString ArchivedImageList::listName()
{
    return _archiveName;
}
