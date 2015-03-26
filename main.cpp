#include <QApplication>
#include "mymainwindow.h"
#include "ibbdprinterconfig.h"
#include <QTextCodec>
QSettings * IBBDPrinterConfig::config = new QSettings("printer.ini",QSettings::IniFormat);

int main(int argc, char *argv[])
{
    IBBDPrinterConfig::config->setIniCodec("UTF-8");
    QApplication a(argc, argv);
    a.setOverrideCursor(Qt::BlankCursor);
    qRegisterMetaType<PictureInfo>("PictureInfo");
    MyMainWindow w;

    w.show();
    
    return a.exec();
}
