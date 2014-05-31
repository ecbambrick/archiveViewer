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

#include "playlist.h"

// ------------------------------------------------------------------- public //

Playlist::Playlist(std::shared_ptr<ImageSource> source)
    : _filter(Filter())
    , _index(0)
    , _list(source->images())
    , _loops(false)
    , _orderBy(ImageSource::AscendingOrder)
    , _sortBy(ImageSource::SortByFileName)
    , _source(source)
{
}

ImageInfo Playlist::current() const
{
    if (_list.empty()) {
        throw std::out_of_range("");
    }
    return _list.at(_index);
}

void Playlist::filter(Filter filter)
{
   _filter = filter;
   this->reload();
}

int Playlist::index() const
{
    return _index;
}

bool Playlist::isEmpty() const
{
    return _list.isEmpty();
}

bool Playlist::loops() const
{
    return _loops;
}

int Playlist::size() const
{
    return _list.size();
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
    if (_list.empty()) {
        _index = 0;
    } else if (_loops) {
        _index = (_index + steps) % _list.size();
    } else {
        if (_index + steps >= _list.size()) {
            _index = _list.size() - 1;
        } else {
            _index += steps;
        }
    }

    emit indexChanged();
}

void Playlist::previous(int steps)
{
    if (_list.empty()) {
        _index = 0;
    } else if (_loops) {
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

    emit indexChanged();
}

void Playlist::shuffle(bool value)
{
    if (value) {
        this->reload(_filter, _sortBy, ImageSource::RandomOrder);
    } else {
        this->reload();
    }
}

// ------------------------------------------------------------------ private //

void Playlist::reload()
{
    this->reload(_filter, _sortBy, _orderBy);
}

void Playlist::reload(const Filter &filter,
                      ImageSource::SortType sortBy,
                      ImageSource::OrderType orderBy)
{   
    if (_list.empty()) {
        _list = _source->images(filter, sortBy, orderBy);
        _index = 0;

    } else {
        QString filePath = _list.at(_index).absoluteFilePath();

        _list = _source->images(filter, sortBy, orderBy);
        auto iterator = std::find_if(_list.begin(), _list.end(), [=](ImageInfo i) {
            return i.absoluteFilePath() == filePath;
        });

        if (iterator == _list.end()) {
            _index = 0;
        } else {
            _index = iterator - _list.begin();
        }
    }

    emit indexChanged();
}
