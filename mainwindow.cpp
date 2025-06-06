#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "placeholderfile.h"
#include "preferences.h"
#include "ui_preferences.h"
#include "cextglobals.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QErrorMessage>
#include <QDataStream>
#include <QTextStream>
#include <QIODevice>
#include <QFont>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TITLE = "CextEdit";
    ACTIVE_TITLE = TITLE;
    setWindowTitle(TITLE);
    ui->plainTextEdit->setVisible(false);

    pref = new Preferences();
    pref->ui->fontCombo->setCurrentFont(ui->plainTextEdit->font());

    DEFAULT_FONT_SIZE = ui->plainTextEdit->fontInfo().pointSize();

    aboutWin = new AboutDialog();

    connect(ui->plainTextEdit,&QPlainTextEdit::textChanged,this,&MainWindow::textEdited);
    connect(ui->actionAbout,&QAction::triggered, this, [this]() {
        this->aboutWin->show();
    });
    connect(ui->actionNew,&QAction::triggered,this,&MainWindow::NewFile);
    connect(ui->actionDelete,&QAction::triggered,this,&MainWindow::DeleteFile);
    connect(ui->actionOpen,&QAction::triggered,this,[this]() {
        OpenFile();
    });
    connect(ui->actionResetZoom,&QAction::triggered,this,[this]() {
        setFontSize(DEFAULT_FONT_SIZE);
    });
    connect(ui->actionZoomIn,&QAction::triggered,this,[this]() {
        setFontSize(1,1);
    });
    connect(ui->actionZoomOut,&QAction::triggered,this,[this]() {
        setFontSize(-1,1);
    });
    connect(ui->actionSave,&QAction::triggered,this,[this]() {
        Save();
    });
    connect(ui->actionSaveAs,&QAction::triggered,this,[this]() {
        SaveAs();
    });

    connect(ui->actionClose,&QAction::triggered,this,[this]() {
        if (!safelyClose()) return;
        reset();
        ACTIVE_TITLE = TITLE;
        setWindowTitle(TITLE);
        ui->plainTextEdit->setVisible(false);
    });
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);
    connect(ui->actionUndo,&QAction::triggered,ui->plainTextEdit,&QPlainTextEdit::undo);
    connect(ui->actionRedo,&QAction::triggered,ui->plainTextEdit,&QPlainTextEdit::redo);

    connect(ui->actionPreferences,&QAction::triggered,this,[this]() {
        pref->show();
    });

    connect(pref,&Preferences::FontUpdated,this,[this]() {
        QFont newFont = pref->ui->fontCombo->currentFont();
        newFont.setPointSize(ui->plainTextEdit->fontInfo().pointSize());
        ui->plainTextEdit->setFont(newFont);
    });

    connect(pref,&Preferences::WStyleUpdated,this,[this]() {
        QString style = pref->ui->widgetsCombo->currentText();
        if (style == "System")
            CextGlobals::APP->setStyle(CextGlobals::SystemWStyle);
        else
            CextGlobals::APP->setStyle(style);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    delete aboutWin;
    delete activeFile;
}

bool MainWindow::safelyClose() {
    if (activeFile == nullptr || initData == ui->plainTextEdit->toPlainText()) return true;
    QMessageBox::StandardButton opt = QMessageBox::warning(this,"Close File — CextEdit",QString("File \"%1\" has unsaved changes. What would you like to do?").arg(activeFile->fileName()),QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Cancel);
    if (opt == QMessageBox::Cancel) return false;
    if (opt == QMessageBox::Save)
        return Save();
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (safelyClose())
        return event->accept();
    event->ignore();
}

bool MainWindow::SaveFile(QString* p) {
    if (activeFile == nullptr) return false;
    auto err = [this](QString exception) {
        QMessageBox::critical(this,"Error — " + TITLE,exception,QMessageBox::StandardButton::Ok,QMessageBox::StandardButton::Ok);
    };
    try {
        QString path = p == nullptr? activeFile->filePath() : *p;
        QFile f(path);
        if (f.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text)) {
            QTextStream out(&f);
            out << ui->plainTextEdit->toPlainText();
            f.close();
            initData = ui->plainTextEdit->toPlainText();
            textEdited();
        } else {
            err("Could not open file for save: " + f.errorString());
            return false;
        }
    } catch(...) {
        return false;
    }
    return true;
}

