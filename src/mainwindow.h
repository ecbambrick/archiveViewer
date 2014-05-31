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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QSettings>
#include <QStatusBar>
#include <QToolBar>
#include "imagesource.h"
#include "imageview.h"
#include "localimagesource.h"
#include "playlist.h"
#include "quazipimagesource.h"

///
/// \brief The MainWindow class represents the main window of the application.
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void filter();
    bool open();
    bool open(const QString &filePath);
    void loadImage();
    void reloadImage(int id);
    void updateFileName();
    void updateFilePosition();
    void zoomFit();

signals:

    void imageLoaded(const ImageInfo *image);

protected:

    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:

    ///
    /// \brief loadUI
    ///
    void loadActions();

    ///
    /// \brief loadGeometry
    ///
    void loadGeometry();

    ///
    /// \brief loadInitialFile
    /// \return
    ///
    bool loadInitialFile();

    ///
    /// \brief loadStyleSheet
    /// \return
    ///
    bool loadStyleSheet();

    ///
    /// \brief loadUI
    ///
    void loadWidgets();

    QAction *_actionOpen;           ///< Open file.
    QAction *_actionPrevious;       ///< Go to previous image.
    QAction *_actionNext;           ///< Go to next image.
    QAction *_actionShuffle;        ///< Shuffle the images.
    QAction *_actionZoomFit;        ///< Zoom to full, width, or width/height.
    QAction *_actionZoomIn;         ///< Zoom in.
    QAction *_actionZoomOut;        ///< Zoom out.

    std::shared_ptr<ImageSource> _imageSource;      ///< Source of image files.
    std::unique_ptr<Playlist> _playlist;            ///< Playlist of images.
    std::unique_ptr<QSettings> _settings;           ///< Application settings.
    bool _wasMaximized;                              ///< True if the application was maximized before becoming fullscreen; otherwise, false.

    QWidget *_widgetSpacer;         ///< Expanding, empty spacer.
    QToolBar *_widgetToolBar;       ///< Status bar.
    QStatusBar *_widgetStatusBar;   ///< Tool bar.
    QLineEdit *_widgetSearchBox;    ///< Search box.
    QLabel *_widgetFileName;        ///< File name label.
    QLabel *_widgetFilePosition;    ///< File position label.
    ImageView *_widgetImageView;    ///< Image viewing widget.
};

#endif // MAINWINDOW_H
