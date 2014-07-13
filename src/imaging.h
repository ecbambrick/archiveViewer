#ifndef IMAGING_H
#define IMAGING_H
#include <QImage>
#include <QSize>
#include <QFutureWatcher>

namespace Imaging
{
    ///
    /// \brief Scales a copy of the given image to half its size and returns
    /// that copy. This function has similar results to using smooth scaling,
    /// but is much faster. If the function is called concurrently, an optional
    /// watcher parameter can be passed to determine wither or not the operation
    /// should be cancelled.
    /// \param source The image to scale.
    /// \param watcher The future watcher that calls the function.
    /// \return A copy of the image, scaled to half size.
    ///
    QImage halfScaled(const QImage &source, QFutureWatcher<void> *watcher = nullptr);

    /// \brief Scales a copy of the given image using bilinear resampling and
    /// returns that copy. This function should generally only be used when
    /// scaling less than 50%. Anything beyond this bound will have similar
    /// quality to nearest neighbor.
    /// \param source The image to scale.
    /// \param size The size to scale the image to.
    /// \param watcher The future watcher that calls the function.
    /// \return A copy of the image, scaled to the given size.
    ///
    QImage bilinearScaled(const QImage &source, const QSize &size, QFutureWatcher<void> *watcher = nullptr);
}

#endif // IMAGING_H
