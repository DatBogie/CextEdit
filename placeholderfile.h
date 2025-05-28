#ifndef PLACEHOLDERFILE_H
#define PLACEHOLDERFILE_H

#include <QFile>
#include <QString>

class PlaceHolderFile : public QFile
{
    Q_OBJECT
public:
    explicit PlaceHolderFile(QString name) { this->name = name; }
private:
    QString name;
};

#endif // PLACEHOLDERFILE_H
