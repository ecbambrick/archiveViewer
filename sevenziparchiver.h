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

    Archiver:
    Class for managing an external archiving tool. Handles reading and
    extracting files from an archive. Uses 7zip.

*******************************************************************************/

#ifndef SEVENZIPARCHIVER_H
#define SEVENZIPARCHIVER_H

#include <QObject>
#include <QProcess>
#include <archiver.h>

class SevenZipArchiver : public Archiver
{
    Q_OBJECT

public:
    explicit SevenZipArchiver(QObject *parent = 0);
    ~SevenZipArchiver();

    // Archive commands
    bool e(QString source, QString destination, QString fileName);
    bool e(QString source, QString destination);
    bool x(QString source, QString destination, QString fileName);
    bool x(QString source, QString destination);
    QStringList l(QString source);

    // Settings/Getters
    QString program();

private:
    QString getProgram();

    QProcess *_process;
    QString _program;
    
};

#endif // SEVENZIPARCHIVER_H
