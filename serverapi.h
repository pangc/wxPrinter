#ifndef SERVERAPI_H
#define SERVERAPI_H
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QCryptographicHash>
#include <QTimer>
#include "pictureinfo.h"
#include <QUrl>
#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QObject>
#include <QList>
#include <QSet>
#include <QFile>
#include <windows.h>
#include "json.h"
#include "ibbdprinterconfig.h"
#define IBBDPRINTER_IDLE 0
#define IBBDPRINTER_OFFLine 1
#define IBBDPRINTER_WORKING 2
#define IBBDPRINTER_UNDEFINE 3

class ServerAPI : public QObject
{
Q_OBJECT
public:
    ServerAPI();
    QUrl serverURL;
    QString token;
    QString code;
    QTimer *timer;
    int statusReturnCount;
    bool requestStatus;
    void failReturn(QString photoID,int usedtime,int error,QString scode);
    void succReturn(QString photoID,int usedtime,QString scode);
    void statusReturn(QString printerName);
    void registerPost(QString code);
    void getTask();
    void downloadFile(QString url,QString fileName);
    void setToken(QString tk);
    void setCode(QString c);
    void setServerURL(QString url);
    QString calToken(QString code,int time);
    QList<PictureInfo> task;
    QSet<QString> currPhotoTask;
private:
    QNetworkAccessManager *qnamTask,*qnamFail,*qnamSucc,*qnamNoReply,*qamDownloadFile;
    QNetworkRequest *request;
    QString cacheFileName;
signals:
    void newTask(QList<PictureInfo> task);
    void downloadFinish(QString path);
private slots:
    void replyTask(QNetworkReply*);
    void replyFail(QNetworkReply*);
    void replySucc(QNetworkReply*);
    void replyNoReply(QNetworkReply*);
    void replyDownloadFile(QNetworkReply*);
    void timerFunction();
};

#endif // SERVERAPI_H
