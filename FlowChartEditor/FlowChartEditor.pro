QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutuswindow.cpp \
    dabstractbase.cpp \
    dcurvelineitem.cpp \
    ddiaitem.cpp \
    dellitem.cpp \
    dfcarditem.cpp \
    dfconditionitem.cpp \
    dfdataitem.cpp \
    dfdelayitem.cpp \
    dfdocumentitem.cpp \
    dfinddialog.cpp \
    dfinformationitem.cpp \
    dfinternalstoreitem.cpp \
    dfmanualinputitem.cpp \
    dfmanualoperateitem.cpp \
    dfmergeitem.cpp \
    dforitem.cpp \
    dfpredefineitem.cpp \
    dfnodeitem.cpp \
    dfoptionalprocessitem.cpp \
    dfprepareitem.cpp \
    dfprocessitem.cpp \
    dfstartenditem.cpp \
    dfsummaryconnitem.cpp \
    dlinebase.cpp \
    dlineitem.cpp \
    dmultidocitem.cpp \
    dparagramitem.cpp \
    dpolygonbase.cpp \
    dpolylineitem.cpp \
    drectitem.cpp \
    droundrectitem.cpp \
    dscene.cpp \
    dshapebase.cpp \
    dtextitem.cpp \
    dtrapitem.cpp \
    dtriitem.cpp \
    dview.cpp \
    global.cpp \
    inspector.cpp \
    magpoint.cpp \
    main.cpp \
    mainwindow.cpp \
    saveandloadmanager.cpp \
    serializer.cpp \
    undomanager.cpp

HEADERS += \
    aboutuswindow.h \
    dabstractbase.h \
    dallitems.h \
    dcurvelineitem.h \
    ddiaitem.h \
    dellitem.h \
    dfcarditem.h \
    dfconditionitem.h \
    dfdataitem.h \
    dfdelayitem.h \
    dfdocumentitem.h \
    dfinddialog.h \
    dfinformationitem.h \
    dfinternalstoreitem.h \
    dfmanualinputitem.h \
    dfmanualoperateitem.h \
    dfmergeitem.h \
    dforitem.h \
    dfpredefineitem.h \
    dfnodeitem.h \
    dfoptionalprocessitem.h \
    dfprepareitem.h \
    dfprocessitem.h \
    dfstartenditem.h \
    dfsummaryconnitem.h \
    dlinebase.h \
    dlineitem.h \
    dmultidocitem.h \
    dparagramitem.h \
    dpolygonbase.h \
    dpolylineitem.h \
    drectitem.h \
    droundrectitem.h \
    dscene.h \
    dshapebase.h \
    dtextitem.h \
    dtrapitem.h \
    dtriitem.h \
    dview.h \
    global.h \
    inspector.h \
    magpoint.h \
    mainwindow.h \
    saveandloadmanager.h \
    serializer.h \
    undomanager.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc \
    qss.qrc
