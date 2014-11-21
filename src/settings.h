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

#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>
#include <QSize>
#include <QPoint>

///
/// \brief The Settings class represents a set of persisted application
/// settings. Settings are stored to and retrieved from permanent storage.
///
struct Settings
{
    ///
    /// \brief Constructs a set of application settings properties using the
    /// INI format. The settings are user-specific.
    /// \param autoSave True if application settings are to be automatically
    /// saved when the object is destroyed; otherwise, false.
    ///
    Settings(bool autoSave = true);

    ///
    /// \brief Automatically saves the file if in auto save mode.
    ///
    ~Settings();

    ///
    /// \brief Saves the application settings to permanent storage.
    ///
    void save();

    ///
    /// \brief Loads the application settings from permanent storage.
    ///
    void load();

    /// True if application settings are to be automatically saved when the
    /// object is destroyed; otherwise, false.
    bool autoSave;

    /// True if hidden files are to be opened when loading from a directory;
    /// otherwise, false.
    bool includeHiddenFiles;

    /// True if system links are to be opened when loading from a directory;
    /// otherwise, false.
    bool includeSystemLinks;

    /// True if the application is maximized; otherwise, false;
    bool isWindowMaximized;

    /// The last file that was opened through the open file dialog.
    QString lastOpenedFile;

    /// The last file that was viewed.
    QString lastViewedFile;

    /// True if the application was maxmimized before going full screen;
    /// otherwise, false.
    bool wasWindowMaximized;

    /// The application window's position.
    QPoint windowPosition;

    /// The application window's size.
    QSize windowSize;
};

#endif // SETTINGS_H
