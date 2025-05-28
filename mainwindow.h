#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "aboutdialog.h"
#include <QFile>

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
    void setFontSize();
    bool safelyClose();
    QFile activeFile;
    bool fileExists;
    QString initData;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void NewFile();
    void OpenFile();
    bool SaveFile();
    bool SaveAs();
    bool Save();
    void DeleteFile();
    void textEdited();
};
#endif // MAINWINDOW_H
