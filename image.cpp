#include "image.h"

/* ----------------------------------------------------------- INITIALIZATION */

Image::Image(const QString path) : QFileInfo(path)
{
    _width = 0;
    _height = 0;
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
