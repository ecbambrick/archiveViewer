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

#include <QCoreApplication>
#include <QFileDialog>
#include <QFileInfo>
#include "mainwindow.h"
#include "utility.h"

#include <QDebug>

// ------------------------------------------------------------------- public //

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , _imageSource(nullptr)
    , _playlist(new Playlist())
    , _settings(new QSettings(QSettings::IniFormat,
                          QSettings::UserScope,
                          "ArchiveViewer",
                          "ArchiveViewer"))
    , _wasMaximized(false)
{
    this->loadActions();
    this->loadWidgets();
    this->loadStyleSheet();
    this->loadInitialFile();
    this->loadGeometry();
}

MainWindow::~MainWindow()
{
    _settings->beginGroup("Window");
    if (this->isFullScreen()) {
        _settings->setValue("isMaximized", _wasMaximized);
    } else if (this->isMaximized()) {
        _settings->setValue("isMaximized", true);
    } else {
        _settings->setValue("position", this->pos());
        _settings->setValue("size", this->size());
        _settings->setValue("isMaximized", false);
    }
    _settings->endGroup();

    delete _actionOpen;
    delete _actionPrevious;
    delete _actionNext;
    delete _actionShuffle;
    delete _widgetSpacer;
    delete _widgetSearchBox;
    delete _widgetFileName;
    delete _widgetFilePosition;
    delete _widgetStatusBar;
    delete _widgetToolBar;
    delete _widgetImageViewer;
}

// ------------------------------------------------------------- public slots //

void MainWindow::loadImage()
{
    if (_playlist->isEmpty()) {
        _widgetImageViewer->clear();
        return;
    }

    std::shared_ptr<ImageInfo> image = _playlist->current();

    if (image->exists()) {
        _widgetImageViewer->load(image->absoluteFilePath());
        _settings->setValue("LastSession/viewedFile", image->relativeFilePath());
        emit imageLoaded(image);
    } else {
        _widgetImageViewer->clear();
        emit imageNotLoaded(image);
    }
}

bool MainWindow::open()
{
    QString selectedFile = QFileDialog::getOpenFileName(
                0,
                "Open File",
                _settings->value("LastSession/openedFile").toString(),
                Utility::openDialogFilter());

    if (!selectedFile.isNull()) {
        this->open(selectedFile);
        return true;
    } else {
        return false;
    }
}

bool MainWindow::open(const QString &filePath, bool skipToLastViewed)
{
    QFileInfo fileInfo(filePath);
    QString fileType = fileInfo.suffix();
    QString fileName = fileInfo.fileName();
    bool isArchiveFile = Utility::archiveFileTypes().contains(fileType);
    bool isImageFile = Utility::imageFileTypes().contains(fileType);

    if (isImageFile) {
        _imageSource.reset(new LocalImageSource(filePath));
        if (!skipToLastViewed) {
            skipToLastViewed = true;
            _settings->setValue("LastSession/viewedFile", fileName);
        }
    } else if (isArchiveFile) {
        _imageSource.reset(new QuaZipImageSource(filePath));
    } else {
        return false;
    }

    _playlist.reset(new Playlist(_imageSource));
    _playlist->loops(true);
    _settings->setValue("LastSession/openedFile", filePath);

    if (skipToLastViewed) {
        _playlist->find(_settings->value("LastSession/viewedFile").toString());
    }

    this->connect(_actionNext, SIGNAL(triggered()), _playlist.get(), SLOT(next()));
    this->connect(_actionPrevious, SIGNAL(triggered()), _playlist.get(), SLOT(previous()));
    this->connect(_actionShuffle, SIGNAL(toggled(bool)), _playlist.get(), SLOT(shuffle(bool)));
    this->connect(_playlist.get(), SIGNAL(indexChanged()), this, SLOT(loadImage()));
    this->connect(_playlist.get(), SIGNAL(indexChanged()), this, SLOT(updateFileName()));
    this->connect(_playlist.get(), SIGNAL(indexChanged()), this, SLOT(updateFilePosition()));
    this->connect(_imageSource.get(), SIGNAL(imageReady(QString)), this, SLOT(reloadImage(QString)));
    this->connect(this, SIGNAL(imageNotLoaded(std::shared_ptr<ImageInfo>)), _imageSource.get(), SLOT(imageNeeded(std::shared_ptr<ImageInfo>)));

    emit _playlist->indexChanged();

    return true;
}

