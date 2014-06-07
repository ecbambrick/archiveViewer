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

#include <algorithm>
#include <exception>
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

std::shared_ptr<ImageInfo> Playlist::current() const
{
    if (_list.empty()) {
        throw std::out_of_range("Requested current item when playlist is empty.");
    }

    return _list.at(_index).second;
}

void Playlist::filter(Filter filter)
{
   _filter = filter;
   this->reload();
}

int Playlist::index() const
{
    return _list.at(_index).first;
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

bool Playlist::find(const QString &relativeFilePath)
{
    auto index = -1;
    for (auto i = 0; i < _list.length(); i++) {
        if (_list.at(i).second->relativeFilePath() == relativeFilePath) {
            index = i;
        }
    }

    if (index >= 0) {
        _index = index;
        return true;
    } else {
        return false;
    }
}

void Playlist::loops(bool value)
{
    _loops = value;
}

void Playlist::next(int steps)
{
    if (_list.isEmpty()) return;

    if (_loops) {
        _index = (_index + steps) % _list.size();
    } else {
        _index = std::min(_list.size()-1, _index + steps);
    }

    emit indexChanged();
}

void Playlist::previous(int steps)
{
    if (_list.isEmpty()) return;

    if (_loops) {
        _index = (_index >= steps)
                ? _index - steps
                : _list.size() - (_index + steps);
    } else {
        _index = std::max(0, _index - steps);
    }

    emit indexChanged();
}

void Playlist::shuffle(bool value)
{
    if (value) {
        if (_orderBy != ImageSource::RandomOrder) {
            _originalOrderBy = _orderBy;
        }
        _orderBy = ImageSource::RandomOrder;
        this->reload();
    } else {
        _orderBy = _originalOrderBy;
        this->reload();
    }
}

// ------------------------------------------------------------------ private //

void Playlist::reload()
{
    auto filePath = _list.at(_index).second->relativeFilePath();

    if (_list.empty()) {
        _list = _source->images(_filter, _sortBy, _orderBy);
        _index = 0;

    } else {
        _list = _source->images(_filter, _sortBy, _orderBy);

        // Find the item with the current image's relative file path.
        auto iterator = std::find_if(_list.begin(), _list.end(), [=](ImageSourceItem i) {
            return i.second->relativeFilePath() == filePath;
        });

        // If no image is found, go to the beginning of the list.
        _index = (iterator == _list.end())
                ? 0
                : iterator - _list.begin();
    }

    emit indexChanged();
}
