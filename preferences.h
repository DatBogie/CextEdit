#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QWidget>

namespace Ui {
class Preferences;
}

class Preferences : public QWidget
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = nullptr);
    ~Preferences();
    Ui::Preferences *ui;

signals:
    void FontUpdated();
    void WStyleUpdated();
};

#endif // PREFERENCES_H
