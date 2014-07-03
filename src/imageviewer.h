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
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QScrollArea>
#include "imageinfo.h"

///
/// \brief The ImageViewer class represents a widget that can display an image.
/// The image can be manipulated through zooming.
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

public slots:

    ///
    /// \brief Loads the image to be displayed.
    /// \param imageInfo The image information about the image to be displayed.
    ///
    void load(std::shared_ptr<ImageInfo> imageInfo);

    ///
    /// \brief Clears the currently displayed image.
    ///
    void clear();

    ///
    /// \brief Sets the zoom behaviour.
    /// \param zoom The zoom behaviour.
    ///
    void fit(ZoomType zoom);

    ///
    /// \brief Zooms into the currently displayed image.
    ///
    void zoomIn();

    ///
    /// \brief Zooms out of the currently displayed image.
    ///
    void zoomOut();

protected:

    ///
    /// \brief Handles the mouse movement event.
    /// \param e A Mouse event.
    ///
    void mouseMoveEvent(QMouseEvent *e);

    ///
    /// \brief Handles the mouse press event.
    /// \param e A Mouse event.
    ///
    void mousePressEvent(QMouseEvent *e);

    ///
    /// \brief Handles the mouse release event.
    /// \param e A Mouse event.
    ///
    void mouseReleaseEvent(QMouseEvent *e);

    ///
    /// \brief Handles the resize event.
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
    /// \brief Scales the currently displayed image to the given width and
    /// height, keeping its aspect ratio.
    /// \param width The width to scale the image to.
    /// \param height The height to scale the image to.
    ///
    void scale(int width, int height);

    /// True if the image is scaled to fit to the screen's height; otherwise, false.
    bool _fitToHeight;

    /// True if the image is scaled to fit to the screen's width; otherwise, false.
    bool _fitToWidth;

    /// The path to the image file to be displayed.
    QString _imagePath;

    /// The label that holds a pixmap.
    QLabel _label;

    /// The position of the mouse before moving the mouse while pressed.
    QPoint _initMousePos;

    /// The pixmap for the image to be displayed.
    QPixmap _pixmap;

    /// The percentage to scale by.
    float _zoom;
};

#endif // IMAGEVIEWER_H
