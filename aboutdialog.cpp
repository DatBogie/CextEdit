#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QDialog>

AboutDialog::AboutDialog()
    : QDialog(), ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowTitle("About — CextEdit");
}

AboutDialog::~AboutDialog() {
    delete ui;
}
