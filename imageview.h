#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QMouseEvent>
#include <QPixmap>
#include <QLabel>
#include "imagelist.h"

class ImageView : public QScrollArea
{
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = 0);
    ~ImageView();
    void setImage(Image *image);

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    
private:
    QPixmap *_pixmap;
    QLabel *_label;
    QPoint _initMousePos;
    
};

#endif // IMAGEVIEW_H
