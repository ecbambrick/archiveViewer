#ifndef IMAGING_H
#define IMAGING_H
#include <QImage>
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
}

#endif // IMAGING_H
