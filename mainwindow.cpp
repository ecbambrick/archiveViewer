/*******************************************************************************

    Copyright (C) 2013 by Cole Bambrick
    cole.bambrick@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.

*******************************************************************************/

#include <QDebug>
#include <QDir>
#include <QLabel>
#include <cmath>
#include <QSettings>
#include <QLineEdit>
#include <QFileDialog>
#include <QTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

/* Construction --------------------------------------------------------------*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _ui(new Ui::MainWindow)
{
    // Create temporary directory if one doesn't exist
    QDir tempDir(QDir::tempPath()+"/archiveViewer");
    if (!tempDir.exists()) tempDir.mkpath(".");

    _archiver = new Archiver(this);
    _imageList = NULL;
    _pixmap = NULL;
    _animation = NULL;
    _settingsPath = "settings.ini";

    // some of loadSettings should go before initUI
    initUI();
    loadSettings(_settingsPath);
}

/**
    Initialize UI elements
*/
void MainWindow::initUI()
{
    _ui->setupUi(this);

    // Initialize label sizing policy
    _ui->label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _ui->label->setAlignment(Qt::AlignCenter);
    _ui->label->setMinimumSize(240, 160);
    _ui->label->setFocusPolicy(Qt::WheelFocus);

    // a dummy widget taking as much horizontal space as possible
    QWidget *space = new QWidget(this);
    space->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    //asd
    _searchBox = new QLineEdit(this);
    connect(_searchBox, SIGNAL(returnPressed()), this, SLOT(filter()));

    // Initialize toolbar
    _ui->toolBar->addWidget(space);
    _ui->toolBar->addWidget(_searchBox);
}

/* Settings Management -------------------------------------------------------*/

/**
    Load application settings from ini file
*/
void MainWindow::loadSettings(const QString &path)
{
    // Read settings
    QSettings settings(path, QSettings::IniFormat);
    QByteArray geometry = settings.value("window_geometry", saveGeometry()).toByteArray();
    QPoint point = settings.value("window_pos", this->pos()).toPoint();
    bool isMax = settings.value("window_maximized", isMaximized()).toBool();
    int index = settings.value("last_index", 0).toInt();
    _defaultDir = settings.value("default_directory", "").toString();
    _lastOpenedFile = settings.value("last_file", "").toString();

    // Apply settings
    restoreGeometry(geometry);
    move(point);
    if (isMax) showMaximized();
    if (_lastOpenedFile != "") {
        loadFile(_lastOpenedFile);
        goTo(index);
    }
}

/**
    Save application settings to ini file
*/
void MainWindow::saveSettings(const QString &path)
{
    QSettings settings(path, QSettings::IniFormat);
    settings.setValue("window_geometry", saveGeometry());
    settings.setValue("window_maximized", isMaximized());
    settings.setValue("window_pos", pos());
    settings.setValue("window_size", size());
    settings.setValue("default_directory", _defaultDir);
    settings.setValue("last_file", _lastOpenedFile);
    settings.setValue("last_index", _index);
}

/* Image List Management ---------------------------------------------------- */

