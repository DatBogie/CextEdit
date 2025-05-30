#include "preferences.h"
#include "ui_preferences.h"

#include <QFontComboBox>
#include <QComboBox>
#include <QStyleFactory>

Preferences::Preferences(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Preferences)
{
    ui->setupUi(this);
    setWindowTitle("Preferences — CextEdit");
    hide();
    ui->widgetsCombo->addItem("System");
    ui->widgetsCombo->addItems(QStyleFactory::keys());
    connect(ui->fontCombo,&QFontComboBox::currentFontChanged,this,[this]() {
        emit FontUpdated();
    });
    connect(ui->widgetsCombo,&QComboBox::currentTextChanged,this,[this]() {
        emit WStyleUpdated();
    });
}

Preferences::~Preferences()
{
    delete ui;
}
