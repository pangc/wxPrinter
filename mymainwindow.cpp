#include "mymainwindow.h"
#include <QDebug>
#include "ibbdprinterconfig.h"
MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->printerObject = new IBBDPrinterJS;

    timer = new QTimer;
    timer->start(1000*3600);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutFunction()));

    //change window title
    setWindowTitle("IBBD Printer Browser");

    //initialize window size 800* 600
   // resize(800, 600);

    //call the function to set the layout
    setupLayout();

    //call the function of initialization
    setInit();

    //call the function of press return
    pressReturn();

    //call the funcion, refresh url line
    refreshUrl();

    //call the funcion, press refresh button
    pressRefresh();

    //call the funcion, press go button
    pressGo();

    //call the function, click links
    clickLink();


}

MyMainWindow::~MyMainWindow()
{
    
}

//defination of setupLayout()
void MyMainWindow::setupLayout()
{
    //initialize those pointers below
    topLayout = new QHBoxLayout;
    wifiLayout = new QHBoxLayout;
    noLabel = new QLabel;
    refreshButton = new QPushButton;
    goButton = new QPushButton;
    urlLineEdit = new QLineEdit;
    wifiButton = new QPushButton;
    //add those three Widget, pay attention to the order
    wifiLayout->addWidget(noLabel);
    wifiLayout->addWidget(wifiButton);
    topLayout->addWidget(refreshButton);
    topLayout->addWidget(urlLineEdit);
    topLayout->addWidget(goButton);

    //initialize webView
    webView = new QWebView;

    //initialize mainLayout
    mainLayout = new QVBoxLayout;

    //add topLayout to mainLayout, pay attention to addLayout
  //  mainLayout->addLayout(wifiLayout);
   // mainLayout->addLayout(topLayout);
    //add webView, which is a Widget, to mainLayout, as the second part
    mainLayout->addWidget(webView);

    //initialize mainWidget
    mainWidget = new QWidget;
    //put mainLayout into mainwindow
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    this->showFullScreen();
    this->printerObject->webview = webView;
    connect(webView->page()->mainFrame(),SIGNAL(javaScriptWindowObjectCleared()),this,SLOT(addObjectToJs()));

}

//definition of setInit()
void MyMainWindow::setInit()
{
    QPixmap image("image/wifi.jpg");
    wifiButton->setIcon(QIcon(image));
    QSize size;
    size.setWidth(30);
    size.setHeight(30);
    wifiButton->setMaximumSize(size);
    //Initialize refresh button and set shortcut key as Alt+R
    refreshButton->setText("&Refresh");
    //Initialize go button and set shortcut key as Alt+G
    goButton->setText("&Go");

    //Initialize web context as www.baidu.com
    QString url = IBBDPrinterConfig::config->value("server/serverName","NULL").toString()+"/Index/index";
    QString code = this->printerObject->printer->api->code;
    url = url + "/code/" + code;
    QDateTime time;
    QString timeStr;
    time = QDateTime::currentDateTime();
    //qDebug()<<time.toTime_t();
    timeStr = QString::number(time.toTime_t());
    url = url + "/time/" + timeStr;
    QString token = this->printerObject->printer->api->calToken(code,time.toTime_t());
    url = url + "/token/" +token;
    qDebug()<<url;
    webView->load(QUrl(url));
    //webView->load(QUrl("http://wp.ibbd.net/index.php/Home/Index/index/code/2fd17e15c5ba42206383c7989e3ab3c4/time/1415070779/token/2548f654f27fb921"));

    webView->settings()->setAttribute(QWebSettings::PluginsEnabled,true);
    webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);//JavaScript
    webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);//
    webView->settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);//JavaScript打开网页使能
    //Whenever a link is activated the linkClicked() signal is emitted.
    webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
     //webView->setFixedSize(1920,1080);
    //webView->setMaximumHeight(1080);
    //webView->setMaximumWidth(1920);
}

//definition of press return
void MyMainWindow::pressReturn()
{
    //edit url and press Enter/return to load url
    connect(urlLineEdit, SIGNAL(returnPressed()), this, SLOT(loadUrl()));
}

//definition of slot funcion load url
void MyMainWindow::loadUrl()
{
    //load webpage with url from urlLineEdit
    webView->load(QUrl(urlLineEdit->text()));
}

//definition of refresh current url
void MyMainWindow::refreshUrl()
{
    //webView emit signal loadprogress, and parents response the operation refresh url line
    connect(webView, SIGNAL(loadProgress(int)), this, SLOT(refreshUrlLineEdit()));
}

//definiton of slot funcion refresh url line
void MyMainWindow::refreshUrlLineEdit()
{
    //url line set text, which from widget webView
    urlLineEdit->setText(webView->url().toString());
}

//definition of function, press refresh button
void MyMainWindow::pressRefresh()
{
    //refresh button emit signal clicked, and response reload current url
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(reloadUrl()));
}

