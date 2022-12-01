QT       += core gui sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aft_addquery.cpp \
    aft_modify.cpp \
    bfh_addquery.cpp \
    bfh_modify.cpp \
    main.cpp \
    mainwindow.cpp \
    updateinfo.cpp

HEADERS += \
    aft_addquery.h \
    aft_modify.h \
    bfh_addquery.h \
    bfh_modify.h \
    mainwindow.h \
    signalist.h \
    updateinfo.h

FORMS += \
    aft_addquery.ui \
    aft_modify.ui \
    bfh_addquery.ui \
    bfh_modify.ui \
    mainwindow.ui \
    updateinfo.ui

RC_FILE = appicon.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=

DISTFILES += \
    appicon.rc
