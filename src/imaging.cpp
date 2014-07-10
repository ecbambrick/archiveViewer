#include "imaging.h"

// From https://blog.qt.digia.com/blog/2009/01/20/50-scaling-of-argb32-image/
quint32 pixelAverage(quint32 a, quint32 b)
{
    return ((((a ^ b) & 0xfefefefeUL) >> 1) + (a & b));
}

// From https://blog.qt.digia.com/blog/2009/01/20/50-scaling-of-argb32-image/
QImage Imaging::halfScale(const QImage &source)
{
    if (source.width() % 2 != 0 || source.height() % 2 != 0) {
        throw std::invalid_argument("Image must have even dimensions");
    }

    QImage dest(source.size() * 0.5, QImage::Format_ARGB32_Premultiplied);

    const quint32 *src = reinterpret_cast<const quint32*>(source.bits());
    int sx = source.bytesPerLine() >> 2;
    int sx2 = sx << 1;

    quint32 *dst = reinterpret_cast<quint32*>(dest.bits());
    int dx = dest.bytesPerLine() >> 2;
    int ww = dest.width();
    int hh = dest.height();

    for (int y = hh; y; --y, dst += dx, src += sx2) {
        const quint32 *p1 = src;
        const quint32 *p2 = src + sx;
        quint32 *q = dst;
        for (int x = ww; x; --x, q++, p1 += 2, p2 += 2)
            * q = pixelAverage(pixelAverage(p1[0], p1[1]),
                               pixelAverage(p2[0], p2[1]));
    }

    return dest;
}
