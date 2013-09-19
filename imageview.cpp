#include "imageview.h"
#include <math.h>
#include <QDebug>

ImageView::ImageView(QWidget *parent) : QScrollArea(parent)
{
    // right-click menu
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    _contextMenu = new QMenu(this);
    _contextMenu->addAction("Open containing folder");
    _contextMenu->addAction("Open with...");
    _contextMenu->addSeparator();
    _contextMenu->addAction("Set as desktop background");
    _contextMenu->addAction("Rotate left");
    _contextMenu->addAction("Rotate Right");
    _contextMenu->addSeparator();
    _contextMenu->addAction("Copy");
    _contextMenu->addAction("Delete");
    _contextMenu->addAction("Rename...");
    _contextMenu->addSeparator();
    _contextMenu->addAction("Properties");
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            this, SLOT(showContextMenu(const QPoint&)));

    _fitToWindow = true;
    _pixmap = NULL;
    _movie = NULL;

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
    delete _contextMenu;
    delete _pixmap;
    delete _movie;
    delete _label;
}

/* --------------------------------------------------------------- IMAGE JUNK */

void ImageView::setImage(Image *image)
{
    if (image != _image) {
        this->clearImage();
        _image = image;
        _movie = new QMovie(image->absoluteFilePath());
        if (_movie->isValid() && _movie->frameCount() > 1) {
            _movie->start();
        } else {
            delete _movie;
            _movie = NULL;
            _pixmap = new QPixmap(image->absoluteFilePath());
        }
        this->updateImage();
        this->horizontalScrollBar()->setValue(0);
        this->verticalScrollBar()->setValue(0);
    }
}

void ImageView::clearImage()
{
    delete _pixmap;
    delete _movie;
    _pixmap = NULL;
    _movie = NULL;
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
    if (_movie != NULL) {
        _movie->setPaused(true);
        _label->setMovie(_movie);
        _movie->start();
    }
}

void ImageView::toggleZoom()
{
    _fitToWindow = !_fitToWindow;
    this->updateImage();
}

/* --------------------------------------------------------------------- MENU */

void ImageView::showContextMenu(const QPoint& point)
{
    QPoint globalPos = this->mapToGlobal(point);
    if (_contextMenu->exec(globalPos)) {
        // add code
    }
}

/* ------------------------------------------------------------------- EVENTS */

void ImageView::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) {
        _initMousePos = e->pos();
    }
}

void ImageView::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) {
        QScrollBar *vertical = this->verticalScrollBar();
        QScrollBar *horizontal = this->horizontalScrollBar();
        int x = e->pos().x() - _initMousePos.x();
        int y = e->pos().y() - _initMousePos.y();
        horizontal->setValue(horizontal->value() - x);
        vertical->setValue(vertical->value() - y);
        _initMousePos = e->pos();
    }
}

void ImageView::resizeEvent(QResizeEvent *e)
{
    this->updateImage();
}

bool ImageView::event(QEvent *e)
{
    if (e->type() == QEvent::MouseButtonDblClick) {
        this->toggleZoom();
        return true;
    }
    return QScrollArea::event(e);
}
