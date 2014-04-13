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

    A traversable random number generator. Maintains an index and returns
    a random number using the index as a seed.

    Allows for going back and forth between random numbers.

*******************************************************************************/

#ifndef REVERSIBLERAND_H
#define REVERSIBLERAND_H

class ReversibleRand
{
public:
    ReversibleRand();
    ReversibleRand(int seed);
    void setSeed(int seed);
    void newSeed();
    int previous();
    int current();
    int next();
private:
    int hash(int value);
    int _seed;
};

#endif // REVERSIBLERAND_H
