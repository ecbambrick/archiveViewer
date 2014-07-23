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

Playlist::Playlist()
    : _source(nullptr)
{
}

std::shared_ptr<ImageInfo> Playlist::current() const
{
    if (_list.isEmpty()) {
        throw std::out_of_range("Requested current item from empty playlist.");
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
    if (_list.isEmpty()) {
        throw std::out_of_range("Requested current index from empty playlist.");
    }

    // Rather than return the playlist's index, return the item's index. This
    // is so that, even when the playlist is shuffled, the current index will
    // be the same as if it weren't shuffled.
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
    this->sort(_sortBy, orderBy);
}

void Playlist::sort(ImageSource::SortType sortBy)
{
    this->sort(sortBy, _orderBy);
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
    // Find the given relative file path in the list.
    auto i = std::find_if(_list.begin(), _list.end(), [=](ImageSourceItem a) {
        return a.second->relativeFilePath() == relativeFilePath;
    });

    // Found.
    if (i != _list.end()) {
        this->updateIndex(i - _list.begin());
        return true;
    }

    // Not found.
    return false;
}

void Playlist::loops(bool value)
{
    _loops = value;
}

void Playlist::next(int steps)
{
    if (_list.isEmpty()) return;

    int newIndex;
    if (_loops) {
        newIndex = (_index + steps) % _list.size();
    } else {
        newIndex = std::min(_list.size()-1, _index + steps);
    }

    this->updateIndex(newIndex);
}

void Playlist::previous(int steps)
{
    if (_list.isEmpty()) return;

    int newIndex;
    if (_loops) {
        newIndex = (_index >= steps)
                 ? _index - steps
                 : _list.size() - (_index + steps);
    } else {
        newIndex = std::max(0, _index - steps);
    }

    this->updateIndex(newIndex);
}

void Playlist::shuffle(bool value)
{
    // Shuffle, retaining the previous order method.
    if (value) {
        if (_orderBy != ImageSource::RandomOrder) {
            _originalOrderBy = _orderBy;
        }
        _orderBy = ImageSource::RandomOrder;
    }

    // Unshuffle.
    else {
        _orderBy = _originalOrderBy;
    }

    this->reload();
}

// ------------------------------------------------------------------ private //

void Playlist::reload()
{
    if (_source == nullptr) return;

    // Save the current file before reloading the list.
    auto filePath = (_list.isEmpty())
            ? ""
            : _list.at(_index).second->relativeFilePath();

    // Reload the list from the source.
    _list = _source->images(_filter, _sortBy, _orderBy);

    // Find the current file again if it is still in the list; otherwise, go
    // to the beginning of the list.
    if (filePath == "" || !this->find(filePath)) {
        this->updateIndex(0);
    }
}

void Playlist::updateIndex(int index)
{
    _index = index;
    emit indexChanged();
}
