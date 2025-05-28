#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

#include <QCloseEvent>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowTitle("CextEdit");
    ui->setupUi(this);
    ui->plainTextEdit->setVisible(false);

    aboutWin = new AboutDialog();

    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);
    connect(ui->actionUndo,&QAction::triggered,ui->plainTextEdit,&QPlainTextEdit::undo);
    connect(ui->actionRedo,&QAction::triggered,ui->plainTextEdit,&QPlainTextEdit::redo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::safelyClose() {
    if (!activeFile || initData == ui->plainTextEdit->toPlainText()) return true;
    QMessageBox.StandardButton* opt = QMessageBox::warning(this,"Close File — CextEdit",QString("File \"%1\" has unsaved changes. What would you like to do?").arg(activeFile.fileName()),QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Cancel);
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

void MainWindow::NewFile() {
    ui->plainTextEdit->setVisible(true);
}
