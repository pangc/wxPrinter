#include "ibbdprinterconfig.h"
#include <QTextCodec>

#include <QDebug>
IBBDPrinterConfig::IBBDPrinterConfig()
{
 //   QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    qDebug()<<"printer name:"<<config->value("printer/defaultPrinter","NULL").toString();
    qDebug()<<"printer mode:"<<config->value("printer/printMode","NULL").toString();
    qDebug()<<"server:"<<config->value("server/serverName").toString();
    qDebug()<<"network interface"<<config->value("network interface/name").toString();
}
