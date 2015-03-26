#ifndef IBBDPRINTER_H
#define IBBDPRINTER_H
#include <QApplication>

#include "pictureinfo.h"
#include "serverapi.h"
#include "printerthread.h"
#include <QObject>
#include <QThread>
#include <QPrinterInfo>
#include <QPrinter>
#include <QTimer>
#include <QFile>
#include <QCryptographicHash>

#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkConfigurationManager>
#include "ibbdprinterconfig.h"

#define VERIFY_SUCC 0
#define VERIFY_REGIST 1
#define VERIFY_ERROR 2

class IBBDPrinter : public QObject
{
Q_OBJECT
public:
    IBBDPrinter();
    ServerAPI *api;
    //void print(PictureInfo info);
    QList <PictureInfo> task;
    PrinterThread *pThread;
    QPrinter *printer;
    QTimer *timer;
    int verifyRegister();

signals:
    void printStart(PictureInfo info);
    void printSucc(PictureInfo info,int time);
    void printFail(PictureInfo info,int error,int time);
public  slots:
    void runTask(QList <PictureInfo>  task);
    void printSuccReturn(PictureInfo info,int time);
    void printFailReturn(PictureInfo info,int error,int time);
    void printStatusReturn();

};

#endif // IBBDPRINTER_H
