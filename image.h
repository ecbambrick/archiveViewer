#ifndef IMAGE_H
#define IMAGE_H

#include <QFileInfo>

class Image : public QFileInfo
{
public:
    Image(const QString path);
    int width();
    int height();
    QString relativeName();
    void relativePath(QString path);
    QString relativePath();

private:
    int _width;
    int _height;
    QString _relativePath;

    /* metadata
        QString title;
        QString subject;
        QString comments;
        QString tags;
        QString copyright;
        int rating;
    */
};

#endif // IMAGE_H
