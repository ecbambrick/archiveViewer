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

#include "qdebug.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "directoryimagelist.h"
#include "archivedimagelist.h"
#include "sevenziparchiver.h"
#include <QSettings>

/* ----------------------------------------------------------- INITIALIZATION */

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    _imageList = new ImageListFilter();
    _archiver = new SevenZipArchiver();
    _settings = new QSettings(
                    QCoreApplication::applicationDirPath() + "/settings.ini",
                    QSettings::IniFormat);

    initUI();
    initFile();
}

void MainWindow::initFile()
{
    // load settings and command line arguments
    QString lastOpened = _settings->value("last_opened_file", "").toString();
    QString lastViewed = _settings->value("last_viewed_file", "").toString();
    QStringList args   = QCoreApplication::arguments();

    qDebug() << lastViewed;

    // load file from command line arguments
    if (args.size() == 2 && args.at(1) != "") {
        load(args.at(1));

    // load file from previous session
    } else if (lastOpened != "") {
        load(lastOpened);
        setImage(_imageList->goTo(lastViewed));
    }
}

void MainWindow::initUI()
{
    // declare actions
    _actionOpen       = new QAction("&Open", this);
    _actionPrev       = new QAction("&Previous", this);
    _actionNext       = new QAction("&Next", this);
    _actionShuf       = new QAction("&Shuffle", this);
    _actionToggleZoom = new QAction("&View Actual Size", this);

    // declare widgets
    QWidget *uiSpace  = new QWidget(this);
    _uiStatus         = new QStatusBar(this);
    _uiToolbar        = new QToolBar("main", this);
    _uiSearch         = new QLineEdit(this);
    _uiFileName       = new QLabel("No File", this);
    _uiFileNumber     = new QLabel("0/0", this);
    _uiView           = new ImageView(this);

    // initialize layout
    this->addToolBar(_uiToolbar);
    this->setStatusBar(_uiStatus);
    this->setCentralWidget(_uiView);

    // initialize status bar
    _uiStatus->insertWidget(0, _uiFileName);
    _uiStatus->insertPermanentWidget(1, _uiFileNumber);
    _uiStatus->setContentsMargins(6,4,8,4);
    _uiStatus->setSizeGripEnabled(false);

    // initialie toolbar
    _uiToolbar->addAction(_actionOpen);
    _uiToolbar->addSeparator();
    _uiToolbar->addAction(_actionPrev);
    _uiToolbar->addAction(_actionShuf);
    _uiToolbar->addAction(_actionNext);
    _uiToolbar->addSeparator();
    _uiToolbar->addAction(_actionToggleZoom);
    _uiToolbar->addWidget(uiSpace);
    _uiToolbar->addWidget(_uiSearch);
    _uiToolbar->setMovable(false);

    // initialize shortcuts
    _actionToggleZoom->setShortcut(Qt::Key_F);
    _actionShuf->setShortcut(Qt::Key_R);
    _actionNext->setShortcut(Qt::Key_Space);
    _actionOpen->setShortcut(QKeySequence::Open);
    _actionPrev->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Space));

    // initialize connections
    this->connect(_actionOpen,       SIGNAL(triggered()),     this, SLOT(open()));
    this->connect(_actionPrev,       SIGNAL(triggered()),     this, SLOT(previous()));
    this->connect(_actionNext,       SIGNAL(triggered()),     this, SLOT(next()));
    this->connect(_actionShuf,       SIGNAL(toggled(bool)),   this, SLOT(shuffle(bool)));
    this->connect(_actionToggleZoom, SIGNAL(triggered()),     this, SLOT(toggleZoom()));
    this->connect(_uiSearch,         SIGNAL(returnPressed()), this, SLOT(filter()));

    // misc widget settings
    uiSpace->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    _uiFileNumber->setAlignment(Qt::AlignRight);
    _uiFileName->setMinimumWidth(70);
    _uiSearch->setMaximumWidth(192);
    _uiSearch->setMinimumWidth(192);
    _actionShuf->setCheckable(true);

    // initialize style sheet
    QFile file(":/stylesheets/default");
    _uiFileNumber->setProperty("class", "fileNumber");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        this->setStyleSheet(file.readAll());
        file.close();
    }

    // restore window geometry
    QByteArray geometry = _settings->value("window_geometry", saveGeometry()).toByteArray();
    bool maximized = _settings->value("window_maximized", false).toBool();
    this->restoreGeometry(geometry);
    if (maximized) {
        this->showMaximized();
    }
}

MainWindow::~MainWindow()
{
    _settings->setValue("window_geometry", saveGeometry());
    _settings->setValue("window_maximized", isMaximized());
    delete _settings;
    delete _imageList->list();
    delete _imageList;
}

/* ------------------------------------------------------------- FILE ACTIONS */

/// Display the open file dialog and open the selected file
void MainWindow::open()
{
    QString openDirectory = _settings->value("last_opened_file", "").toString();
    QString selectedFile = FileIO::openFileDialogue(openDirectory);

    if (selectedFile != NULL) {
        this->load(selectedFile);
    }
}

