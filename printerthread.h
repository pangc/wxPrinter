#ifndef PRINTERTHREAD_H
#define PRINTERTHREAD_H
#include <QThread>
#include "serverapi.h"
#include "pictureinfo.h"
#include <QThread>
#include <QObject>
#include <QImage>
#include <QDateTime>
#include <QPrinter>
#include <QPainter>
#include <QMutex>
#include <windows.h>
#include "ibbdprinterconfig.h"

#define PRINTER_IDLE 0
#define PRINTER_PRINTING 1
#define PRINTER_ERROR 2
#define PRINTER_OFFLINE 3
class PrinterThread : public QThread
{
Q_OBJECT
public:
    PrinterThread();
    PrinterThread(QPrinter *pr);
    QPrinter *printer;
    //PrinterThread(ServerAPI *);
    void run();
    QList <PictureInfo> task;
    void print(PictureInfo info);
    QNetworkAccessManager *qnamPic;
    QImage *img;
    bool havePic;
    bool status;
    bool checkPrint();
    int errorCode;
    QMutex mutex;
    void getPicture(QString url);
signals:
    void printTreadStart(PictureInfo info);
    void printThreadSucc(PictureInfo info,int time);
    void printThreadFail(PictureInfo info,int error,int time);
public  slots:
    void replyPic(QNetworkReply *reply);

};

#endif // PRINTERTHREAD_Hre
