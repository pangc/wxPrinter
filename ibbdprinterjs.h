#ifndef IBBDPRINTERJS_H
#define IBBDPRINTERJS_H
#include <QObject>
#include <QDateTime>
#include <QFile>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QDebug>
#include <QFileInfo>
#include "ibbdprinter.h"
#include "ibbdprinterconfig.h"
#include "httpcheck.h"
#include "json.h"
class IBBDPrinterJS : public QObject
{
Q_OBJECT
public:
    Q_PROPERTY(QString version READ getVersion WRITE setVersion NOTIFY sendversionchange );
    QString version;
    QString printSuccFunction;
    QString printStartFunction;
    QString printFailFunction;
    QString pingFunction;
    QString callbackFunction;
    QString getVersion();
    QString calToken(QString code,int time);
    IBBDPrinter *printer;
    HttpCheck *httpcheck;
    QWebView *webview;
    void setVersion(const QString &_version);
    IBBDPrinterJS();
private:
    void updateKeyMapFile(QMap<QString,QString>  keymap);
    void loadKeyMapFile(QMap<QString,QString> &keyMap);
signals:
  void sendversionchange(void);
public slots:
    QString getToken();
    QString token();
    QString getCode();
    void cache(QString url,QString key,QString suffix,int expired,QString callback);
    void refresh();
    QString getLog();
    void ping(QString host,int times,QString functionName);
    void bindEvent(QString event,QString functionName);
private slots:
    void evaluatePrintStartJS(PictureInfo info);
    void evaluatePrintSuccJS(PictureInfo info,int time);
    void evaluatePrintFailJS(PictureInfo info,int time,int error);
    void evaluatePingJs(QString);
    void evaluateDownloadCallback(QString path);

};
#endif // IBDPRINTERJS_H
