QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutuswindow.cpp \
    dclass/base/dabstractbase.cpp \
    dclass/base/dlinebase.cpp \
    dclass/base/dpolygonbase.cpp \
    dclass/base/dshapebase.cpp \
    dclass/base/dtextbase.cpp \
    dclass/basicshape/ddiaitem.cpp \
    dclass/basicshape/dellitem.cpp \
    dclass/basicshape/dhexgonitem.cpp \
    dclass/basicshape/dparagramitem.cpp \
    dclass/basicshape/dpentagonitem.cpp \
    dclass/basicshape/drectitem.cpp \
    dclass/basicshape/droundrectitem.cpp \
    dclass/basicshape/dtrapitem.cpp \
    dclass/basicshape/dtriitem.cpp \
    dclass/flowshape/dfcarditem.cpp \
    dclass/flowshape/dfcompareitem.cpp \
    dclass/flowshape/dfconditionitem.cpp \
    dclass/flowshape/dfdataitem.cpp \
    dclass/flowshape/dfdelayitem.cpp \
    dclass/flowshape/dfdirectstorageitem.cpp \
    dclass/flowshape/dfdiskitem.cpp \
    dclass/flowshape/dfdocumentitem.cpp \
    dclass/flowshape/dfinformationitem.cpp \
    dclass/flowshape/dfinternalstoreitem.cpp \
    dclass/flowshape/dfmanualinputitem.cpp \
    dclass/flowshape/dfmanualoperateitem.cpp \
    dclass/flowshape/dfmergeitem.cpp \
    dclass/flowshape/dfmultidocitem.cpp \
    dclass/flowshape/dfnodeitem.cpp \
    dclass/flowshape/dfoffpageitem.cpp \
    dclass/flowshape/dfoptionalprocessitem.cpp \
    dclass/flowshape/dforderstorageitem.cpp \
    dclass/flowshape/dforitem.cpp \
    dclass/flowshape/dfpredefineitem.cpp \
    dclass/flowshape/dfprepareitem.cpp \
    dclass/flowshape/dfprocessitem.cpp \
    dclass/flowshape/dfshowitem.cpp \
    dclass/flowshape/dfsortitem.cpp \
    dclass/flowshape/dfstartenditem.cpp \
    dclass/flowshape/dfstoredataitem.cpp \
    dclass/flowshape/dfsummaryconnitem.cpp \
    dclass/line/dcurvelineitem.cpp \
    dclass/line/dlineitem.cpp \
    dclass/line/dpolylineitem.cpp \
    dclass/text/dtextitem.cpp \
    dfinddialog.cpp \
    dscene.cpp \
    dview.cpp \
    formworkwidget.cpp \
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
    dallitems.h \
    dclass/base/dabstractbase.h \
    dclass/base/dlinebase.h \
    dclass/base/dpolygonbase.h \
    dclass/base/dshapebase.h \
    dclass/base/dtextbase.h \
    dclass/basicshape/ddiaitem.h \
    dclass/basicshape/dellitem.h \
    dclass/basicshape/dhexgonitem.h \
    dclass/basicshape/dparagramitem.h \
    dclass/basicshape/dpentagonitem.h \
    dclass/basicshape/drectitem.h \
    dclass/basicshape/droundrectitem.h \
    dclass/basicshape/dtrapitem.h \
    dclass/basicshape/dtriitem.h \
    dclass/flowshape/dfcarditem.h \
    dclass/flowshape/dfcompareitem.h \
    dclass/flowshape/dfconditionitem.h \
    dclass/flowshape/dfdataitem.h \
    dclass/flowshape/dfdelayitem.h \
    dclass/flowshape/dfdirectstorageitem.h \
    dclass/flowshape/dfdiskitem.h \
    dclass/flowshape/dfdocumentitem.h \
    dclass/flowshape/dfinformationitem.h \
    dclass/flowshape/dfinternalstoreitem.h \
    dclass/flowshape/dfmanualinputitem.h \
    dclass/flowshape/dfmanualoperateitem.h \
    dclass/flowshape/dfmergeitem.h \
    dclass/flowshape/dfmultidocitem.h \
    dclass/flowshape/dfnodeitem.h \
    dclass/flowshape/dfoffpageitem.h \
    dclass/flowshape/dfoptionalprocessitem.h \
    dclass/flowshape/dforderstorageitem.h \
    dclass/flowshape/dforitem.h \
    dclass/flowshape/dfpredefineitem.h \
    dclass/flowshape/dfprepareitem.h \
    dclass/flowshape/dfprocessitem.h \
    dclass/flowshape/dfshowitem.h \
    dclass/flowshape/dfsortitem.h \
    dclass/flowshape/dfstartenditem.h \
    dclass/flowshape/dfstoredataitem.h \
    dclass/flowshape/dfsummaryconnitem.h \
    dclass/line/dcurvelineitem.h \
    dclass/line/dlineitem.h \
    dclass/line/dpolylineitem.h \
    dclass/text/dtextitem.h \
    dfinddialog.h \
    dscene.h \
    dview.h \
    formworkwidget.h \
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
