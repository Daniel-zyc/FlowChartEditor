QT       += core gui svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutuswindow.cpp \
    dabstractbase.cpp \
    ddiaitem.cpp \
    dellitem.cpp \
    denditem.cpp \
    dfinddialog.cpp \
    dfinternalstoreitem.cpp \
    dfmanualoperateitem.cpp \
    ditemgroup.cpp \
    dlinebase.cpp \
    dlineitem.cpp \
    dpolygonbase.cpp \
    dpolylineitem.cpp \
    dpreitem.cpp \
    drectitem.cpp \
    droundrectitem.cpp \
    dscene.cpp \
    dshapebase.cpp \
    dtextitem.cpp \
    dtrapitem.cpp \
    dtriitem.cpp \
    dview.cpp \
    global.cpp \
    magpoint.cpp \
    main.cpp \
    mainwindow.cpp \
    saveandloadmanager.cpp \
    serializer.cpp \
    dparallelogramitem.cpp \
    ddocitem.cpp \
    undomanager.cpp

HEADERS += \
    aboutuswindow.h \
    dabstractbase.h \
    dallitems.h \
    ddiaitem.h \
    dellitem.h \
    denditem.h \
    dfinddialog.h \
    dfinternalstoreitem.h \
    dfmanualoperateitem.h \
    ditemgroup.h \
    dlinebase.h \
    dlineitem.h \
    dpolygonbase.h \
    dpolylineitem.h \
    dpreitem.h \
    drectitem.h \
    droundrectitem.h \
    dscene.h \
    dshapebase.h \
    dtextitem.h \
    dtrapitem.h \
    dtriitem.h \
    dview.h \
    global.h \
    magpoint.h \
    mainwindow.h \
    saveandloadmanager.h \
    serializer.h \
    dparallelogramitem.h \
    ddocitem.h \
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