void MainWindow::filter()
{
    auto pattern = _widgetSearchBox->text();

    this->setFocus();
    _playlist->filter(Filter(pattern));
}

void MainWindow::reloadImage(const QString &relativeFilePath)
{
    if (_playlist->isEmpty()) return;

    if (_playlist->current()->relativeFilePath() == relativeFilePath) {
        this->loadImage();
    }
}

void MainWindow::updateFileName()
{
    int width = _widgetFileName->maximumWidth() - 90;

    if (_playlist->isEmpty()) {
        _widgetFileName->setText("\nNo File");
        this->setWindowTitle("Archive Viewer");
    } else {
        // cut off the name with "..." if too long
        auto image = _playlist->current();
        auto name = _imageSource->name();
        QFontMetrics font(_widgetFileName->font());
        QString newText = font.elidedText(image->fileName(), Qt::ElideRight, width);
        _widgetFileName->setText(name + "\n" + newText);
        this->setWindowTitle(image->relativeFilePath() + " - Archive Viewer");
    }
}

void MainWindow::updateFilePosition()
{
    if (_playlist->isEmpty()) {
        _widgetFilePosition->setText("0/0");
    } else {
        QString index = QString::number(_playlist->index());
        QString size = QString::number(_playlist->size());
        _widgetFilePosition->setText(index + "/" + size);
    }
}

void MainWindow::zoomFit()
{
    ImageViewer::ZoomType zoom = (ImageViewer::ZoomType)_actionZoomFit->property("fit").toInt();
    ImageViewer::ZoomType newZoom;

    switch (zoom) {
        case ImageViewer::ZoomToFullSize:
            _actionZoomFit->setText("Fit to Width");
            newZoom = ImageViewer::ZoomToWidth;
            break;
        case ImageViewer::ZoomToWidth:
            _actionZoomFit->setText("Fit to Window");
            newZoom = ImageViewer::ZoomToWidthAndHieght;
            break;
        case ImageViewer::ZoomToWidthAndHieght:
            _actionZoomFit->setText("Full Size");
            newZoom = ImageViewer::ZoomToFullSize;
            break;
        default:
            newZoom = zoom;
            break;
    }

    _actionZoomFit->setProperty("fit", newZoom);
    _widgetImageViewer->fit(newZoom);
}

// ---------------------------------------------------------------- protected //

void MainWindow::resizeEvent(QResizeEvent *e)
{
    int width = e->size().width();

    if (width > _widgetFileName->minimumWidth()) {
        _widgetFileName->setMaximumWidth(width);
    }
    this->updateFileName();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    auto key = e->key();
    auto modifiers = e->modifiers();

    // Deselect or clear the search box.
    if (key == Qt::Key_Escape) {
        if (_widgetSearchBox->hasFocus()) {
            this->setFocus();
        } else {
            _widgetSearchBox->clear();
            this->filter();
        }

    // Focus on the search box.
    } else if (key == Qt::Key_F && modifiers == Qt::ControlModifier) {
        _widgetSearchBox->setFocus();
        _widgetSearchBox->selectAll();

    // hide the toolbar and statusbar
    // toggle toolbar using hide() to keep its actions active
    // TODO: make this not terrible
    } else if (e->key() == Qt::Key_F12) {
        statusBar()->setHidden(!statusBar()->isHidden());
        if (_widgetToolBar->height() == 0) {
            _widgetToolBar->setMaximumHeight(48);
        } else {
            _widgetToolBar->setMaximumHeight(0);
        }

    // Toggle fullscreen.
    } else if (e->key() == Qt::Key_F11) {
        if (this->isFullScreen()) {
            if (_wasMaximized) {
                this->showMaximized();
            } else {
                this->showNormal();
            }
        } else {
            _wasMaximized = this->isMaximized();
            this->showFullScreen();
        }
    }
}

// ------------------------------------------------------------------ private //

void MainWindow::loadGeometry()
{
    _settings->beginGroup("Window");
    auto position = _settings->value("position", QPoint(200, 200)).toPoint();
    auto size = _settings->value("size", QSize(640, 480)).toSize();
    auto isMaximized = _settings->value("isMaximized", false).toBool();
    _settings->endGroup();

    this->move(position);
    this->resize(size);
    if (isMaximized) {
        this->showMaximized();
    }
}

