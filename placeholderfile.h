#ifndef PLACEHOLDERFILE_H
#define PLACEHOLDERFILE_H

#include <QFileInfo>
#include <QString>

class PlaceHolderFile : public QFileInfo
{
public:
    PlaceHolderFile(QString name);
    // QString fileName();
    QString name;
};

#endif // PLACEHOLDERFILE_H
