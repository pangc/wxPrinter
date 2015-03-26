#ifndef PICTUREINFO_H
#define PICTUREINFO_H
#include <QString>
#include <QMetaType>
#include "json.h"
#include <QImage>
class PictureInfo
{
public:
    PictureInfo();
    PictureInfo(QString pictureID, QString url, QString size);
    QString pictureID;
    QString size;
    QString url;
    QString num;
    QString scode;
    QString data;


};
Q_DECLARE_METATYPE(PictureInfo);
#endif // PICTUREINFO_H
