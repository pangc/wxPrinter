#include "ibbdprinter.h"
#include <QTextCodec>


IBBDPrinter::IBBDPrinter()
{
    timer = new QTimer();
    api = new ServerAPI();
    api->setServerURL(IBBDPrinterConfig::config->value("server/serverName","NULL").toString()+"/Printer");
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    QString interfaceName;
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    QString defaultname = IBBDPrinterConfig::config->value("server/interfaceName","NULL").toString();
    for(int i=0;i<list.count();i++){
        interfaceName = list.at(i).humanReadableName();
        qDebug()<<i<< interfaceName.mid(0,4);
        if(interfaceName.mid(0,4) == defaultname)
        {
            api->setCode(list.at(i).hardwareAddress().remove(':'));
            break;
        }
    }
    QPrinterInfo printerInfo;
    QList<QPrinterInfo> printer_list = QPrinterInfo::availablePrinters();
    for (int i = 0; i < printer_list.length(); i++) {
        QPrinterInfo info = printer_list.at(i);
        QString name = info.printerName();
        if(name==IBBDPrinterConfig::config->value("printer/defaultPrinter","NULL").toString())
            printerInfo = info;
     }
    printer = new QPrinter(printerInfo);
    pThread = new PrinterThread(printer);
    timer->start(1000*3600);
    printStatusReturn();
    connect(timer, SIGNAL(timeout()), this, SLOT(printStatusReturn()));
    connect(pThread,SIGNAL(printTreadStart(PictureInfo)),this,SIGNAL(printStart(PictureInfo)));
    connect(pThread,SIGNAL(printThreadSucc(PictureInfo,int)),this,SIGNAL(printSucc(PictureInfo,int)));
    connect(pThread,SIGNAL(printThreadFail(PictureInfo,int,int)),this,SIGNAL(printFail(PictureInfo,int,int)));
    connect(api,SIGNAL(newTask(QList <PictureInfo> )),this,SLOT(runTask(QList <PictureInfo> )));
    connect(this,SIGNAL(printSucc(PictureInfo,int)),this,SLOT(printSuccReturn(PictureInfo,int)));
    connect(this,SIGNAL(printFail(PictureInfo,int,int)),this,SLOT(printFailReturn(PictureInfo,int,int)));


}

int IBBDPrinter::verifyRegister()
{
    QString md5;
    QString verifyStr;
    QByteArray bb;
    QString code = api->code;
    QFile file("verifyFile");
    if(!file.open(QFile::ReadOnly |QFile::Text))//以只写方式打开文件，如果打开失败则弹出提示框并返回
    {
           qDebug()<<"can't open file";
           file.close();
           QFile writeFile("verifyFile");
           if(!writeFile.open(QFile::ReadWrite |QFile::Text))
           {
               qDebug()<<"can't write File";
           }
           verifyStr = "code="+code+"version=20141120secret=e856166ea87fbdc83564c5f5d8643ab4";
           qDebug()<<verifyStr;
           bb = QCryptographicHash::hash ( verifyStr.toAscii(), QCryptographicHash::Md5 );
           md5.append(bb.toHex());
           writeFile.write(md5.toAscii());
           writeFile.close();
           api->registerPost(api->code);
           return VERIFY_REGIST;
    }
    QString str = file.readAll();
    verifyStr = "code="+code+"version=20141120secret=e856166ea87fbdc83564c5f5d8643ab4";
    bb = QCryptographicHash::hash ( verifyStr.toAscii(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());
    if(str == md5)
    {
        return VERIFY_SUCC;
    }
    else
    {
        return VERIFY_ERROR;
    }
}



void IBBDPrinter::runTask(QList <PictureInfo> task)
{

   if(task.count()>0){
    pThread->task.append(task);
    pThread->start();
   }
}

void IBBDPrinter::printSuccReturn(PictureInfo info, int time)
{
    api->succReturn(info.pictureID,time,info.scode);
}

void IBBDPrinter::printFailReturn(PictureInfo info, int error, int time)
{
    api->failReturn(info.pictureID,time,error,info.scode);
}

void IBBDPrinter::printStatusReturn()
{
    timer->stop();
    api->statusReturn(printer->printerName());
    timer->start(1000*3600);
}

