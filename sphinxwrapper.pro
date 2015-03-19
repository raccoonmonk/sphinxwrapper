TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

INCLUDEPATH += $$PWD/../libsphinx/include
INCLUDEPATH += $$PWD/../libsphinx/include/sphinxbase
INCLUDEPATH += $$PWD/../libsphinx/include/pocketsphinx

SOURCES += \
    sphinxwrapper.cpp

HEADERS += \
    sphinxwrapper.h
