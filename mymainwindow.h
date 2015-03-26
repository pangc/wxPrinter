#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H
#include "wlansetting.h"
#include "ibbdprinterjs.h"
#include <QMainWindow>
#include <QTimer>
#include <QDir>
#include <QFile>

#include <QHBoxLayout>//for use of class QHBoxLayout
#include <QPushButton>//for class QPushButton
#include <QLineEdit>//for class QLineEdit
#include <QtWebKit/QWebView>//for class QWebView
#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>
#include <QVBoxLayout>//for class QVBoxLayout
#include <QWidget>
#include <QLabel>
#include <QList>
#include <QtNetwork/QNetworkConfiguration>
#include <QtNetwork/QNetworkConfigurationManager>
class MyMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MyMainWindow(QWidget *parent = 0);
    IBBDPrinterJS *printerObject;
    QTimer *timer;

   // WlanSetting wlan;
    ~MyMainWindow();

//declarations below are all within the scope private until private slots
private:

    /**************************************
    *declare horizontal layout at the top of the window\
    *(or the mainLayout of centralWidget)
    **************************************/
    QHBoxLayout *topLayout,*wifiLayout;
    //declare a button, perform to refresh
    QPushButton *refreshButton;
    //declare a button, perform to go (or jump~.~)
    QPushButton *goButton;
    //declare a editline, used for url
    QLineEdit *urlLineEdit;

    QPushButton *wifiButton;

    QLabel *noLabel;

    //declare a function named setupLayout, used for set all layout.
    void setupLayout();

    //declare webView
    QWebView *webView;

    //daclare mainLayout
    QVBoxLayout *mainLayout;

    //declare a widget which will be embedded in centralWidget
    QWidget *mainWidget;

    //declare a function of initialization
    void setInit();

    //declare a function, responsed of Return
    void pressReturn();

    //declare a function, refresh url line
    void refreshUrl();

    //declare a function, press refresh button
    void pressRefresh();

    //declare a function, press go button
    void pressGo();

    //declare function click links
    void clickLink();

    void pressWifi();

    bool removeCacheDir(const QString &dirName);
    //delare slot functions
private slots:
    void timeoutFunction();
    //declare slot function of load url
    void loadUrl();

    //declare slot funcion, refresh widget urlLineEdit
    void refreshUrlLineEdit();

    //declare slot function, reload current url
    void reloadUrl();

    //declare slot function, load links, pay attention to const
    void loadLink(const QUrl &url);

    void addObjectToJs();

    void wifiSetting();

    void loadFail(bool ok);

};

#endif // MYMAINWINDOW_H
