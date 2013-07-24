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

    A class for filtering and ordering an existing instance of an imageList

*******************************************************************************/

#ifndef IMAGELISTFILTER_H
#define IMAGELISTFILTER_H

#include <QObject>
#include <QStringList>
#include "imagelist.h"

class ImageListFilter : public QList<Image*>
{
public:
    ImageListFilter(ImageList *imageList);
    ~ImageListFilter();

    int open();
    void close();
    void shuffle();
    void unshuffle();
    void filter(QString query);

    int index();
    void setIndex(int index);
    QString listName();

private:
    QStringList parseQuery(QString query);
    ImageList *_imageList;
    int _index;
};

#endif // IMAGELISTFILTER_H
