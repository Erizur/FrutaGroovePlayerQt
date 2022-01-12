QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutinfo.cpp \
    audiothread.cpp \
    main.cpp \
    musicplayer.cpp

HEADERS += \
    aboutinfo.h \
    audiothread.h \
    musicplayer.h

FORMS += \
    aboutinfo.ui \
    musicplayer.ui

TRANSLATIONS += \
    FrutaGroovePlayerQt_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    mainRes.qrc


win32: LIBS += -L$$PWD/bass24/c/x64/ -lbass

INCLUDEPATH += $$PWD/bass24/c/x64
DEPENDPATH += $$PWD/bass24/c/x64
