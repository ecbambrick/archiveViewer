#include "imaging.h"

// From https://blog.qt.digia.com/blog/2009/01/20/50-scaling-of-argb32-image/
quint32 pixelAverage(quint32 a, quint32 b)
{
    return ((((a ^ b) & 0xfefefefeUL) >> 1) + (a & b));
}

QImage Imaging::bilinearScaled(const QImage &source, const QSize &size, QFutureWatcher<void> *watcher)
{
    if (size.width() > source.width() || size.height() > source.height()) {
        throw std::out_of_range("Size must be smaller than or equal to the source image.");
    }

    // Calculate the ratio of the source and destionation's dimensions.
    float xRatio = (float)(source.width() - 1)/size.width();
    float yRatio = (float)(source.height() - 1)/size.height();

    // Get the source and destination images' data as unsigned integers.
    QImage destination = QImage(size, QImage::Format_ARGB32_Premultiplied);
    quint32 *dst = reinterpret_cast<quint32*>(destination.bits());
    const quint32 *src = reinterpret_cast<const quint32*>(source.bits());

    // Keep track of the pixel position of the destination image.
    int offset = 0;

    // Scale the image.
    for (int i = 0; i < destination.height(); i++) {

        // Cancel the scaling if the watcher has been cancelled.
        if (watcher != nullptr && watcher->isCanceled()) {
            break;
        }

        for (int j = 0; j < destination.width(); j++) {

            // Calculate the (x,y) position of the next pixel to
            float x = (int)(xRatio * j);
            float y = (int)(yRatio * i);
            int index = y * source.width() + x;

            // Calculate the differences between the source and destination
            // images at the current pixel.
            float xDifference = (xRatio * j) - x ;
            float yDifference = (yRatio * i) - y ;

            // Get a 2x2 square of pixels at the calculated position.
            int a = src[index];
            int b = src[index+1];
            int c = src[index+source.width()];
            int d = src[index+source.width()+1];

            // Calculate the blue element.
            float blue = (a&0xff)*(1-xDifference)*(1-yDifference)
                       + (b&0xff)*(xDifference)*(1-yDifference)
                       + (c&0xff)*(yDifference)*(1-xDifference)
                       + (d&0xff)*(xDifference*yDifference);

            // Calculate the green element.
            float green = ((a>>8)&0xff)*(1-xDifference)*(1-yDifference)
                        + ((b>>8)&0xff)*(xDifference)*(1-yDifference)
                        + ((c>>8)&0xff)*(yDifference)*(1-xDifference)
                        + ((d>>8)&0xff)*(xDifference*yDifference);

            // Calculate the red element.
            float red = ((a>>16)&0xff)*(1-xDifference)*(1-yDifference)
                      + ((b>>16)&0xff)*(xDifference)*(1-yDifference)
                      + ((c>>16)&0xff)*(yDifference)*(1-xDifference)
                      + ((d>>16)&0xff)*(xDifference*yDifference);

            // Calculate the alpha element.
            float alpha = ((a>>24)&0xff)*(1-xDifference)*(1-yDifference)
                        + ((b>>24)&0xff)*(xDifference)*(1-yDifference)
                        + ((c>>24)&0xff)*(yDifference)*(1-xDifference)
                        + ((d>>24)&0xff)*(xDifference*yDifference);

            // Calculate the pixel value for the destination image
            dst[offset++] = ((((int)alpha)<<24)&0xff000000)
                          | ((((int)red)<<16)&0xff0000)
                          | ((((int)green)<<8)&0xff00)
                          | ((int)blue) ;
        }
    }

    return destination;
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
