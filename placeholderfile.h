#ifndef PLACEHOLDERFILE_H
#define PLACEHOLDERFILE_H

#include <QFileInfo>
#include <QString>

class PlaceHolderFile : public QFileInfo {
public:
    explicit PlaceHolderFile(QString name) { this->name = name; }
    QString fileName() { return name; }
private:
    QString name;
};

#endif // PLACEHOLDERFILE_H
