QT       += core gui
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH = /opt/X11/include $$INCLUDEPATH
LIBS += -L/opt/X11/lib -lGLU -lGL -lglut

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bsl_interpriter.cpp \
    cpu.cpp \
    editor.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_2.cpp \
    mainwindow_3.cpp \
    master.cpp \
    player.cpp \
    player_set_ship.cpp \
    result.cpp \
    result_2.cpp \
    result_3.cpp \
    set_ship.cpp \
    start.cpp \
    warning.cpp

HEADERS += \
    bsl_interpriter.h \
    cpu.h \
    define.h \
    editor.h \
    mainwindow.h \
    mainwindow_2.h \
    mainwindow_3.h \
    master.h \
    player.h \
    player_set_ship.h \
    result.h \
    result_2.h \
    result_3.h \
    set_ship.h \
    start.h \
    warning.h
    math.h \

FORMS += \
    editor.ui \
    mainwindow.ui \
    mainwindow_2.ui \
    mainwindow_3.ui \
    result.ui \
    result_2.ui \
    result_3.ui \
    set_ship.ui \
    start.ui \
    warning.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
