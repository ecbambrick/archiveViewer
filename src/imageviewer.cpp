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
    , _movie(nullptr)
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

    // Determine if the file is animated.
    bool isMovie = false;
    if (filePath.endsWith("gif", Qt::CaseInsensitive)) {
        _movie = std::unique_ptr<QMovie>(new QMovie(filePath));
        if (_movie->isValid() && _movie->frameCount() > 1) {
            isMovie = true;
        }
    }

    // Get the size of the first frame of the movie and then restart it.
    if (isMovie) {
        _movie->jumpToFrame(0);
        _movieSize = _movie->currentImage().size();
        _movie->stop();
    }

    // Unload the movie and load the image;
    else {
        _movie.reset(nullptr);
        _pixmap.load(filePath);
    }

    this->horizontalScrollBar()->setValue(0);
    this->verticalScrollBar()->setValue(0);
    this->refresh();
}

void ImageViewer::clear()
{
    this->cancelSmooth();

    _label.clear();
    _movie.reset(nullptr);
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

void ImageViewer::zoomToOriginalSize()
{
    _zoom = 1.0;
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

        // Pan the image.
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
    QSize originalSize;
    if (_movie) {
        originalSize = _movieSize;
    } else {
        originalSize = _pixmap.size();
    }

    int width;
    if (_fitToWidth) {
        width = std::min((int)(originalSize.width() * _zoom), this->width());
    } else {
        width = originalSize.width() * _zoom;
    }

    int height;
    if (_fitToHeight) {
        height = std::min((int)(originalSize.height() * _zoom), this->height());
    } else {
        height = originalSize.height() * _zoom;
    }

    // Get the target image size for scaling.
    QSize newSize = originalSize.scaled(width, height, Qt::KeepAspectRatio);

    if (_movie) {
        refreshMovie(newSize);
    } else {
        refreshImage(newSize);
    }
}

void ImageViewer::refreshImage(const QSize &size)
{
    // Always redraw the image if there is currently no image displayed.
    if (_label.pixmap() == NULL) {
        this->scale(size);
        return;
    }

    // Scale only if the width or height changed.
    bool widthChanged = _label.pixmap()->width() != size.width();
    bool heightChanged = _label.pixmap()->height() != size.height();
    if (widthChanged || heightChanged) {
        this->scale(size);
    }

    // Otherwise, reload the pixmap so that the label re-centers itself.
    else {
        _label.setPixmap(*_label.pixmap());
    }
}

void ImageViewer::refreshMovie(const QSize &size)
{
    bool widthChanged = _movie->currentImage().width() != size.width();
    bool heightChanged = _movie->currentImage().height() != size.height();

    if (widthChanged || heightChanged) {
        _movie->setScaledSize(size);
    }

    _label.setMovie(_movie.get());
    _label.resize(size);

    if (_movie->state() == QMovie::NotRunning) {
        _movie->start();
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

    // If the image has not been loaded yet, smooth the image immediately.
    else if (_label.pixmap() == NULL) {
        _label.resize(size);
        this->smooth(size);
    }

    // Otherwise, scale the image quickly and then smooth it in a separate
    // thread.
    else {
        _label.resize(size);
        _label.setPixmap(_pixmap.scaled(size));
        _smoothWatcher->setFuture(QtConcurrent::run(this, &ImageViewer::smooth, size, _smoothWatcher.get()));
    }
}

void ImageViewer::smooth(const QSize &size, QFutureWatcher<void> *watcher)
{
    // Wait 200 milliseconds to see if another smooth request has been fired.
    // This is in case the user zooms multiple times in a row very quickly
    // (i.e. when resizing the window).
    if (watcher != nullptr) {
        for (int i = 0; i < 4; i++) {
            if (watcher->isCanceled()) {
                return;
            }
            QThread::msleep(50);
        }
    }

    QSize targetSize = size;
    QImage image = QImage(_pixmap.toImage());

    // Scale the image in 50% intervals to maintain quality.
    while (image.width()/2 > targetSize.width()) {
        image = Imaging::evenDimensioned(image);
        image = Imaging::halfScaled(image, watcher);
    }

    // Use smooth scaling when scaling up.
    // This is because bilinear scaling currently only works for downscaling.
    if (targetSize.width() > _pixmap.width()) {
        image = image.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // Use bilinear scaling when scaling down.
    else {
        image = Imaging::bilinearScaled(image, targetSize, watcher);
    }

    if ((watcher != nullptr && !watcher->isCanceled()) || watcher == nullptr) {
        _label.setPixmap(QPixmap::fromImage(image));
    }
}
