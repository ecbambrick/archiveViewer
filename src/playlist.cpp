/*******************************************************************************

    Copyright (C) 2014 by Cole Bambrick
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

#include <QDebug>
#include "playlist.h"

// ------------------------------------------------------------------- public //

Playlist::Playlist(ImageSource *source)
    : _index(0)
    , _list(source->images())
    , _loops(false)
    , _orderBy(ImageSource::AscendingOrder)
    , _sortBy(ImageSource::SortByFileName)
    , _source(source)
{
}

Playlist::~Playlist()
{
}

ImageInfo Playlist::current()
{
    if (_list.empty()) {
        throw std::out_of_range("");
    }
    return _list.at(_index);
}

int Playlist::index()
{
    return _index;
}

bool Playlist::isEmpty()
{
    return _list.isEmpty();
}

bool Playlist::loops()
{
    return _loops;
}

void Playlist::sort(ImageSource::OrderType orderBy)
{
    _orderBy = orderBy;
    this->reload();
}

void Playlist::sort(ImageSource::SortType sortBy)
{
    _sortBy = sortBy;
    this->reload();
}

void Playlist::sort(ImageSource::SortType sortBy, ImageSource::OrderType orderBy)
{
    _orderBy = orderBy;
    _sortBy = sortBy;
    this->reload();
}

// ------------------------------------------------------------- public slots //

void Playlist::index(int position)
{
    if (position >= _list.size() || position < 0) {
        throw std::out_of_range("position");
    }
    _index = position;
}

void Playlist::loops(bool value)
{
    _loops = value;
}

void Playlist::next(int steps)
{
    if (_loops) {
        _index = (_index + steps) % _list.size();
    } else {
        if (_index + steps >= _list.size()) {
            _index = _list.size() - 1;
        } else {
            _index += steps;
        }
    }
}

void Playlist::previous(int steps)
{
    if (_loops) {
        if (_index >= steps) {
            _index -= steps;
        } else {
            _index = _list.size() - (steps - _index);
        }
    } else {
        if (_index >= steps) {
            _index -= steps;
        } else {
            _index = 0;
        }
    }
}

// ------------------------------------------------------------------ private //

void Playlist::reload()
{
    if (_list.empty()) {
        _list = _source->images(Filter(""), _sortBy, _orderBy);
        _index = 0;

    } else {
        QString filePath = _list.at(_index).absoluteFilePath();

        _list = _source->images(Filter(""), _sortBy, _orderBy);
        auto iterator = std::find_if(_list.begin(), _list.end(), [=](ImageInfo i) {
            return i.absoluteFilePath() == filePath;
        });

        if (iterator == _list.end()) {
            _index = 0;
        } else {
            _index = iterator - _list.begin();
        }
    }
}
