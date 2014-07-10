#ifndef IMAGING_H
#define IMAGING_H
#include <QImage>

namespace Imaging
{
    ///
    /// \brief Scales a copy of the given image to half its size and returns
    /// that copy. This function has similar results to using smooth scaling,
    /// but is much faster.
    /// \param image The image to scale.
    /// \return A copy of the image, scaled to half size.
    ///
    QImage halfScale(const QImage &image);
}

#endif // IMAGING_H
