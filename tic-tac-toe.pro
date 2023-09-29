QT       += core gui
QT       += network
QT       += gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    apihandler.cpp \
    client.cpp \
    clientgamepage.cpp \
    lans.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    player.cpp \
    server.cpp \
    serverbroadsetpage.cpp \
    servergamehandeler.cpp \
    servermessagehandeler.cpp \
    tcpsocketconnection.cpp

HEADERS += \
    apihandler.h \
    client.h \
    clientgamepage.h \
    lans.h \
    mainwindow.h \
    message.h \
    player.h \
    server.h \
    serverbroadsetpage.h \
    servergamehandeler.h \
    servermessagehandeler.h \
    tcpsocketconnection.h

FORMS += \
    client.ui \
    clientgamepage.ui \
    lans.ui \
    mainwindow.ui \
    server.ui \
    serverbroadsetpage.ui \
    servergamehandeler.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qtresource.qrc

DISTFILES += \
    messagetypes.txt
