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
    along with this program. If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

#include <QRegularExpression>
#include <memory>
#include "filter.h"

Filter::Filter(QString pattern)
{
    _tokens = QStringList();

    // extract quoted tokens
    for (int i = 0; i<pattern.length(); i++) {
        if (pattern.at(i) == '"') {
            if (i > 0 && pattern.at(i-1) == '-') {
                i--;
            }
            for (int j = i+1; j < pattern.length(); j++) {
                if (pattern.at(j) == '"') {
                    _tokens.append(pattern.mid(i+1,j-i-1));
                    pattern.remove(i,j-i+1);
                    break;
                }
            }
        }
    }

    // extract single-word tokens
    for (QString token : pattern.split(QRegularExpression("\\s"))) {
        if (token != "") {
            _tokens.append(token);
        }
    }
}

bool Filter::match(const QString &text)
{
    for (QString token : _tokens) {
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
