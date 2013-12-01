#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPixmap>
#include <QMovie>
#include <QMenu>
#include <QLabel>
#include "imagelist.h"

class ImageView : public QScrollArea
{
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = 0);
    ~ImageView();
    void setImage(Image *image);
    void clearImage();
    void updateImage();
    void toggleZoom();
    Image* image();

public slots:
    void fitToWidth(bool val);
    void showContextMenu(const QPoint &point);

private slots:
    void openContainingFolder();
    void openWith();
    void setAsDesktopBackground();
    void rotateLeft();
    void rotateRight();
    void copyImage();
    void deleteFile();
    void renameFile();
    void displayProperties();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void resizeEvent(QResizeEvent *e);
    bool event(QEvent *e);

private:
    void initContextMenu();

    QMenu *_contextMenu;
    Image *_image;
    QPixmap *_pixmap;
    QMovie *_movie;
    QLabel *_label;
    QPoint _initMousePos;
    bool _fitToWindow;
    bool _fitToWidth;
};

#endif // IMAGEVIEW_H
