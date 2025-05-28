#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "aboutdialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AboutDialog* aboutWin;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void NewFile();
    void OpenFile();
    void SaveFile();
    void SaveAs();
    void Save();
    void DeleteFile();
};
#endif // MAINWINDOW_H