/// Populate the image list and set the image
void MainWindow::load(const QString path)
{
    QFileInfo file(path);
    QString fileName = file.fileName();
    FileIO::FileType fileType = FileIO::getFileType(&file);

    // error checking
    if (fileType == FileIO::INVALID) {
        qDebug() << "invalid file: " << file.absoluteFilePath();
        return;
    }

    // remove old list
    ImageList *oldList = _imageList->list();
    if (oldList != NULL) { delete oldList; }

    // create new image list
    ImageList *newList;
    if (fileType == FileIO::ARCHIVE) {
        newList = new ArchivedImageList(_archiver, file.absoluteFilePath());
    } else {
        newList = new DirectoryImageList(file.absoluteFilePath());
    }
    _imageList->setList(newList);
    newList->open();

    // update the image
    _settings->setValue("last_opened_file", file.absoluteFilePath());
    _settings->setValue("last_viewed_file", file.fileName());
    this->setImage(_imageList->goTo(fileName));
}

/* ------------------------------------------------------- NAVIGATION ACTIONS */

/// Go to the previous image in the image list if it exists
void MainWindow::previous()
{
    Image *image = _imageList->previous();
    this->setImage(image);
}

/// Go to the next image in the image list if it exists
void MainWindow::next()
{
    Image *image = _imageList->next();
    this->setImage(image);
}

/* ----------------------------------------------------------- FILTER ACTIONS */

/// Filter the image list based on the searchbox's contents
void MainWindow::filter()
{
    Image *image = _imageList->filter(_uiSearch->text());
    setImage(image);
}

/// Toggle shuffling of image list
void MainWindow::shuffle(bool value)
{
    _imageList->setShuffle(value);
}

/* ------------------------------------------------------------- ZOOM ACTIONS */

/// Toggle fit-to-window or 100% zoom
void MainWindow::toggleZoom()
{
    bool fitToWindow = _actionToggleZoom->property("fit").toBool();
    if (fitToWindow) {
        _actionToggleZoom->setText("&View Actual Size");
        _actionToggleZoom->setProperty("fit", false);
    } else {
        _actionToggleZoom->setText("&Fit to Window");
        _actionToggleZoom->setProperty("fit", true);
    }
    _uiView->toggleZoom();
}

/* --------------------------------------------------------------- UI ACTIONS */

/// Set the pixmap to display, if NULL image is provided, clear the display
void MainWindow::setImage(Image *image)
{
    if (image == NULL) {
        _uiView->clearImage();
    } else {
        _uiView->setImage(image);
        _settings->setValue("last_viewed_file", image->name);
    }
    this->updateStatusNumber(image);
    this->updateStatusName(image);
}

/// Update the image's filename in the status bar
void MainWindow::updateStatusName(Image *image)
{
    int width = _uiFileName->maximumWidth() - 90;
    if (_imageList->empty() || image == NULL) {
        _uiFileName->setText("No File");
        this->setWindowTitle("Archive Viewer");
    } else {
        // cut off the name with "..." if too long
        QFontMetrics font(_uiFileName->font());
        QString newText = font.elidedText(image->name, Qt::ElideRight, width);
        _uiFileName->setText(_imageList->listName() + "\n" + newText);
        this->setWindowTitle(image->name + " - Archive Viewer");
    }
}

/// Update the image's index in the status bar
void MainWindow::updateStatusNumber(Image *image)
{
    QString index = QString::number(_imageList->index()+1);
    QString size = QString::number(_imageList->size());
    QString absoluteSize = QString::number(_imageList->list()->size());

    if (image == NULL) {
        _uiFileNumber->setText("0/0");
    } else {
        _uiFileNumber->setText(
                    index + " / " + size +
                    "\n(" + absoluteSize + ")"
        );
    }
}

void MainWindow::toggleClean()
{
    if (statusBar()->isHidden()) {
        statusBar()->show();
    } else {
        statusBar()->hide();
    }
}

/* ------------------------------------------------------------------- EVENTS */

/// resize statusbar filename
void MainWindow::resizeEvent(QResizeEvent *e)
{
    int width = this->width();
    if (width > _uiFileName->minimumWidth()) {
        _uiFileName->setMaximumWidth(width);
    }
    this->updateStatusName(_imageList->current());
}

/// misc. actions
void MainWindow::keyPressEvent(QKeyEvent *e)
{
    // deselect search bar
    if (e->key() == Qt::Key_Return) {
        if (_uiSearch->hasFocus() && !_imageList->empty()) {
            _uiView->setFocus();
        }

    // deselect / clear search bar
    } else if (e->key() == Qt::Key_Escape) {
        if (_uiSearch->hasFocus()) {
            _uiView->setFocus();
        } else {
            _uiSearch->clear();
            this->filter();
        }

    // focus on search bar
    } else if (e->key() == Qt::Key_F &&
               e->modifiers() == Qt::ControlModifier) {
        _uiSearch->setFocus();
        _uiSearch->selectAll();

    // hide the toolbar and statusbar
    } else if (e->key() == Qt::Key_F11) {
        this->toggleClean();
    }
}
