#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
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
    
private:
    QPixmap *_pixmap;
    QLabel *_label;
    
};

#endif // IMAGEVIEW_H