bool MainWindow::SaveAs(QString* p) {
    if (activeFile == nullptr) return false;
    QString name = QFileDialog::getSaveFileName(this,"Save File As...",fileExists? activeFile->dir().path() : QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation));
    if (name == "") return false;
    auto err = [this](QString exception) {
        QMessageBox::critical(this,"Error — " + TITLE,exception,QMessageBox::StandardButton::Ok,QMessageBox::StandardButton::Ok);
    };
    if (QFileInfo(name).isDir()) {
        err(name + " is a directory.");
        return false;
    }
    try {
        QFile f(name);
        if (f.open(QIODevice::WriteOnly|QIODevice::Truncate|QIODevice::Text)) {
            QTextStream out(&f);
            out << ui->plainTextEdit->toPlainText();
            f.close();
        } else {
            err("Could not open file for save: " + f.errorString());
            return false;
        }
        initData = ui->plainTextEdit->toPlainText();
        OpenFile(&name);
    } catch(...) {
        return false;
    }
    return true;
}

bool MainWindow::Save(QString* p) {
    if (activeFile == nullptr) return false;
    bool r = false;
    if (!fileExists)
        r = SaveAs();
    else
        r = SaveFile();
    textEdited();
    return r;
}

void MainWindow::NewFile() {
    if (!safelyClose()) return;
    reset();
    activeFile = new PlaceHolderFile("Untitled");
    fileExists = false;
    ACTIVE_TITLE = activeFile->fileName() + " — " + TITLE;
    setWindowTitle(ACTIVE_TITLE);
    ui->plainTextEdit->setVisible(true);
}

void MainWindow::OpenFile(QString* p) {
    if (!safelyClose()) return;
    QString path = QString();
    if (p == nullptr) {
        path = QFileDialog::getOpenFileName(this,"Open File...",fileExists? activeFile->dir().path() : QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        if (path == QString()) return;
    } else {
        path = *p;
    }
    auto err = [this](QString exception) {
        QMessageBox::critical(this,"Error — " + TITLE,exception,QMessageBox::StandardButton::Ok,QMessageBox::StandardButton::Ok);
    };
    if (QFileInfo(path).isDir()) return err(path + " is a directory.");
    try {
        QFile f(path);
        if (f.open(QIODevice::ReadOnly|QIODevice::Text) || f.open(QIODevice::ReadOnly)) {
            QTextStream in(&f);
            reset();
            ui->plainTextEdit->setPlainText(in.readAll());
            f.close();
            ui->plainTextEdit->setVisible(true);
            activeFile = new QFileInfo(path);
            fileExists = true;
            ACTIVE_TITLE = activeFile->fileName() + " — " + TITLE;
            setWindowTitle(ACTIVE_TITLE);
            initData = ui->plainTextEdit->toPlainText();
            textEdited();
        } else {
            err("Could not open file: " + f.errorString());
        }
    } catch(...) {}
}

void MainWindow::reset() {
    activeFile = nullptr;
    fileExists = false;
    initData = QString();
    ui->plainTextEdit->clear();
    setWindowTitle(TITLE);
}

void MainWindow::textEdited() {
    if (initData != ui->plainTextEdit->toPlainText())
        setWindowTitle("*" + ACTIVE_TITLE);
    else
        setWindowTitle(ACTIVE_TITLE);
}

void MainWindow::DeleteFile() {
    if (activeFile == nullptr || !fileExists) return;
    QMessageBox::StandardButton opt = QMessageBox::warning(this,"Delete File — CextEdit",QString("Delete \"%1\" permanently?").arg(activeFile->fileName()),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if (opt == QMessageBox::No) return;
    QFile f(activeFile->filePath());
    auto err = [this](QString exception) {
        QMessageBox::critical(this,"Error — " + TITLE,exception,QMessageBox::StandardButton::Ok,QMessageBox::StandardButton::Ok);
    };
    if (!f.remove())
        return err("Couldn't delete file: " + f.errorString());
    reset();
    safelyClose();
    ACTIVE_TITLE = TITLE;
    setWindowTitle(TITLE);
    ui->plainTextEdit->setVisible(false);
}

void MainWindow::setFontSize(int pt, int mode) {
    QFont font(ui->plainTextEdit->font());
    font.setPointSize(mode == 0? pt : font.pointSize() + pt);
    ui->plainTextEdit->setFont(font);
}
