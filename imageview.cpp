#include "imageview.h"
#include "fileio.h"
#include <math.h>
#include <QDebug>

ImageView::ImageView(QWidget *parent) : QScrollArea(parent)
{
    // initialize
    this->initContextMenu();
    _fitToWindow = true;
    _fitToWidth = false;
    _pixmap = NULL;
    _movie = NULL;

    // set label properties
    _label = new QLabel(this);
    _label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _label->setAlignment(Qt::AlignCenter);
    _label->setPalette(QPalette(QColor(255,255,255,255)));

    // set widget properties
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
        } else if (_fitToWidth && _pixmap->width() > this->width()) {
            _label->setPixmap(_pixmap->scaled(
                this->width(),
                _pixmap->height(),
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

void ImageView::fitToWidth(bool val)
{
    _fitToWidth = val;
    this->updateImage();
}

/* ------------------------------------------------------------- CONTEXT MENU */

void ImageView::initContextMenu()
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    _contextMenu = new QMenu(this);
    _contextMenu->addAction("Open containing folder", this, SLOT(openContainingFolder()));
    _contextMenu->addAction("Open With...", this, SLOT(openWith()));
    _contextMenu->addSeparator();
    _contextMenu->addAction("Set as desktop background", this, SLOT(setAsDesktopBackground()));
    _contextMenu->addAction("Rotate left", this, SLOT(rotateLeft()));
    _contextMenu->addAction("Rotate Right", this, SLOT(rotateRight()));
    _contextMenu->addSeparator();
    _contextMenu->addAction("Copy Image", this, SLOT(copyImage()));
    _contextMenu->addAction("Delete File", this, SLOT(deleteFile()));
    _contextMenu->addAction("Rename...", this, SLOT(renameFile()));
    _contextMenu->addSeparator();
    _contextMenu->addAction("Properties", this, SLOT(displayProperties()));
    this->connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
                  this, SLOT(showContextMenu(const QPoint&)));
}

void ImageView::showContextMenu(const QPoint& point)
{
    QPoint globalPos = this->mapToGlobal(point);
    _contextMenu->exec(globalPos);
}

void ImageView::openContainingFolder()
{
    QFileInfo *file = (QFileInfo*) _image;
    FileIO::openFileManager(file);
}

void ImageView::openWith() {}
void ImageView::setAsDesktopBackground() {}
void ImageView::rotateLeft() {}
void ImageView::rotateRight() {}
void ImageView::copyImage() {}
void ImageView::deleteFile() {}
void ImageView::renameFile() {}
void ImageView::displayProperties() {}

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
        QCursor::setPos(this->mapToGlobal(_initMousePos));
    }
}

void ImageView::resizeEvent(QResizeEvent *e)
{
    this->updateImage();
}

bool ImageView::event(QEvent *e)
{
    if (e->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *mouseEvent = (QMouseEvent*) e;
        if (mouseEvent->buttons() == Qt::LeftButton) {
            this->toggleZoom();
            return true;
        }
    }
    return QScrollArea::event(e);
}

Image* ImageView::image()
{
    return _image;
}
