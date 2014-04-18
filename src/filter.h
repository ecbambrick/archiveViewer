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

#ifndef FILTER_H
#define FILTER_H
#include <QString>

///
/// \brief The Filter class provides simple pattern matching. Patterns are
/// parsed into tokens of the following type:
///   - Any block of non-white space characters seperated by whitespace
///   - Any block of characters enclosed within double-quotes.
///   - Any of the above, prepended with a '-' symbol
///
/// \note Escaping double-quotes is not supported at this time.
///
/// If a token begins with a '-' character, then the token must not exist
/// in a string for that string to match the filter. Otherwise, the string must
/// contain the token to match the filter.
///
/// \example For a string to pass a filter of "red green -blue", the string must
/// contain the substrings "red" and "green" but cannot contain the substring
/// "blue".
///
class Filter
{
public:

    ///
    /// \brief Constructor.
    /// \param pattern The query string to generate tokens from.
    ///
    explicit Filter(QString pattern);

    ///
    /// \brief Determines whether or not a string passes the filter.
    /// \param text The string to check the filter against.
    /// \return True if the string matches the filter, and false otherwise.
    ///
    bool match(const QString &text);

private:

    /// The list of tokens generated from the pattern.
    QStringList _tokens;
};

#endif // FILTER_H
