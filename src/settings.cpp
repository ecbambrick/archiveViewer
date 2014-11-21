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

#include "settings.h"

const QString applicationName = "Archive Viewer";
const QSettings::Format format = QSettings::IniFormat;
const QSettings::Scope scope = QSettings::UserScope;

Settings::Settings(bool autoSave)
{
    this->autoSave = autoSave;
    this->load();
}

Settings::~Settings()
{
    if (autoSave) {
        this->save();
    }
}

void Settings::load()
{
    QSettings settings(format, scope, applicationName, applicationName);

    settings.beginGroup("Window");
    windowSize = settings.value("size", QSize(640,480)).toSize();
    windowPosition = settings.value("position", QPoint(200,200)).toPoint();
    isWindowMaximized = settings.value("isMaximized", false).toBool();
    wasWindowMaximized = settings.value("wasMaximized", false).toBool();
    settings.endGroup();

    settings.beginGroup("LastSession");
    lastOpenedFile = settings.value("openedFile", "").toString();
    lastViewedFile = settings.value("viewedFile", "").toString();
    settings.endGroup();

    settings.beginGroup("Preferences");
    includeHiddenFiles = settings.value("includeHiddenFiles", false).toBool();
    includeSystemLinks = settings.value("includeSystemLinks", false).toBool();
    settings.endGroup();
}

void Settings::save()
{
    QSettings settings(format, scope, applicationName, applicationName);

    settings.beginGroup("Window");
    settings.setValue("size", windowSize);
    settings.setValue("position", windowPosition);
    settings.setValue("isMaximized", isWindowMaximized);
    settings.setValue("wasMaximized", wasWindowMaximized);
    settings.endGroup();

    settings.beginGroup("LastSession");
    settings.setValue("openedFile", lastOpenedFile);
    settings.setValue("viewedFile", lastViewedFile);
    settings.endGroup();

    settings.beginGroup("Preferences");
    settings.setValue("includeHiddenFiles", includeHiddenFiles);
    settings.setValue("includeHiddenFiles", includeSystemLinks);
    settings.endGroup();
}
