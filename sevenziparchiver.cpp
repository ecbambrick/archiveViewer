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

#include "SevenZipArchiver.h"
#include <QDebug>
#include <QCoreApplication>

/**
    TODO: use a zip library instead of calling through a seperate process
*/
SevenZipArchiver::SevenZipArchiver(QObject *parent)
{
    _process = new QProcess(parent);
    _processName = "7z";
}

/* Archive Reading Functions ------------------------------------------------ */

/**
    Get the list of files in the archive
*/
QStringList SevenZipArchiver::l(QString source)
{
    QStringList args;
    QStringList output;
    QStringList fileNames;
    QRegExp filter = QRegExp("^.*(jpg|jpeg|png|gif)$");
    filter.setCaseSensitivity(Qt::CaseInsensitive);

    // get list of files from 7z process
    args << "l" << source;
    _process->start(_processName, args);
    _process->waitForFinished();

    // parse output for exact file names
    output = ((QString) _process->readAll()).split("\n");
    foreach(QString line, output) {
        if (line.contains(QRegExp("^[0-9-]+\\s+[0-9:]+\\s+.+$"))) {
            QStringList args = line.split(QRegExp("\\s+"));
            QString name = "";
            for (int i=5; i<args.length(); i++) {
                name += args.at(i) + " ";
            }
            name.chop(2);
            if (name.contains(filter)) {
                fileNames << name;
            }
        }
    }

    fileNames.sort();
    return fileNames;
}

/* Archive Extraction Functions --------------------------------------------- */

/**
    Extract a single file from the archive to a destination folder,
    ignoring the file structure of the archive
*/
bool SevenZipArchiver::e(QString source, QString destination, QString fileName)
{
    QStringList parameters;
    int exitCode = 1;

    parameters << "e" << source << fileName << "-o"+destination << "-y";
    exitCode = _process->execute(_processName, parameters);

    if (exitCode == 0)  return true;
    else                return false;
}

/**
    Extract the entire archive to a destination folder, ignoring file structure
*/
bool SevenZipArchiver::e(QString source, QString destination)
{
    QStringList parameters;
    int exitCode;

    parameters << "e" << source << "-o"+destination << "-y";
    exitCode = _process->execute(_processName, parameters);

    if (exitCode == 0)  return true;
    else                return false;
}

/**
    Extract a single file from the archive to a destination folder,
    keeping the file structure of the archive
*/
bool SevenZipArchiver::x(QString source, QString destination, QString fileName)
{
    QStringList parameters;
    int exitCode;

    parameters << "x" << source << fileName << "-o"+destination << "-y";
    exitCode = _process->execute(_processName, parameters);

    if (exitCode == 0)  return true;
    else                return false;
}


/**
    Extract the entire archive to a destination folder, keeping file structure
*/
bool SevenZipArchiver::x(QString source, QString destination)
{
    QStringList parameters;
    int exitCode;

    parameters << "x" << source << "-o"+destination << "=y";
    exitCode = _process->execute(_processName, parameters);

    if (exitCode == 0)  return true;
    else                return false;
}

/* Event Functions ---------------------------------------------------------- */

SevenZipArchiver::~SevenZipArchiver()
{
    delete _process;
}

/* Settings/Getters --------------------------------------------------------- */

QString SevenZipArchiver::program() { return _processName; }
