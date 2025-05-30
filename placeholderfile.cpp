#include "placeholderfile.h"

#include <QFileInfo>
#include <QString>

PlaceHolderFile::PlaceHolderFile(QString name) : QFileInfo(name), name(name) {}

// QString PlaceHolderFile::fileName() {
//     return this->name;
// }
