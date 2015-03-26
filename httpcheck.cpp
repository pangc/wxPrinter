#include "httpcheck.h"

HttpCheck::HttpCheck()
{
}

HttpCheck::HttpCheck(QString h, int n)
{
    begin = QDateTime::currentDateTime().toTime_t();
    receive = 0;
    error = 0;
    timer = new QTimer;
    host = h;
    times = n;
    timer->start(4000*n);
    qam = new QNetworkAccessManager;
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutFunction()));
    connect(qam,SIGNAL(finished(QNetworkReply*)),this,SLOT(getReply(QNetworkReply*)));
}

HttpCheck::~HttpCheck()
{
    qam->~QNetworkAccessManager();
    timer->~QTimer();
}

void HttpCheck::checkHost()
{
    if(times>0)
    {
        qDebug()<<host;
        qam->get(QNetworkRequest(host));
    }
    if(times == 0)
    {
        qDebug()<<"receive"<< receive;
        qDebug()<<"error"<< error;
        end = QDateTime::currentDateTime().toTime_t();
        qDebug()<<end - begin;
        emit checkFinish(QString::number(receive));
    }
}

void HttpCheck::getReply(QNetworkReply *reply)
{
    times = times -1;
    if(reply->error()==QNetworkReply::NoError)
    {
        receive++;
    }
    else
    {
        error++;
    }
    checkHost();
}

void HttpCheck::timeoutFunction()
{
    timer->stop();
    emit checkFinish("error");
    this->~HttpCheck();
}

