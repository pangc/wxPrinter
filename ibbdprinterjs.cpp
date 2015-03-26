#include "ibbdprinterjs.h"

IBBDPrinterJS::IBBDPrinterJS()
{
    printer = new IBBDPrinter;
    connect(this->printer->api,SIGNAL(downloadFinish(QString)),this,SLOT(evaluateDownloadCallback(QString)));
    version = "20141111";
}

void IBBDPrinterJS::updateKeyMapFile(QMap<QString, QString> keymap)
{
    QFile file("cache.key");
    if(!file.open(QFile::ReadWrite |QFile::Text))//以只写方式打开文件，如果打开失败则弹出提示框并返回
    {
        qDebug()<< "can't open log file";
    }
    QTextStream textStream(&file);
    QMap<QString,QString>::iterator it;
    for(it=keymap.begin();it!=keymap.end();it++)
        textStream<<it.key()<<"="<<it.value()<<endl;
    file.close();
}

void IBBDPrinterJS::loadKeyMapFile(QMap<QString, QString> &keyMap)
{
    QFile file("cache.key");
    if(!file.open(QFile::ReadWrite |QFile::Text))//以只写方式打开文件，如果打开失败则弹出提示框并返回
    {
        qDebug()<< "can't open log file";
    }
    QByteArray ba;
    int i=0;
    while(!file.atEnd())
    {
        ba = file.readLine();
        QString line(ba);
        int pos = line.indexOf("=");
        qDebug()<<pos;
        if(pos>0)
        {
            QString key = line.mid(0,pos);
            QString value = line.mid(pos+1,-1);
            keyMap.insert(key,value);
        }
    }
    qDebug()<<keyMap;
    file.close();
}
QString IBBDPrinterJS::getVersion()
{
    return version;
}

QString IBBDPrinterJS::getToken()
{
    qDebug()<<"12345";
    int time = QDateTime::currentDateTime().toTime_t();
    return "sdafghewon335tyqng38nb4";
}

QString IBBDPrinterJS::calToken(QString code, int time)
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


QString IBBDPrinterJS::token()
{
    QString code = this->printer->api->code;
    int time = QDateTime::currentDateTime().toTime_t();
    QString token = this->printer->api->calToken(code,time);
    QtJson::JsonObject result;
    result["time"] = QString::number(time);
    result["code"] = code;
    result["token"] = token;
    QString str = QtJson::serializeStr(result);
    qDebug()<< str;
    return str;
}

QString IBBDPrinterJS::getCode()
{
    return printer->api->code;
}

QString IBBDPrinterJS::getLog()
{
    QFile file("printer.log");
    if(!file.open(QFile::ReadOnly |QFile::Text))//以只写方式打开文件，如果打开失败则弹出提示框并返回
    {
        return "can't open log file";
    }
    QString str = file.readAll();
    return str;

}

void IBBDPrinterJS::ping(QString host, int times,QString functionName)
{
    pingFunction = functionName;
    qDebug()<<host<<times;
    httpcheck = new HttpCheck(host,times);
    httpcheck->checkHost();
    connect(httpcheck,SIGNAL(checkFinish(QString)),this,SLOT(evaluatePingJs(QString)));
}

void IBBDPrinterJS::bindEvent(QString event, QString functionName)
{
    if(event == "PRINT_START")
    {
        printStartFunction = functionName;
        connect(printer,SIGNAL(printStart(PictureInfo)),this,SLOT(evaluatePrintStartJS(PictureInfo)));
    }
    else if(event == "PRINT_SUCC")
    {
        printSuccFunction = functionName;
        connect(printer,SIGNAL(printSucc(PictureInfo,int)),this,SLOT(evaluatePrintSuccJS(PictureInfo,int)));

    }
    else if(event == "PRINT_FAIL")
    {
        printFailFunction = functionName;
        connect(printer,SIGNAL(printFail(PictureInfo,int,int)),this,SLOT(evaluatePrintFailJS(PictureInfo,int,int)));
    }
}



