TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

unix {
  LIBS += -lpocketsphinx -lsphinxbase -lsphinxad -lpthread -lm
  INCLUDEPATH += /usr/local/include/sphinxbase
  INCLUDEPATH += /usr/local/include/pocketsphinx

}else:win32 {
  #TODO add local pre-built libs and include headers
  
  INCLUDEPATH += $$PWD/libwin32sphinx08stable/include
  INCLUDEPATH += $$PWD/libwin32sphinx08stable/include/sphinxbase
  INCLUDEPATH += $$PWD/libwin32sphinx08stable/include/pocketsphinx
  LIBS += -L$$PWD/libwin32sphinx08stable
  
  LIBS += -lpocketsphinx -lsphinxbase -lsphinxad -lpthread -lm -liconv
}

SOURCES += \
    sphinxwrapper.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    sphinxwrapper.h

defineTest(copyDir) {
    dirtocopy = $$1

    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$dirtocopy $$OUT_PWD
    export(QMAKE_POST_LINK)
}

#copy model dir
copyDir($$PWD/model)
