QT += widgets
QT += gui-private

#Minimum Qt 5

TARGET       = QWinTaskBar

TEMPLATE = lib
DEFINES += QWINTASKBAR_LIBRARY

CONFIG += c++17
CONFIG += debug_and_release
CONFIG += build_all

CONFIG(debug, debug|release) {
    TARGET = $$join(TARGET,,,d)
}

VERSION = 0.1.0

DEFINES += APP_VERSION=$${VERSION}

QMAKE_TARGET_COMPANY = "romanmatv"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2023, romanmatv"
QMAKE_TARGET_PRODUCT = "QWinTaskBar"
QMAKE_TARGET_DESCRIPTION = "Windows TaskBar extention for Qt"

## Windows 7 SDK required

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += skip_target_version_ext
DESTDIR = bin

SOURCES += \
    qwintaskbar.cpp

HEADERS += \
    QWinTaskBar_global.h \
    qwintaskbar.h

LIBS += -L"C:/windows/system32" -lole32
LIBS += -lUser32

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
