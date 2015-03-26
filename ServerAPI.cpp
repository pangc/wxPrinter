#include "ServerAPI.h"

ServerAPI::ServerAPI()
{
    timer = new QTimer();
    statusReturnCount = 1;
    connect(timer, SIGNAL(timeout()), this, SLOT(timerFunction()));
    timer->start(10000);
    currPhotoTask.clear();
    qnamTask = new QNetworkAccessManager();
    qnamFail = new QNetworkAccessManager();
    qnamSucc = new QNetworkAccessManager();
    qnamNoReply = new QNetworkAccessManager();
    qamDownloadFile = new QNetworkAccessManager();
    request = new QNetworkRequest();
    //serverURL = QUrl("http://wp.ibbd.net/index.php/Home/Printer");
    connect(qnamNoReply,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyNoReply(QNetworkReply*)));
    connect(qnamTask,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyTask(QNetworkReply*)));
    connect(qnamFail,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFail(QNetworkReply*)));
    connect(qnamSucc,SIGNAL(finished(QNetworkReply*)),this,SLOT(replySucc(QNetworkReply*)));
    connect(qamDownloadFile,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyDownloadFile(QNetworkReply*)));
}

void ServerAPI::failReturn(QString photoID, int usedtime, int error,QString scode)
{
    QString url = this->serverURL.toString();
    if(code.isEmpty())
    {
        return;
    }
    url = url + "/fail/code/"+code;
    url = url + "/scode/"+scode;
    url = url + "/photo/"+photoID;
    url = url + "/error/"+QString::number(error);
    url = url + "/use/"+QString::number(usedtime);
    QDateTime time;
    QString timeStr;
    time = QDateTime::currentDateTime();
    //qDebug()<<time.toTime_t();
    timeStr = QString::number(time.toTime_t());
    url = url + "/time/" + timeStr;
    token = this->calToken(code,time.toTime_t());
    url = url + "/token/" + token;
    qnamFail->get(QNetworkRequest(url));
    //qDebug()<<url;
}

void ServerAPI::succReturn(QString photoID, int usedtime,QString scode)
{
    QString url = serverURL.toString();
    if(code.isEmpty())
    {
        return;
    }
    url = url + "/succ/code/"+code;
    url = url + "/scode/"+scode;
    url = url + "/photo/"+photoID;
    url = url + "/use/"+QString::number(usedtime);
    QDateTime time;
    QString timeStr;
    time = QDateTime::currentDateTime();
    timeStr = QString::number(time.toTime_t());
    url = url + "/time/" + timeStr;
    token = this->calToken(code,time.toTime_t());
    url = url + "/token/" + token;
    qnamSucc->get(QNetworkRequest(url));
    //qDebug()<<url;
}

void ServerAPI::statusReturn(QString printerName)
{
    HANDLE   PrnHandle=0;
    LPWSTR lpw =(LPTSTR)(printerName.utf16());
    int statusCode = IBBDPRINTER_UNDEFINE;
    if(OpenPrinter(lpw,&PrnHandle,NULL))
    {
        PRINTER_INFO_2*   pInfo;
        DWORD   dwSize;
        unsigned    char   buf[8192];
        pInfo   =   (PRINTER_INFO_2*)buf;
        GetPrinter(PrnHandle,2,buf,sizeof(buf),&dwSize);
        if (pInfo->Attributes&PRINTER_ATTRIBUTE_WORK_OFFLINE)//测试成功
        {
            statusCode = IBBDPRINTER_OFFLine;
        }
        else
        {
            //int currJobNum = pInfo->cJobs;
            if(pInfo->cJobs>0)
            {
                statusCode = IBBDPRINTER_WORKING;
            }
            else
            {
                statusCode = IBBDPRINTER_IDLE;
            }
        }
    }
    QString url = serverURL.toString();
    url = url + "/status/code/"+code;
    url = url + "/msg/"+statusCode;
    QDateTime time;
    QString timeStr;
    time = QDateTime::currentDateTime();
    timeStr = QString::number(time.toTime_t());
    url = url + "/time/" + timeStr;
    url = url + "/num/" + QString::number(statusReturnCount);
    token = this->calToken(code,time.toTime_t());
    url = url + "/token/" + token;
    qnamSucc->get(QNetworkRequest(url));
    statusReturnCount++;

}

void ServerAPI::registerPost(QString code)
{
    QString url = serverURL.toString();
    if(code.isEmpty())
    {
        return;
    }
    url = url + "/reg/code/"+code;
    QDateTime time;
    QString timeStr;
    time = QDateTime::currentDateTime();
    timeStr = QString::number(time.toTime_t());
    url = url + "/time/" + timeStr;
    token = this->calToken(code,time.toTime_t());
    url = url + "/token/" + token;
    qnamNoReply->get(QNetworkRequest(url));
    //qDebug()<<url;
}
void ServerAPI::getTask()
{
     QString url = serverURL.toString();
    if(code.isEmpty())
    {
       return;
    }
    url = url + "/task/code/"+code;
    QString timeStr;
    QDateTime time;
    time = QDateTime::currentDateTime();
    //qDebug()<<time.toTime_t();
    timeStr = QString::number(time.toTime_t());
    url = url + "/time/"+timeStr;
    token = this->calToken(code,time.toTime_t());
    url= url + "/token/"+token;
    //qDebug()<<"task URL:"<<url;
    QUrl taskURL = QUrl(url);
    request->setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
    request->setUrl(taskURL);
    qnamTask->get(*request);

}

