#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    aboutWin = new AboutDialog();

    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);
}

MainWindow::~MainWindow()
{
    delete ui;
}
