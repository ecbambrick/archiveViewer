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
    TODO: figure out a better way to access 7z.exe
*/
SevenZipArchiver::SevenZipArchiver(QObject *parent)
{
    _process = new QProcess(parent);
    _program = getProgram();
}

/**
    Returns the system archiving program if it exists
*/
QString SevenZipArchiver::getProgram()
{
    int success;

    // check for 7zip
    _process->start("7z");
    _process->waitForStarted();
    success = (int) _process->pid();
    _process->waitForFinished();
    if (success) { return "7z"; }

    // else, no driver found
    return NULL;

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

    if (_program == "7z") {
        args << "l" << source;
        _process->start(_program, args);
        _process->waitForFinished();
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

    if (_program == "7z") {
        parameters << "e" << source << fileName << "-o"+destination << "-y";
        exitCode = _process->execute(_program, parameters);
    }

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


    if (_program == "7z") {
        parameters << "e" << source << "-o"+destination << "-y";
        exitCode = _process->execute(_program, parameters);
    }

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

    if (_program == "7z") {
        parameters << "x" << source << fileName << "-o"+destination << "-y";
        exitCode = _process->execute(_program, parameters);
    }

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

    if (_program == "7z") {
        parameters << "x" << source << "-o"+destination << "=y";
        exitCode = _process->execute(_program, parameters);
    }

    if (exitCode == 0)  return true;
    else                return false;
}

/* Event Functions ---------------------------------------------------------- */

SevenZipArchiver::~SevenZipArchiver()
{
    delete _process;
}

/* Settings/Getters --------------------------------------------------------- */

QString SevenZipArchiver::program() { return _program; }