bool MainWindow::loadInitialFile()
{
    QStringList args = QCoreApplication::arguments();
    QString lastOpened = _settings->value("LastSession/openedFile").toString();

    if (args.size() == 2 && args.at(1) != "") {
        return this->open(args.at(1));
    } else if (!lastOpened.isEmpty()) {
        return this->open(lastOpened, true);
    } else {
        return false;
    }
}

bool MainWindow::loadStyleSheet()
{
    _widgetFilePosition->setProperty("class", "fileNumber");

    QFile file(":/stylesheets/default");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        this->setStyleSheet(file.readAll());
        file.close();
    }

    return true;
}

void MainWindow::loadActions()
{
    // Next image.
    _actionNext = new QAction("&Next", this);
    _actionNext->setShortcut(Qt::Key_Space);

    // Open file.
    _actionOpen = new QAction("&Open", this);
    _actionOpen->setShortcut(QKeySequence::Open);

    // Previous image.
    _actionPrevious = new QAction("&Previous", this);
    _actionPrevious->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Space));

    // Shuffle images.
    _actionShuffle = new QAction("&Shuffle", this);
    _actionShuffle->setShortcut(Qt::Key_R);
    _actionShuffle->setCheckable(true);

    // Zoom to full, width, or width/height.
    _actionZoomFit = new QAction("&Fit to Window", this);
    _actionZoomFit->setShortcut(Qt::Key_Z);
    _actionZoomFit->setProperty("fit", ImageViewer::ZoomToWidthAndHieght);

    // Zoom in.
    _actionZoomIn = new QAction("Zoom &In", this);
    _actionZoomIn->setShortcut(Qt::Key_Equal);

    // Zoom out.
    _actionZoomOut = new QAction("Zoom O&ut", this);
    _actionZoomOut->setShortcut(Qt::Key_Minus);

    // Zoom to 100%.
    _actionZoomFull = new QAction("Zoom %to 100%", this);
    _actionZoomFull->setShortcut(Qt::Key_0);
}

void MainWindow::loadWidgets()
{
    // File name label.
    _widgetFileName = new QLabel("\nNo File", this);
    _widgetFileName->setMinimumWidth(70);

    // File position label.
    _widgetFilePosition = new QLabel("0/0", this);
    _widgetFilePosition->setAlignment(Qt::AlignRight);

    // Image viewer.
    _widgetImageViewer = new ImageViewer(this);
    this->connect(_actionZoomIn, &QAction::triggered, _widgetImageViewer, &ImageViewer::zoomIn);
    this->connect(_actionZoomOut, &QAction::triggered, _widgetImageViewer, &ImageViewer::zoomOut);
    this->connect(_actionZoomFull, &QAction::triggered, _widgetImageViewer, &ImageViewer::zoomToOriginalSize);

    // Search box.
    _widgetSearchBox = new QLineEdit(this);
    _widgetSearchBox->setMaximumWidth(192);
    _widgetSearchBox->setMinimumWidth(192);
    this->connect(_widgetSearchBox, &QLineEdit::returnPressed, this, &MainWindow::filter);

    // Spacer.
    _widgetSpacer = new QWidget(this);
    _widgetSpacer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    // Status bar.
    _widgetStatusBar = new QStatusBar(this);
    _widgetStatusBar->insertWidget(0, _widgetFileName);
    _widgetStatusBar->insertPermanentWidget(1, _widgetFilePosition);
    _widgetStatusBar->setContentsMargins(6,4,8,4);
    _widgetStatusBar->setSizeGripEnabled(false);

    // Tool bar.
    _widgetToolBar = new QToolBar("main", this);
    _widgetToolBar->addAction(_actionOpen);
    _widgetToolBar->addSeparator();
    _widgetToolBar->addAction(_actionPrevious);
    _widgetToolBar->addAction(_actionShuffle);
    _widgetToolBar->addAction(_actionNext);
    _widgetToolBar->addSeparator();
    _widgetToolBar->addAction(_actionZoomIn);
    _widgetToolBar->addAction(_actionZoomOut);
    _widgetToolBar->addAction(_actionZoomFit);
    _widgetToolBar->addWidget(_widgetSpacer);
    _widgetToolBar->addWidget(_widgetSearchBox);
    _widgetToolBar->setMovable(false);

    // Main window.
    this->addToolBar(_widgetToolBar);
    this->setStatusBar(_widgetStatusBar);
    this->setCentralWidget(_widgetImageViewer);
    this->connect(_actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    this->connect(_actionZoomFit, SIGNAL(triggered()), this, SLOT(zoomFit()));
    this->addAction(_actionZoomFull);
}
