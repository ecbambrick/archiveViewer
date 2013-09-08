#include "imageview.h"
#include <QDebug>

ImageView::ImageView(QWidget *parent) : QScrollArea(parent)
{
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

void ImageView::setImage(Image *image)
{
    if (image != _image) {
        _label->setPixmap(QPixmap(image->path+image->name));
        this->horizontalScrollBar()->setValue(0);
        this->verticalScrollBar()->setValue(0);
        _image = image;
    }
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