/**
    Load new image
*/
void MainWindow::updateLabel()
{
    if (_imageList == NULL) return;

    Image *image = _imageList->at(_index);
    QString title = _imageList->listName() + "/";
    QString subtitle = image->name;
    QString path = image->path + image->name;
    int size = _imageList->size();

    setWindowTitle(QString(title + subtitle + " (%1/%2)").arg(_index+1).arg(size));
    if (_pixmap != NULL) {
        delete _pixmap;
        _pixmap = NULL;
    } if (_animation != NULL) {
        delete _animation;
        _animation = NULL;
    }

    // Determine if the image is animated or not
    QMovie m(path);
    if(m.isValid()){
        if(m.frameCount() > 1){
            _animation = new QMovie(path);
            _ui->label->setMovie(_animation);
            _ui->label->movie()->start();
        }
        else{
            _pixmap = new QPixmap(path);
            _ui->label->setPixmap(_pixmap->scaled(_ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

/**
    Open file dialog and open selected file
*/
void MainWindow::open()
{
    // Open File Dialog
    QString selectedFile = QFileDialog::getOpenFileName(
                this,
                "Open File",
                _defaultDir,
                "Junk (*.jpg *.jpeg *.png *.gif *.cbz)");

    // No image selected
    if (selectedFile != NULL) loadFile(selectedFile);
}

/**
    Generate image list and load selected file
*/
void MainWindow::loadFile(const QString &path)
{

    QFileInfo file(path);
    if (!file.exists()) return;

    _defaultDir = file.absolutePath();
    delete _imageList;

    // Archive selected
    if (path.contains(QRegExp("^.*\\.(png|gif|jpeg|jpg)$"))) {
        qDebug() << "opened image";
        _imageList = new DirectoryImageList(path);

    // Image selected
    } else if (path.contains(QRegExp("^.*\\.(cbz)$"))) {
        qDebug("opened archive");
        _imageList = new ArchivedImageList(_archiver, path);
    }

    _index = _imageList->open();
    _lastOpenedFile = path;
    updateLabel();
}

/**
    Filter image list by filename
*/
void MainWindow::filter()
{
    if (_imageList->size() == 0) return;

    Image *image;
    QStringList tokens;
    QString text = _searchBox->text();

    // Filter on quoted blocks
    for (int i = 0; i<text.length(); i++) {
        if (text.at(i) == '"') {
            for (int j = i+1; j < text.length(); j++) {
                if (text.at(j) == '"') {
                    tokens.append(text.mid(i+1,j-i-1));
                    text = text.remove(i,j-i+1);
                    break;
                }
            }
        }
    }

    // filter on single words
    foreach(QString token, text.split(QRegExp("\\s"))) {
        if (token != "") {
            tokens.append(token);
        }
    }

    // activate/deactivate images based on filters
    // TODO: better escape logic
    for (int i=0; i<_imageList->size(); i++) {
        image = _imageList->at(i);
        foreach(QString token, tokens) {
            if (!image->name.contains(token, Qt::CaseInsensitive)) {
                image->active = false;
                goto found;
            }
        }
        image->active = true;
        found: i=i;
    }

    // go to the next image if the current image has been deactivated
    if (!_imageList->at(_index)->active) {
        next();
        updateLabel();
    }
    _ui->label->setFocus();
}

/**
    Go to an index in the image list
*/
void MainWindow::goTo(int index)
{
    _index = index;
    if (_imageList->size() > _index) {
        if (!_imageList->at(_index)->active) {
            next();
        } else {
            qDebug() << _index;
            updateLabel();
        }
    }
}

/**
    Go to the next active image in the image list
*/
void MainWindow::next()
{
    if (_imageList == NULL) return;
    for(int i=0; i<_imageList->size(); i++) {
        _index = (_index + 1) % _imageList->size();
        if (_imageList->at(_index)->active) break;
    }
    updateLabel();
}

/**
    Go to the previous active image in the image list
*/
void MainWindow::previous()
{
    if (_imageList == NULL) return;
    for(int i=0; i<_imageList->size(); i++) {
        int size = _imageList->size() - 1;
        _index = _index - 1;
        _index = (_index < 0) ? size : (_index % size);
        if (_imageList->at(_index)->active) break;
    }
    updateLabel();
}

/**
    Go to a random active image in the image list
*/
void MainWindow::random()
{
    if (_imageList == NULL) return;
    QList<int> activeList;
    for(int i=0; i<_imageList->size(); i++) {
        if (_imageList->at(i)->active && i != _index) {
            activeList << i;
        }
    }
    if (activeList.size() <= 1) return;
    srand((uint)QDateTime::currentMSecsSinceEpoch());
    _index = activeList.at(rand()%activeList.size());

    updateLabel();
}

void MainWindow::rename() {}
void MainWindow::zoomIn() {}
void MainWindow::zoomOut() {}
void MainWindow::zoomFit() {}
void MainWindow::zoomFull() {}

/* Events ------------------------------------------------------------------- */

/**
    Resize the pixmap
*/
void MainWindow::resizeEvent(QResizeEvent *)
{
    if (_pixmap == NULL) return;
    _ui->label->setPixmap(_pixmap->scaled(_ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

/**
    Focus/Defocus the search bar
*/
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        if (_searchBox->hasFocus()) _ui->label->setFocus();
        else { _searchBox->clear(); filter(); }

    } else if (e->key() == Qt::Key_F && e->modifiers() == Qt::ControlModifier) {
        _searchBox->setFocus();
        _searchBox->selectAll();
    }
}

MainWindow::~MainWindow()
{
    saveSettings(_settingsPath);
    delete _imageList;
    delete _archiver;
    delete _pixmap;
    delete _ui;
}
