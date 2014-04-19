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
#include "reversiblerand.h"

class ImageListFilter : public QObject
{
    Q_OBJECT

public:
    ImageListFilter();
    ImageListFilter(ImageList *imageList);
    ~ImageListFilter();

    // list manipulation
    ImageInfo *filter(QString query);
    void setShuffle(bool value);
    void reset();

    // traversal
    ImageInfo* current();
    ImageInfo* previous();
    ImageInfo* next();
    ImageInfo* goTo(int index);
    ImageInfo* goTo(QString path);

    // setters
    void setList(ImageList *list);

    // getters
    QString listName();
    ImageList* list();
    bool empty();
    int index();
    int size();

private:
    void filterByTokens(QStringList tokens);
    bool containsAllTokens(QString text, QStringList tokens);
    QStringList parseQuery(QString query);

    ImageList *_originalList;
    QList<ImageInfo*> *_filteredList;
    ReversibleRand *_rRand;
    bool _shuffle;
    int _index;
};

#endif // IMAGELISTFILTER_H
