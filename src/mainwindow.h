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
#include <QKeyEvent>
#include <QLineEdit>
#include <QSettings>
#include <QLabel>
#include <QDebug>
#include "imagelistfilter.h"
#include "archiver.h"
#include "fileio.h"
#include "imageview.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void open();
    void load(const QString path);
    void reload(ImageInfo *image);
    void previous();
    void next();
    void filter();
    void shuffle(bool value);
    void toggleZoom();
    void toggleClean();
    void fitToWidth(bool value);
    void zoomIn();
    void zoomOut();
    void zoomFull();

private:
    void initUI();
    void initFile();
    void loadSettings();
    void saveSettings();
    void setImage(ImageInfo *image);
    void updateStatusNumber(ImageInfo *image);
    void updateStatusName(ImageInfo *image);
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);

    // widgets
    QToolBar *_uiToolbar;
    ImageView *_uiView;
    QLineEdit *_uiSearch;
    QLabel *_uiFileName;
    QLabel *_uiFileNumber;
    QStatusBar *_uiStatus;

    // actions
    QAction *_actionFitToWidth;
    QAction *_actionToggleZoom;
    QAction *_actionOpen;
    QAction *_actionPrev;
    QAction *_actionNext;
    QAction *_actionShuf;
    QAction *_actionZoomIn;
    QAction *_actionZoomOut;
    QAction *_actionZoomFull;

    // settings
    QByteArray _settingsGeometry;
    QString _settingsLastOpened;
    QString _settingsLastViewed;
    bool _settingsStatusHidden;
    bool _settingsMaximized;
    bool _wasMaximized;
    int _settingsX;
    int _settingsY;

    // misc
    ImageListFilter *_imageList;
    Archiver *_archiver;
};

#endif // MAINWINDOW_H
