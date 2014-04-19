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
#include "filter.h"

Filter::Filter(const QString &pattern)
    : _tokens(QList<Token>())
{
    QRegularExpression regex = QRegularExpression(R"(-?"[^"]+"|[^\s]+)");
    QRegularExpressionMatchIterator i = regex.globalMatch(pattern);

    while (i.hasNext()) {
        QString tokenString = i.next().captured(0);
        Token token;

        if (tokenString.startsWith('-') && tokenString.size() > 1) {
            token.type = Negative;
            tokenString.remove(0,1);
        } else {
            token.type = Positive;
        }

        if (tokenString.startsWith('"') && tokenString.endsWith('"')) {
            tokenString = tokenString.mid(1, tokenString.size()-2);
        }

        token.value = tokenString;
        _tokens.append(token);
    }
}

bool Filter::match(const QString &text)
{
    for (const Token &token : _tokens) {
        if (token.type == Negative) {
            if (text.contains(token.value, Qt::CaseInsensitive)) {
                return false;
            }
        } else if (!text.contains(token.value, Qt::CaseInsensitive)) {
            return false;
        }
    }

    return true;
}