void ServerAPI::downloadFile(QString url,QString fileName)
{
    //qDebug()<<url;
    request->setUrl(QUrl(url));
    cacheFileName = fileName;
   // qDebug()<<"downloadFile:"<<fileName;
    qamDownloadFile->get(*request);
}

void ServerAPI::setToken(QString tk)
{
    this->token = tk;
}

void ServerAPI::setCode(QString c)
{
    this->code = c;
}

void ServerAPI::setServerURL(QString url)
{
    this->serverURL = QUrl(url);
}

QString ServerAPI::calToken(QString code, int time)
{
    time = time;
    QString url = "time="+QString::number(time)+"&";
    url = url + "code="+code+"&";
    url = url + "secret=e856166ea87fbdc83564c5f5d8643ab4";
    QString md5;
    QByteArray bb;
    bb = QCryptographicHash::hash ( url.toAscii(), QCryptographicHash::Md5 );
    md5.append(bb.toHex());
    return md5.mid(8,16);
}



void ServerAPI::replyTask(QNetworkReply *reply)
{
    bool newtask=false;
    PictureInfo pInfo;
    QByteArray ba = reply->readAll();
    bool ok;
    QString jsonstr = QString::fromUtf8(ba);
    //qDebug()<<jsonstr;
    //jsonstr="{\"status\": 0,\"data\":[{\"photo\":123,\"url\":\"http://diuleiloumou.net\",\"preview\":\"http://preview\",\"size\":\"A7\"},{\"photo\":456,\"url\":\"http://diuleiloumou.net2\",\"preview\":\"http://preview2\",\"size\":\"A7\"}]}";
    QtJson::JsonObject json = QtJson::parse(jsonstr, ok).toMap();
    QString status = json["status"].toString();
    QString msg = json["msg"].toString();
    if(status!="0")
    {
        //qDebug()<<"status==0";
        newtask = false;
        requestStatus = false;
        return;
    }
    else
    {
        QtJson::JsonArray dataArray = json["data"].toList();
        qDebug()<<"serverAPI-reply getTask begin:";
        foreach (QVariant data, dataArray) {
            if(currPhotoTask.contains(data.toMap()["photo"].toString()) == false)
            {
                pInfo.pictureID = data.toMap()["photo"].toString();
                pInfo.url = data.toMap()["url"].toString();
                pInfo.size = data.toMap()["size"].toString();
                pInfo.scode = data.toMap()["scode"].toString();
                pInfo.num = data.toMap()["num"].toString();
                pInfo.data = data.toMap()["data"].toString();
                //pInfo.isGetImage = false;
                //printTask(pInfo);
                task.append(pInfo);
                newtask = true;
            }
        }
        for(int i=0;i<task.count();i++)
        {
            qDebug()<< task.at(i).pictureID;
        }
        qDebug()<<"serverAPI-reply getTask end:";

        if(newtask){
            emit newTask(task);
            task.clear();
        }
    }

    requestStatus = true;
}

void ServerAPI::replyFail(QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();
    bool ok;
    QString jsonstr = QString::fromUtf8(ba);
   // qDebug()<<jsonstr;
    QtJson::JsonObject json = QtJson::parse(jsonstr, ok).toMap();
    QString status = json["status"].toString();
    QString msg = json["msg"].toString();
    if(status!="0")
    {
        requestStatus=false;
        return;
    }
    requestStatus=true;
}

void ServerAPI::replySucc(QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();
    bool ok;
    QString jsonstr = QString::fromUtf8(ba);
    qDebug()<<jsonstr;
    QtJson::JsonObject json = QtJson::parse(jsonstr, ok).toMap();
    QString status = json["status"].toString();
    QString msg = json["msg"].toString();
    if(status!="0")
    {
        requestStatus=false;
        return;
    }
    requestStatus=true;
}

void ServerAPI::replyNoReply(QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();
    bool ok;
    QString jsonstr = QString::fromUtf8(ba);
    qDebug()<<jsonstr;
    QtJson::JsonObject json = QtJson::parse(jsonstr, ok).toMap();
    QString status = json["status"].toString();
    QString msg = json["msg"].toString();
    if(status!="0")
    {
        requestStatus=false;
        return;
    }
    requestStatus=true;
}

void ServerAPI::replyDownloadFile(QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();
    QString dirName = "cache\\"+cacheFileName;
    QFile file(dirName);
    file.open(QFile::WriteOnly);
    file.write(ba);
    file.close();
    emit downloadFinish(cacheFileName);
    //qDebug()<<"emit:"<<cacheFileName;
}

void ServerAPI::timerFunction()
{
    timer->stop();
    this->getTask();
    timer->start(5000);
}
