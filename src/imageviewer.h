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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <memory>
#include <QElapsedTimer>
#include <QFutureWatcher>
#include <QLabel>
#include <QMouseEvent>
#include <QMovie>
#include <QPixmap>
#include <QScrollArea>
#include "imageinfo.h"

///
/// \brief The ImageViewer class represents a widget that can load and display
/// an image from a file. The image can be manipulated through zooming.
///
class ImageViewer : public QScrollArea
{
public:

    ///
    /// \brief The ZoomType enum represents zoom behaviours.
    ///
    enum ZoomType {
         ZoomToFullSize
        ,ZoomToHeight
        ,ZoomToWidthAndHieght
        ,ZoomToWidth
    };

    ///
    /// \brief Constructs an image viewer.
    /// \param parent The parent widget.
    ///
    ImageViewer(QWidget *parent = 0);

    ///
    /// \brief Destructor.
    ///
    ~ImageViewer();

public slots:

    ///
    /// \brief Loads an image to be displayed.
    /// \param filePath The file path to the image.
    ///
    void load(const QString &filePath);

    ///
    /// \brief Clears and unloads the currently displayed image.
    ///
    void clear();

    ///
    /// \brief Sets the zoom behaviour.
    /// \param zoom The zoom behaviour.
    ///
    void fit(ZoomType zoomType);

    ///
    /// \brief Zooms in on the currently displayed image.
    ///
    void zoomIn();

    ///
    /// \brief Zooms out on the currently displayed image.
    ///
    void zoomOut();

protected:

    ///
    /// \brief Handles the mouse movement event. Pans the image while the left
    /// mouse button is down.
    /// \param e A Mouse event.
    ///
    void mouseMoveEvent(QMouseEvent *e);

    ///
    /// \brief Handles the mouse press event. Updates the position of the mouse
    /// which is used when panning the image.
    /// \param e A Mouse event.
    ///
    void mousePressEvent(QMouseEvent *e);

    ///
    /// \brief Handles the mouse release event. Unhides the mouse cursor after
    /// panning.
    /// \param e A Mouse event.
    ///
    void mouseReleaseEvent(QMouseEvent *e);

    ///
    /// \brief Handles the resize event. Resizes the displayed image depending
    /// on the zoom type.
    /// \param e A resize event.
    ///
    void resizeEvent(QResizeEvent *e);

private:

    ///
    /// \brief Refreshes the currently displayed image to account for any
    /// new transformations.
    ///
    void refresh();

    ///
    /// \brief Refreshes the currently displayed non-animated image.
    /// \param size The size to scale to.
    ///
    void refreshImage(const QSize &size);

    ///
    /// \brief Refreshes the currently displayed animated image.
    /// \param size The size to scale to.
    ///
    void refreshMovie(const QSize &size);

    ///
    /// \brief Scales the currently displayed image to the given width and
    /// height, keeping its aspect ratio.
    /// \param size The size to scale the image to.
    ///
    void scale(QSize size);

    ///
    /// \brief Smooths the currently displayed image.
    ///
    void smooth();

    ///
    /// \brief Cancels the smoothing process.
    ///
    void cancelSmooth();

    /// True if the image is scaled to fit to the viewer's height; otherwise, false.
    bool _fitToHeight;

    /// True if the image is scaled to fit to the viewer's width; otherwise, false.
    bool _fitToWidth;

    /// The label that holds the pixmap.
    QLabel _label;

    /// The initial position of the mouse before panning the image.
    QPoint _initialMousePosition;

    /// The movie for an animated image.
    std::unique_ptr<QMovie> _movie;

    /// The original size for an animated image.
    QSize _movieSize;

    /// The pixmap for a non-animated image.
    QPixmap _pixmap;

    /// The future watcher for the smoothing process.
    std::unique_ptr<QFutureWatcher<void>> _smoothWatcher;

    /// The percentage to scale the image by.
    float _zoom;
};

#endif // IMAGEVIEWER_H
