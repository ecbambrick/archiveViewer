#include "imaging.h"

// From https://blog.qt.digia.com/blog/2009/01/20/50-scaling-of-argb32-image/
quint32 pixelAverage(quint32 a, quint32 b)
{
    return ((((a ^ b) & 0xfefefefeUL) >> 1) + (a & b));
}

// From https://blog.qt.digia.com/blog/2009/01/20/50-scaling-of-argb32-image/
QImage Imaging::halfScaled(const QImage &source, QFutureWatcher<void> *watcher)
{
    if (source.width() % 2 != 0 || source.height() % 2 != 0) {
        throw std::invalid_argument("Image must have even dimensions");
    }

    // Get the source and destination images' data as unsigned integers.
    QImage destination(source.size() * 0.5, QImage::Format_ARGB32_Premultiplied);
    const quint32 *src = reinterpret_cast<const quint32*>(source.bits());
    quint32 *dst = reinterpret_cast<quint32*>(destination.bits());

    // Get the number of bytes to skip each iteration of the y-value.
    int dx = destination.bytesPerLine() >> 2;
    int sx = source.bytesPerLine() >> 2;
    int sx2 = sx << 1;

    // Scale.
    for (int y = destination.height(); y; --y, dst += dx, src += sx2) {

        // Cancel the scaling if the watcher has been cancelled.
        if (watcher != nullptr && watcher->isCanceled()) {
            break;
        }

        // Get the pixel data for the current row.
        const quint32 *p1 = src;
        const quint32 *p2 = src + sx;
        quint32 *q = dst;

        // Update the pixel data for the current row.
        for (int x = destination.width(); x; --x, q++, p1 += 2, p2 += 2) {
            * q = pixelAverage(pixelAverage(p1[0], p1[1]), pixelAverage(p2[0], p2[1]));
        }
    }

    return destination;
}
