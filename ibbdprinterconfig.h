#ifndef IBBDPRINTERCONFIG_H
#define IBBDPRINTERCONFIG_H
#include <QSettings>
class IBBDPrinterConfig
{
public:
    IBBDPrinterConfig();
    static QSettings *config;
};
#endif // IBBDPRINTERCONFIG_H
