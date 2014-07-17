#include <exception>
#include <QScrollBar>
#include <QtConcurrent/QtConcurrent>
#include "imageviewer.h"
#include "imaging.h"

// ------------------------------------------------------------------- public //

ImageViewer::ImageViewer(QWidget *parent)
    : QScrollArea(parent)
    , _fitToHeight(true)
    , _fitToWidth(true)
    , _smoothWatcher(new QFutureWatcher<void>())
    , _zoom(1)
{
    QColor backgroundColor(255, 255, 255);

    _label.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _label.setAlignment(Qt::AlignCenter);
    _label.setPalette({backgroundColor});

    this->setWidget(&_label);
    this->setWidgetResizable(true);
    this->setFrameShape(QScrollArea::NoFrame);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setPalette({backgroundColor});
}

ImageViewer::~ImageViewer()
{
    this->cancelSmooth();
}

// ------------------------------------------------------------- public slots //

void ImageViewer::load(const QString &filePath)
{
    this->clear();

    _pixmap.load(filePath);

    this->horizontalScrollBar()->setValue(0);
    this->verticalScrollBar()->setValue(0);
    this->refresh();
}

void ImageViewer::clear()
{
    this->cancelSmooth();

    _label.clear();
    _pixmap = QPixmap();
}

void ImageViewer::fit(ZoomType zoomType)
{
    _fitToWidth = false;
    _fitToHeight = false;

    switch(zoomType) {
        case ZoomToWidth:
            _fitToWidth = true;
            break;
        case ZoomToWidthAndHieght:
            _fitToWidth = true;
            _fitToHeight = true;
            break;
        case ZoomToHeight:
            _fitToHeight = true;
            break;
        default:
            break;
    }

    this->refresh();
}

void ImageViewer::zoomIn()
{
    _zoom *= 1.1;
    this->refresh();
}

void ImageViewer::zoomOut()
{
    _zoom /= 1.1;
    this->refresh();
}

// ---------------------------------------------------------------- protected //

void ImageViewer::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) {

        // Hide the mouse cursor.
        if (this->cursor().shape() != Qt::BlankCursor) {
            this->setCursor(Qt::BlankCursor);
        }

        // Get the delta for the mouse position.
        int dx = e->pos().x() - _initialMousePosition.x();
        int dy = e->pos().y() - _initialMousePosition.y();

        // Scroll the image.
        QScrollBar *vertical = this->verticalScrollBar();
        QScrollBar *horizontal = this->horizontalScrollBar();
        horizontal->setValue(horizontal->value() - dx);
        vertical->setValue(vertical->value() - dy);

        // Keep the cursor in-place by moving it back to its original position.
        QCursor::setPos(this->mapToGlobal(_initialMousePosition));
    }
}

void ImageViewer::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) {
        _initialMousePosition = e->pos();
    }
}

void ImageViewer::mouseReleaseEvent(QMouseEvent *)
{
    if (this->cursor().shape() != Qt::ArrowCursor) {
        this->setCursor(Qt::ArrowCursor);
    }
}

void ImageViewer::resizeEvent(QResizeEvent *)
{
    this->refresh();
}

// ------------------------------------------------------------------ private //

void ImageViewer::cancelSmooth()
{
    if (_smoothWatcher->isRunning()) {
        _smoothWatcher->cancel();
        _smoothWatcher->waitForFinished();
        _smoothWatcher.reset(new QFutureWatcher<void>());
    }
}

void ImageViewer::refresh()
{
    int width;
    if (_fitToWidth) {
        width = std::min((int)(_pixmap.width() * _zoom), this->width());
    } else {
        width = _pixmap.width() * _zoom;
    }

    int height;
    if (_fitToHeight) {
        height = std::min((int)(_pixmap.height() * _zoom), this->height());
    } else {
        height = _pixmap.height() * _zoom;
    }

    QSize newSize = _pixmap.size().scaled(width, height, Qt::KeepAspectRatio);

    // Always redraw the image if there is currently no image displayed.
    if (_label.pixmap() == NULL) {
        this->scale(QSize(width, height));
        return;
    }

    // Scale only if the width or height changed.
    bool widthChanged = _label.pixmap()->width() != newSize.width();
    bool heightChanged = _label.pixmap()->height() != newSize.height();
    if (widthChanged || heightChanged) {
        this->scale(QSize(width, height));
    }

    // Otherwise, reload the pixmap so that the label re-centers itself.
    else {
        _label.setPixmap(*_label.pixmap());
    }
}

void ImageViewer::scale(QSize size)
{
    if (size.width() == 0 || size.height() == 0) {
        return;
    }

    this->cancelSmooth();

    // Do not rescale the image if it is being displayed at 100% size.
    if (size == _pixmap.size()) {
        _label.setPixmap(_pixmap);
    }

    // Otherwise, scale the image quickly and then smooth it in a separate
    // thread.
    else {
        _label.setPixmap(_pixmap.scaled(size, Qt::KeepAspectRatio));
        _smoothWatcher->setFuture(QtConcurrent::run(this, &ImageViewer::smooth));
    }
}

void ImageViewer::smooth()
{
    // Wait 200 milliseconds to see if another smooth request has been fired.
    // This is in case the user zooms multiple times in a row very quickly
    // (i.e. when resizing the window).
    _timer.start();
    while (_timer.elapsed() < 200) {
        if (_smoothWatcher->isCanceled()) {
            return;
        }
    }

    QSize targetSize = _label.pixmap()->size();
    QImage destination = QImage(_pixmap.toImage());

    // Scale the image in 50% intervals to maintain quality.
    while (destination.size().width()/2 > targetSize.width()) {
        destination = Imaging::evenDimensioned(destination);
        destination = Imaging::halfScaled(destination, _smoothWatcher.get());
    }

    // Use smooth scaling when scaling up.
    // This is because bilinear scaling currently only works for downscaling.
    if (targetSize.width() > _pixmap.width()) {
        destination = destination.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // Use bilinear scaling when scaling down.
    else {
        destination = Imaging::bilinearScaled(destination, targetSize, _smoothWatcher.get());
    }

    if (!_smoothWatcher->isCanceled()) {
        _label.setPixmap(QPixmap::fromImage(destination));
    }
}
