TEMPLATE = lib
CONFIG += staticlib
CONFIG -= qt

unix {
  LIBS += -lpocketsphinx -lsphinxbase -lsphinxad -lpthread -lm
  INCLUDEPATH += /usr/local/include/sphinxbase
  INCLUDEPATH += /usr/local/include/pocketsphinx

}else:win32 {
  INCLUDEPATH += /local/include
  LIBS += -L/c/tmp/sphinx_wrapper/lib32
  LIBS += -lpocketsphinx -lsphinxbase -lsphinxad -lpthread -lm -liconv
}

SOURCES += \
#    main.cpp \
    sphinxwrapper.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    sphinxwrapper.h

defineTest(copyDir) {
    dirtocopy = $$1

    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$dirtocopy $$OUT_PWD
#    message($$QMAKE_POST_LINK)
    export(QMAKE_POST_LINK)
}

#copy model dir
copyDir($$PWD/model)