//defination of reload url, also refresh
void MyMainWindow::reloadUrl()
{
    //widget webView perform reload
    webView->reload();
}

//definiton of press go button
void MyMainWindow::pressGo()
{
    //go button emit signal clicked, and its parents response operation loadUrl
    connect(goButton, SIGNAL(clicked()), this, SLOT(loadUrl()));
}

//definiton of operation click link
void MyMainWindow::clickLink()
{
    //when webView emit signal linkClicked, MyMainWindow response the slot function loadLink
    connect(webView, SIGNAL(linkClicked(QUrl)), this, SLOT(loadLink(QUrl)));
}

void MyMainWindow::pressWifi()
{
    connect(wifiButton,SIGNAL(clicked()),this,SLOT(wifiSetting()));
}

bool MyMainWindow::removeCacheDir(const QString &dirName)
{
    static QVector<QString> dirNames;
    QDir dir;
    QFileInfoList filst;
    QFileInfoList::iterator curFi;
    //初始化
    dirNames.clear();
    if(dir.exists()){
        dirNames<<dirName;
    }
    else{
        return true;
    }
    //遍历各级文件夹，并将这些文件夹中的文件删除
    for(int i=0;i<dirNames.size();++i){
        dir.setPath(dirNames[i]);
        filst=dir.entryInfoList(QDir::Dirs|QDir::Files
                                |QDir::Readable|QDir::Writable
                                |QDir::Hidden|QDir::NoDotAndDotDot
                                ,QDir::Name);
        if(filst.size()>0){
            curFi=filst.begin();
            while(curFi!=filst.end()){
                 //遇到文件夹,则添加至文件夹列表dirs尾部
                if(curFi->isDir()){
                    dirNames.push_back(curFi->filePath());
                }else if(curFi->isFile()){
                    //遇到文件,则删除之
                    if(!dir.remove(curFi->fileName())){
                        return false;
                    }
                }
                curFi++;
            }//end of while
        }
    }
    //删除文件夹
    for(int i=dirNames.size()-1;i>=0;--i){
        dir.setPath(dirNames[i]);
        if(!dir.rmdir(".")){
            return false;
        }
    }
    return true;
}

void MyMainWindow::timeoutFunction()
{
    timer->stop();
    qDebug()<<"test";
    QFileInfo fileInfo("verifyFile");
    if(fileInfo.exists())
    {
        int t =  QDateTime::currentDateTime().toTime_t() - fileInfo.lastModified().toTime_t();
        if(t > 3600*24*7)
        {
            removeCacheDir("cache");
            QFile file("verifyFile");
            file.open(QFile::ReadOnly |QFile::Text);
            QString str = file.readAll();
            file.close();
            file.open(QFile::WriteOnly |QFile::Text);
            file.write(str.toAscii());
            file.close();
        }
    }
    QString printerName = this->printerObject->printer->printer->printerName();
    HANDLE   PrnHandle=0;
    LPWSTR lpw =(LPTSTR)(printerName.utf16());
    if(OpenPrinter(lpw,&PrnHandle,NULL))
    {
        PRINTER_INFO_2*   pInfo;
        DWORD   dwSize;
        unsigned    char   buf[8192];
        pInfo   =   (PRINTER_INFO_2*)buf;
        GetPrinter(PrnHandle,2,buf,sizeof(buf),&dwSize);
        int currJobNum = pInfo->cJobs;
        if(pInfo->cJobs==0)
        {
            QString url = IBBDPrinterConfig::config->value("server/serverName","NULL").toString()+"/Index/index";
            QString code = this->printerObject->printer->api->code;
            url = url + "/code/" + code;
            QDateTime time;
            QString timeStr;
            time = QDateTime::currentDateTime();
            //qDebug()<<time.toTime_t();
            timeStr = QString::number(time.toTime_t());
            url = url + "/time/" + timeStr;
            QString token = this->printerObject->printer->api->calToken(code,time.toTime_t());
            url = url + "/token/" +token;
            qDebug()<<url;
            webView->load(QUrl(url));
        }
    }

    timer->start(1000*3600);
}

//definiton of function load link
void MyMainWindow::loadLink(const QUrl &url)
{
    //load url of links
    webView->load(url);
}

void MyMainWindow::addObjectToJs()
{
    qDebug()<<"addjs";
    int verifyCode = this->printerObject->printer->verifyRegister();
    if(verifyCode == VERIFY_ERROR)
    {
        this->close();
    }
    this->webView->page()->mainFrame()->addToJavaScriptWindowObject("IBBDPrinter",this->printerObject);
}

void MyMainWindow::wifiSetting()
{
    qDebug()<< "wifi setting";
    QNetworkConfigurationManager mgr;
    QList<QNetworkConfiguration> activeConfigs = mgr.allConfigurations(QNetworkConfiguration::Active);
}

void MyMainWindow::loadFail(bool ok)
{
    if(!ok)
    {
      webView->reload();
    }
}




