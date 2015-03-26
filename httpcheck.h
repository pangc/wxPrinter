#ifndef HTTPCHECK_H
#define HTTPCHECK_H
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QObject>
#include <QDebug>
#include <QDateTime>

class HttpCheck : public QObject
{
Q_OBJECT
public:
    HttpCheck();
    HttpCheck(QString h,int n);
    ~HttpCheck();
    void checkHost();
    QString host;
    QTimer *timer;
    int times;
    int receive;
    int error,begin,end;
    QNetworkAccessManager *qam;
signals:
    void checkFinish(QString result);
private slots:
    void getReply(QNetworkReply*);
    void timeoutFunction();
};

#endif // HTTPCHECK_H
