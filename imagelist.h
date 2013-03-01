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

    Image:
    A structure containing information about the image file.

    ImageList:
    An abstract class for a list of images.

*******************************************************************************/

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QObject>
#include <QList>

struct Image {
    QString path;
    QString name;
    bool active;
    bool exists;
};

class ImageList : public QObject, public QList<Image*>
{
    Q_OBJECT

public:
    virtual int open() = 0;
    virtual void close() = 0;
    virtual QString listName() = 0;
};

#endif // IMAGELIST_H
