#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QWidget>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class AboutDialog;
}
QT_END_NAMESPACE

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    AboutDialog();
    ~AboutDialog();
private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
