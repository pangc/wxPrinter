#include "printerthread.h"
#include <QDebug>
PrinterThread::PrinterThread()
{
    havePic = false;
    img = new QImage();
    qnamPic = new QNetworkAccessManager();

}

PrinterThread::PrinterThread(QPrinter *pr)
{
    printer = pr;
    havePic = false;

    img = new QImage();
    }

void PrinterThread::replyPic(QNetworkReply *reply)
{

    QPainter painter(printer);
    errorCode = 0;
    qDebug()<<"replyPic";
    qDebug()<<printer->printerName();
    mutex.lock();
    if(reply->error() == QNetworkReply::NoError){
        img->loadFromData(reply->readAll());
        qDebug()<<"reply img:"<<img->size();
        img->save("temp.jpg");
        QImage imageTemp("temp.jpg");
        QRect rect = painter.viewport();
        QSize size = imageTemp.size();
        if((size.height()!=0)&&(size.width()!=0)){
            size.scale(rect.size(), Qt::KeepAspectRatio);
            painter.setViewport(rect.x(), rect.y(),74*3.8, 105*3.8);
            painter.setWindow(imageTemp.rect());
            painter.drawImage(0, 0, imageTemp);
            status = painter.begin(printer);
        }
        else
        {
            errorCode = 2;
        }
    }
    this->exit();
}





void PrinterThread::run()
{
    qnamPic = new QNetworkAccessManager();
    connect(qnamPic, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyPic(QNetworkReply*)));


    for(int i=0;i<task.count();i++)
    {
        print(task.at(i));
    }
    task.clear();
}
void PrinterThread::getPicture(QString url)
{
    qnamPic->get(QNetworkRequest(url));
    this->exec();
}
void PrinterThread::print(PictureInfo info)
{
        emit printTreadStart(info);

        QString filename;
        int time = 0;
        int beginTime,endTime;
        beginTime =QDateTime::currentDateTime().toTime_t();
        qDebug()<<"printing..."<< info.url;
        printer->setDocName(info.pictureID);
        printer->setCopyCount(info.num.toInt());
        getPicture(info.url);
        qDebug()<<"get picture";

        qDebug()<<img->size();
        bool status;
        if(errorCode!=2)
        {
            status = checkPrint();
        }
        else
        {
            status = false;
        }


        qDebug()<<time;
        if(status)
        {
            endTime =QDateTime::currentDateTime().toTime_t();
            time = endTime - beginTime;
            qDebug()<<"print:url:"<<info.url;
            qDebug()<<"print:id:"<<info.pictureID;
            emit printThreadSucc(info,time);
        }
        else
        {
            endTime =QDateTime::currentDateTime().toTime_t();
            time = endTime - beginTime;
            emit printThreadFail(info,errorCode,time);
            qDebug()<<"fail";
        }
        mutex.unlock();
}



bool PrinterThread::checkPrint()
{
    QString printerName = printer->printerName();
    int beginTime;
    HANDLE   PrnHandle=0;
    LPWSTR lpw =(LPTSTR)(printerName.utf16());
    if(OpenPrinter(lpw,&PrnHandle,NULL))
    {
        PRINTER_INFO_2*   pInfo;
        DWORD   dwSize;
        unsigned    char   buf[8192];
        pInfo   =   (PRINTER_INFO_2*)buf;
        GetPrinter(PrnHandle,2,buf,sizeof(buf),&dwSize);
        qDebug()<<"jobs"<<pInfo->cJobs;
        int currJobNum = pInfo->cJobs;
        status = PRINTER_PRINTING;
        DWORD pcbNeeded=0,pcReturned=0;
        JOB_INFO_2 *       pJobInfo = NULL;
        if(!EnumJobs((HANDLE)PrnHandle, 0, 1, 2, (LPBYTE)pJobInfo,pcbNeeded, (LPDWORD)&pcbNeeded, (LPDWORD)&pcReturned))
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
               if (pJobInfo) LocalFree(pJobInfo);
                pJobInfo = (JOB_INFO_2 *) LocalAlloc(LPTR, pcbNeeded);
            }
            else
            {
              qDebug()<<"EnumJobs fail";
               GetLastError();
            }
        }
        qDebug()<<pcReturned;
        qDebug()<<"beging task"<<pJobInfo->JobId;
        qDebug()<<QString::fromUtf16(pJobInfo->pDocument);
       beginTime = QDateTime::currentDateTime().toTime_t();
       int tmpTime = QDateTime::currentDateTime().toTime_t();
       while(pInfo->cJobs>0){
           int i=0;
           if((QDateTime::currentDateTime().toTime_t() - tmpTime)>1)
           {
               if((QDateTime::currentDateTime().toTime_t() - beginTime)>80){
                   qDebug()<<"over time";
                   errorCode = 1;
                   return false;
               }
               GetPrinter(PrnHandle,2,buf,sizeof(buf),&dwSize);
               tmpTime = QDateTime::currentDateTime().toTime_t();
               i++;
           }

       }
      qDebug()<<"complete task";
      return true;
    }
    errorCode = 3;
    return false;
}



