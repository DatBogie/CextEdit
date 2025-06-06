#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "aboutdialog.h"
#include "preferences.h"

#include <QMainWindow>
#include <QFileInfo>

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
    Preferences* pref;
    void setFontSize(int pt, int mode = 0);
    bool safelyClose();
    QFileInfo* activeFile = nullptr;
    bool fileExists = false;
    QString initData;
    QString TITLE;
    QString ACTIVE_TITLE;
    void reset();
    int DEFAULT_FONT_SIZE;
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void NewFile();
    void OpenFile(QString* p = nullptr);
    bool SaveFile(QString* p = nullptr);
    bool SaveAs(QString* p = nullptr);
    bool Save(QString* p = nullptr);
    void DeleteFile();
    void textEdited();
};
#endif // MAINWINDOW_H
