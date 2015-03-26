#include "pictureinfo.h"

PictureInfo::PictureInfo()
{
}

PictureInfo::PictureInfo(QString pictureID, QString url, QString size)
{
    this->pictureID = pictureID;
    this->url = url;
    this->size = size;
}
