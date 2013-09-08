#include "imageview.h"
#include <math.h>
#include <QDebug>

ImageView::ImageView(QWidget *parent) : QScrollArea(parent)
{
    _fitToWindow = true;
    _pixmap = NULL;

    _label = new QLabel(this);
    _label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _label->setAlignment(Qt::AlignCenter);
    _label->setPalette(QPalette(QColor(255,255,255,255)));

    this->setWidget(_label);
    this->setWidgetResizable(true);
    this->setFrameShape(QScrollArea::NoFrame);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ImageView::~ImageView()
{
    delete _label;
}

/* --------------------------------------------------------------- IMAGE JUNK */

void ImageView::setImage(Image *image)
{
    if (image != _image) {
        _image = image;
        _pixmap = new QPixmap(_image->path + _image->name);
        this->updateImage();
        this->horizontalScrollBar()->setValue(0);
        this->verticalScrollBar()->setValue(0);
    }
}

void ImageView::clearImage()
{
    delete _pixmap;
    _pixmap = NULL;
    _image = NULL;
    _label->clear();
}

void ImageView::updateImage()
{
    if (_pixmap != NULL) {
        if (_fitToWindow && (
               _pixmap->width() > this->width()
            || _pixmap->height() > this->height())) {
            _label->setPixmap(_pixmap->scaled(
                                  this->size(),
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation));
        } else {
            _label->setPixmap(*_pixmap);
        }
    }
}

void ImageView::toggleZoom()
{
    _fitToWindow = !_fitToWindow;
    this->updateImage();
}

/* ------------------------------------------------------------------- EVENTS */

void ImageView::mousePressEvent(QMouseEvent *e)
{
    _initMousePos = e->pos();
}

void ImageView::mouseMoveEvent(QMouseEvent *e)
{
    QScrollBar *vertical = this->verticalScrollBar();
    QScrollBar *horizontal = this->horizontalScrollBar();
    int x = e->pos().x() - _initMousePos.x();
    int y = e->pos().y() - _initMousePos.y();
    horizontal->setValue(horizontal->value() - x);
    vertical->setValue(vertical->value() - y);
    _initMousePos = e->pos();
}

void ImageView::resizeEvent(QResizeEvent *e)
{
    if (_pixmap != NULL) {
        if (_fitToWindow) {
            this->updateImage();
        }
        _label->setPixmap(*_label->pixmap());
    }
}
