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

#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPixmap>
#include <QMovie>
#include <QMenu>
#include <QLabel>
#include "imageinfo.h"

class ImageView : public QScrollArea
{
    Q_OBJECT

public:

    enum ZoomType {
         ZoomToFullSize
        ,ZoomToHeight
        ,ZoomToWidthAndHieght
        ,ZoomToWidth
    };

    explicit ImageView(QWidget *parent = 0);
    ~ImageView();
    void setImage(const ImageInfo *image);
    void clearImage();
    void updateImage();
    void toggleZoom();
    const ImageInfo *image();

public slots:
    void fitToWidth(bool val);
    void showContextMenu(const QPoint &point);
    void displayProperties();
    void zoomIn();
    void zoomOut();
    void zoom(float scale);
    void fit(ZoomType zoom);

private slots:
    void openContainingFolder();
    void openWith();
    void setAsDesktopBackground();
    void rotateLeft();
    void rotateRight();
    void copyImage();
    void deleteFile();
    void renameFile();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *e);
    bool event(QEvent *e);

private:
    void initContextMenu();

    QMenu *_contextMenu;
    ImageInfo const *_image;
    QPixmap *_pixmap;
    QMovie *_movie;
    QLabel *_label;
    QPoint _initMousePos;
    bool _fitToWindow;
    bool _fitToWidth;
    float _zoom;
};

#endif // IMAGEVIEW_H
