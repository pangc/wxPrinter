#-------------------------------------------------
#
# Project created by QtCreator 2012-09-22T11:33:19
#
#-------------------------------------------------

QT       += core gui

#-----------------------------------------------
#Erorr: undefined refernce to
#'QWebView::QWebView(QWidget *)'
QT += webkit \
     network    \
#-----------------------------------------------

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyBrowser
TEMPLATE = app
LIBS += -lwinspool

SOURCES += main.cpp\
        mymainwindow.cpp \
    ibbdprinterjs.cpp \
    json.cpp \
    ServerAPI.cpp \
    pictureinfo.cpp \
    ibbdprinter.cpp \
    printerthread.cpp \
    ibbdprinterconfig.cpp \
    httpcheck.cpp

HEADERS  += mymainwindow.h \
    ibbdprinterjs.h \
    json.h \
    serverapi.h \
    pictureinfo.h \
    ibbdprinter.h \
    printerthread.h \
    ibbdprinterconfig.h \
    httpcheck.h
