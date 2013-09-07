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

#include "reversiblerand.h"
#include <QTime>
#include <QDebug>

/* ----------------------------------------------------------- INITIALIZATION */

ReversibleRand::ReversibleRand()
{
    _seed = QTime::currentTime().msec();
}

ReversibleRand::ReversibleRand(int seed)
{
    _seed = seed;
}

/* -------------------------------------------------------- SEED MODIFICATION */

/// set the seed
void ReversibleRand::setSeed(int seed)
{
    _seed = seed;
}

/// set the seed to the milliseconds of the current time
void ReversibleRand::newSeed()
{
    _seed = QTime::currentTime().msec();
}

/* ----------------------------------------------------------- SEED TRAVERSAL */

/// decrement the seed and return the hashed value
int ReversibleRand::previous()
{
    return this->hash(--_seed);
}

/// return the hashed value of the current seed
int ReversibleRand::current()
{
    return this->hash(_seed);
}

/// increment the seed and return the hashed value
int ReversibleRand::next()
{
    return this->hash(++_seed);
}

/* ------------------------------------------------------------------ HASHING */

/// hash a value
/// taken from http://stackoverflow.com/questions/664014
int ReversibleRand::hash(int value)
{
    value = ((value >> 16) ^ value) * 0x45d9f3b;
    value = ((value >> 16) ^ value) * 0x45d9f3b;
    value = ((value >> 16) ^ value);
    return value;
}
