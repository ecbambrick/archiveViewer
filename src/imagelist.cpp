//#include "imagelist.h"
//#include <QDir>
//#include <QDateTime>
//#include <QMessageBox>
//#include <QDebug>

///*

//    put extraction stuff in seperate thread or something

///* Construction ------------------------------------------------------------- */

//ImageList::ImageList(Archiver *archiver, const QString &path)
//{
//    // Initialize
//    _archiver = archiver;
//    _timeStamp = QString::number(QDateTime::currentMSecsSinceEpoch());
//    _extractPath = QDir::tempPath()+"/archiveViewer/"+_timeStamp;
//    _archivePath = path;
//    _archiveName = QFileInfo(path).completeBaseName();

//    // Generate the list of images from the archive
//    // note: the images are not actually extracted yet
//    foreach (QString fileName, archiver->l(_archivePath)) {
//        Image *newImage = new Image();
//        newImage->path = _extractPath + "/";
//        newImage->name = fileName;
//        newImage->active = false;
//        this->append(newImage);
//    }
//}

///* Extraction --------------------------------------------------------------- */

///// Extract each file from the archive
//void ImageList::open()
//{
//    for (int i = 0; i < this->size(); ++i) {
//        _archiver->e(_archivePath, _extractPath, this->at(i)->name);
//        this->at(i)->active = true;
//    }
//}

///// Delete each extracted file
//void ImageList::close()
//{
//    // Recursively delete extracted files
//    removeDir(_extractPath);

//    // Mark images as inactive
//    for (int i = 0; i < this->size(); ++i) {
//        this->at(i)->active = false;
//    }
//}

///* Events ------------------------------------------------------------------- */

//ImageList::~ImageList()
//{
//    close();
//    qDebug() << "deleted all files";
//    for (int i = 0; i < this->size(); ++i) {
//        delete this->at(i);
//    }
//}


///* Helper Functions --------------------------------------------------------- */

///// Remove a directory and all of its contents
//void ImageList::removeDir(const QString &path)
//{
//    // paranoid check
//    if (!path.startsWith(QDir::tempPath()+"/archiveViewer/")) {
//        QMessageBox msgBox;
//        msgBox.setText("Oh god, why is it deleting a non-temp thing?");
//        msgBox.exec();
//        return;
//    }

//    // Delete extracted files
//    QDir dir(path);
//    foreach(QFileInfo info, dir.entryInfoList(
//                QDir::NoDotAndDotDot | QDir::System | QDir::Hidden
//                | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
//        qDebug() << info.absoluteFilePath();
//        if (info.isDir()) {
//            removeDir(info.absoluteFilePath());
//        } else {
//            QFile::remove(info.absoluteFilePath());
//        }
//    }
//    dir.rmdir(_extractPath);
//}

///* Settings/Getters --------------------------------------------------------- */

///// Return the archive name
//QString ImageList::listName()
//{
//    return _archiveName;
//}
