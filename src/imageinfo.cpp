#include "imageinfo.h"

/* ----------------------------------------------------------- INITIALIZATION */

ImageInfo::ImageInfo(const QString path) : QFileInfo(path)
{
    _width = 0;
    _height = 0;
    _relativePath = "";
}

/* ------------------------------------------------------------------ SETTERS */

void ImageInfo::relativePath(QString path)
{
    this->_relativePath = path;
}

/* ------------------------------------------------------------------ GETTERS */

int ImageInfo::width()
{
    return _width;
}

int ImageInfo::height()
{
    return _height;
}

QString ImageInfo::relativeName()
{
    return this->_relativePath + this->fileName();
}


QString ImageInfo::relativePath()
{
    return this->_relativePath;
}
