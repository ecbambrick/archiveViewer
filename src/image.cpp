#include "image.h"

/* ----------------------------------------------------------- INITIALIZATION */

Image::Image(const QString path) : QFileInfo(path)
{
    _width = 0;
    _height = 0;
    _relativePath = "";
}

/* ------------------------------------------------------------------ SETTERS */

void Image::relativePath(QString path)
{
    this->_relativePath = path;
}

/* ------------------------------------------------------------------ GETTERS */

int Image::width()
{
    return _width;
}

int Image::height()
{
    return _height;
}

QString Image::relativeName()
{
    return this->_relativePath + this->fileName();
}


QString Image::relativePath()
{
    return this->_relativePath;
}
