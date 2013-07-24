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

#include "imagelistfilter.h"

ImageListFilter::ImageListFilter(ImageList *imageList)
{
    _imageList = imageList;
    _index = 0;
}

ImageListFilter::~ImageListFilter()
{
}

/******************************************************************************/

int ImageListFilter::open()
{
    return _imageList->open();
}

void ImageListFilter::close()
{
    _imageList->close();
}

/******************************************************************************/

void ImageListFilter::shuffle()
{
}

void ImageListFilter::unshuffle()
{
}

/******************************************************************************/

void ImageListFilter::filter(QString query)
{
    this->clear();

    QStringList tokens = parseQuery(query);
    Image *image;
    bool match;

    for (int i=0; i<_imageList->size(); i++) {
        image = _imageList->at(i);
        match = true;
        foreach(QString token, tokens) {
            if (!image->name.contains(token, Qt::CaseInsensitive)) {
                match = false;
                break;
            }
        }
        if (match) {
            this->append(image);
        }
    }
}

QStringList ImageListFilter::parseQuery(QString query)
{
    QStringList tokens;

    // extract quoted tokens
    for (int i = 0; i<query.length(); i++) {
        if (query.at(i) == '"') {
            for (int j = i+1; j < query.length(); j++) {
                if (query.at(j) == '"') {
                    tokens.append(query.mid(i+1,j-i-1));
                    query = query.remove(i,j-i+1);
                    break;
                }
            }
        }
    }

    // extract single words tokens
    foreach(QString token, query.split(QRegExp("\\s"))) {
        if (token != "") {
            tokens.append(token);
        }
    }

    return tokens;
}

/******************************************************************************/

int ImageListFilter::index()
{
    return _index;
}

void ImageListFilter::setIndex(int index)
{
    _index = index;
}

QString ImageListFilter::listName()
{
    return _imageList->listName();
}
