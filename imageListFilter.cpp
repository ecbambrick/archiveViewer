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
#include "directoryimagelist.h"
#include <QDebug>

/* ----------------------------------------------------------- INITIALIZATION */

ImageListFilter::ImageListFilter()
{
    _originalList = NULL;
    _filteredList = new QList<Image*>;
    _rRand = new ReversibleRand();
    _shuffle = false;
    _index = 0;
}

ImageListFilter::ImageListFilter(ImageList *imageList)
{
    _filteredList = new QList<Image*>;
    _rRand = new ReversibleRand();
    _shuffle = false;
    _index = 0;
    this->setList(imageList);
}

ImageListFilter::~ImageListFilter()
{
    delete _filteredList;
    delete _rRand;
}

/* ---------------------------------------------------------------- TRAVERSAL */

/// return the current image
Image* ImageListFilter::current()
{
    if (_filteredList->size() == 0) return NULL;
    return _filteredList->at(_index);
}

/// decrement the index return the previous image in the list
Image* ImageListFilter::previous()
{
    if (_filteredList->size() == 0) return NULL;

    if (_shuffle) {
        _index = _rRand->previous() % _filteredList->size();
    } else {
        _index = ((_index > 0) ? _index - 1 : _filteredList->size() - 1);
    }
    return _filteredList->at(_index);
}

/// increment the index return the next image in the list
Image* ImageListFilter::next()
{
    if (_filteredList->size() == 0) return NULL;

    if (_shuffle) {
        _index = _rRand->next() % _filteredList->size();
    } else {
        _index = (_index + 1) % _filteredList->size();
    }
    return _filteredList->at(_index);
}

/// set the index and return the image at that position
Image* ImageListFilter::goTo(int index)
{
    if (index < 0 || index >= _filteredList->size()) return NULL;

    _index = index;
    return _filteredList->at(_index);
}

/// find the image with the same filename and set the index to that position
Image* ImageListFilter::goTo(QString fileName)
{
    if (_filteredList->empty()) return NULL;

    // get index of image with fileName
    _index = 0;
    for (int i = 0; i < _filteredList->size(); i++) {
        Image *image = _filteredList->at(i);
        if (image->fileName() == fileName) {
            _index = i;
        }
    }
    return _filteredList->at(_index);
}

/* ---------------------------------------------------------------- SHUFFLING */

/// toggle shuffling (ie - next()/previous() will go to random positions)
void ImageListFilter::setShuffle(bool value)
{
    _shuffle = value;
    if (_shuffle) {
        _rRand->newSeed();
    }
}

/* ---------------------------------------------------------------- FILTERING */

/// refill the list with all entries from the original list
void ImageListFilter::reset()
{
    if (_originalList != NULL) {
        _filteredList->clear();
        for(int i = 0; i < _originalList->size(); i++) {
            _filteredList->append(_originalList->at(i));
        }
    }
}

/// filter the list based on a query
Image *ImageListFilter::filter(QString query)
{
    Image *originalImage = this->current();

    if (query == "") {
        this->reset();
    } else if (_originalList != NULL) {
        this->filterByTokens(parseQuery(query));
    }

    // find the image that was being viewed before filtering
    // if the image has been filtered, go to the first image in the list
    if (originalImage != NULL && !_filteredList->empty()) {
        this->goTo(originalImage->fileName());
    } else {
        _index = 0;
    }

    return this->current();
}

/// clear the image list and add back images that contain all the tokens
void ImageListFilter::filterByTokens(QStringList tokens)
{
    Image *image;
    _filteredList->clear();
    for (int i=0; i<_originalList->size(); i++) {
        image = _originalList->at(i);
        if (containsAllTokens(image->fileName(), tokens)) {
            _filteredList->append(image);
        }
    }
}

/// return true if the text contains all of the tokens
bool ImageListFilter::containsAllTokens(QString text, QStringList tokens)
{
    foreach(QString token, tokens) {
        if (token.startsWith("-")) {
            if (text.contains(token.mid(1), Qt::CaseInsensitive)) {
                return false;
            }
        } else if (!text.contains(token, Qt::CaseInsensitive)) {
            return false;
        }
    }
    return true;
}

/// parse a query into a list of tokens to check against
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

/* ------------------------------------------------------ SETTERS AND GETTERS */

void ImageListFilter::setList(ImageList *list)
{
    _originalList = list;
    _index = 0;
    this->reset();
}

bool ImageListFilter::empty()
{
    return _filteredList->empty();
}

int ImageListFilter::size()
{
    return _filteredList->size();
}

int ImageListFilter::index()
{
    return _index;
}

ImageList* ImageListFilter::list()
{
    return _originalList;
}

QString ImageListFilter::listName()
{
    if (_originalList == NULL) {
        return NULL;
    } else {
        return _originalList->listName();
    }
}
