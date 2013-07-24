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

    MainWindow:
    Main window of the application, handles user input and image display.

*******************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QLineEdit>
#include <sevenziparchiver.h>
#include <imagelist.h>
#include <archivedimagelist.h>
#include <directoryimagelist.h>
#include <QMovie>
#include <QKeyEvent>
#include <imageListFilter.h>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    // manipulate image list
    void open();
    void filter();
    void next();
    void previous();
    void goTo(int index);
    void random();
    void imageUpdated(int index);

    // manipulate current image
    void zoomIn();
    void zoomOut();
    void zoomFit();
    void zoomFull();
    void rename();

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *e);

private:
    // initialization
    void initUI();
    void loadSettings(const QString &path);
    void saveSettings(const QString &path);

    // update currently displayed image
    void loadFile(const QString &path);
    void updateLabel();

    Ui::MainWindow  *_ui;
    QLineEdit       *_searchBox;
    QList<int>      *_shuffeHistory;
    QPixmap         *_pixmap;
    QMovie          *_animation;
    Archiver        *_archiver;
    ImageList       *_imageList;
    QString         _defaultDir;
    QString         _lastOpenedFile;
    QString         _settingsPath;
    int             _index;
    int             _numReady;
};

#endif // MAINWINDOW_H