void IBBDPrinterJS::evaluatePrintStartJS(PictureInfo info)
{
    QString method =printStartFunction+"(\"{ \\\"id\\\":\\\""+info.pictureID+"\\\", \\\"data\\\":"+info.data+" }\",\"\")";
    qDebug()<<"method"<<method;
    this->webview->page()->mainFrame()->evaluateJavaScript(method);

}

void IBBDPrinterJS::evaluatePrintSuccJS(PictureInfo info, int time)
{

    QString method = printSuccFunction+"(\"{ \\\"id\\\" : \\\""+info.pictureID+"\\\", \\\"time\\\" : \\\""+QString::number(time)+"\\\" }\",\"\")";

    this->webview->page()->mainFrame()->evaluateJavaScript(method);
}

void IBBDPrinterJS::evaluatePrintFailJS(PictureInfo info, int time, int error)
{
    QString method = printFailFunction+"(\"{ \\\"id\\\" : \\\""+info.pictureID+"\\\", \\\"time\\\" : \\\""+QString::number(time)+"\\\" , \\\"error\\\" : \\\""+QString::number(error)+"\\\"}\",\"\")";

    this->webview->page()->mainFrame()->evaluateJavaScript(method);
}

void IBBDPrinterJS::refresh()
{
    QString printername = this->printer->printer->printerName();
    QString url = IBBDPrinterConfig::config->value("server/serverName","NULL").toString()+"/Index/index";
    QString code = this->printer->api->code;
    url = url + "/code/" + code;
    QDateTime time;
    QString timeStr;
    time = QDateTime::currentDateTime();
    //qDebug()<<time.toTime_t();
    timeStr = QString::number(time.toTime_t());
    url = url + "/time/" + timeStr;
    QString token = this->printer->api->calToken(code,time.toTime_t());
    url = url + "/token/" +token;
    this->webview->load(QUrl(url));
}

void IBBDPrinterJS::evaluatePingJs(QString receive)
{
    QString method = pingFunction + "(\""+receive+"\")";
    qDebug()<<"method"<< method;
    this->webview->page()->mainFrame()->evaluateJavaScript(method);
}

void IBBDPrinterJS::cache(QString url, QString key, QString suffix,int expired, QString callback)
{
    callbackFunction = callback;
    qDebug()<<"function:"<<callback;
    qDebug()<<"key:"<<key;
    QString md5;
    QByteArray bb;
    bb = QCryptographicHash::hash ( key.toAscii(), QCryptographicHash::Md5 );
    md5.append(bb.toHex()).mid(8,16);
    key = md5;
    QString fileName = key+"."+suffix;
    QMap<QString,QString> keyMap;
    loadKeyMapFile(keyMap);
    QMap<QString,QString>::iterator it = keyMap.find(key);
    qDebug()<<keyMap;
    if(it!=keyMap.end())
    {
        qDebug()<<it.value();
        qDebug()<<url;
        if(it.value().replace("\n","")==url)
        {
            QString method = callback+"(\'cache\\\\"+key+"."+suffix+"\')";
            QFileInfo fileInfo("cache\\"+fileName);
            qDebug()<<"fileName:"<< fileName;
            int lived;
            if(fileInfo.exists())
            {
                lived = QDateTime::currentDateTime().toTime_t() - fileInfo.lastModified().toTime_t();
                if(lived>expired){
                    qDebug()<<"lived:"<<lived<<"expired:"<<expired;

                    this->printer->api->downloadFile(url,fileName);
                }
                else
                    this->webview->page()->mainFrame()->evaluateJavaScript(method);
            }
            else
            {
                this->printer->api->downloadFile(url,fileName);
            }
        }
        else
        {
            keyMap[key]=url;
            this->printer->api->downloadFile(url,fileName);
        }
    }
    else
    {
        keyMap.insert(key,url);
        this->printer->api->downloadFile(url,fileName);
    }
    updateKeyMapFile(keyMap);
}

void IBBDPrinterJS::evaluateDownloadCallback(QString path)
{
    QString method = callbackFunction+"(\'cache\\\\"+path+"\')";
    qDebug()<<method;
    this->webview->page()->mainFrame()->evaluateJavaScript(method);
}



void IBBDPrinterJS::setVersion(const QString &_version)
{
    version = _version;
    emit sendversionchange();
}


