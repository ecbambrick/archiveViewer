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

#include "archiver.h"
#include <QDebug>
#include <QCoreApplication>

/**
    TODO: figure out a better way to access 7z.exe
*/
Archiver::Archiver(QObject *parent) : QObject(parent)
{
    _exePath = QCoreApplication::applicationDirPath() + "/7z.exe";
    _process = new QProcess(parent);
}

/* Archive Reading Functions ------------------------------------------------ */

/**
    Get the list of files in the archive
*/
QStringList Archiver::l(QString source)
{
    // Create arguments
    QStringList args;
    args << "l" << source;

    // Create filter
    QRegExp filter = QRegExp("^.*(jpg|jpeg|png|gif)$");
    filter.setCaseSensitivity(Qt::CaseInsensitive);

    // Grab the process output as a list of strings
    _process->start(_exePath, args);
    _process->waitForFinished();
    QStringList lines = ((QString) _process->readAll()).split("\n");

    // parse through the output to find file names
    QStringList files;
    foreach(QString line, lines) {
        if (line.contains(QRegExp("^[0-9-]+\\s+[0-9:]+\\s+.+$"))) {
            QStringList args = line.split(QRegExp("\\s+"));
            QString name = "";
            for (int i=5; i<args.length(); i++) {
                name += args.at(i) + " ";
            }
            name.chop(2);
            if (name.contains(filter)) {
                files << name;
            }
        }
    }
    files.sort();
    return files;
}

/* Archive Extraction Functions --------------------------------------------- */

/**
    Extract a single file from the archive to a destination folder,
    ignoring the file structure of the archive
*/
bool Archiver::e(QString source, QString destination, QString fileName)
{
    QStringList parameters;
    int exitCode;

    parameters << "e" << source << fileName << "-o"+destination << "-y";
    exitCode = _process->execute(_exePath, parameters);

    if (exitCode == 0) return true;
    else return false;
}

/**
    Extract the entire archive to a destination folder, ignoring file structure
*/
bool Archiver::e(QString source, QString destination)
{
    QStringList parameters;
    int exitCode;

    parameters << "e" << source << "-o"+destination << "-y";
    exitCode = _process->execute(_exePath, parameters);

    if (exitCode == 0) return true;
    else return false;
}

/**
    Extract a single file from the archive to a destination folder,
    keeping the file structure of the archive
*/
bool Archiver::x(QString source, QString destination, QString fileName)
{
    QStringList parameters;
    int exitCode;

    parameters << "x" << source << fileName << "-o"+destination << "-y";
    exitCode = _process->execute(_exePath, parameters);

    if (exitCode == 0) return true;
    else return false;
}


/**
    Extract the entire archive to a destination folder, keeping file structure
*/
bool Archiver::x(QString source, QString destination)
{
    QStringList parameters;
    int exitCode;

    parameters << "x" << source << "-o"+destination << "=y";
    exitCode = _process->execute(_exePath, parameters);

    if (exitCode == 0) return true;
    else return false;
}

/* Event Functions ---------------------------------------------------------- */

Archiver::~Archiver()
{
    delete _process;
}
