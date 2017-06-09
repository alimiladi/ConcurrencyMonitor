QT       += core
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = readwritemonitoring
TEMPLATE = app

INCLUDEPATH += src


SOURCES += src/main.cpp\
    src/waitinglogger.cpp \
    src/synchrocontroller.cpp \
    src/omutex.cpp \
    src/osemaphore.cpp \
    src/owaitcondition.cpp \
    src/ohoaremonitor.cpp \
    src/hoaremonitor.cpp \
    src/reader_writer_thread.cpp

HEADERS  +=  \
    src/waitinglogger.h \
    src/synchrocontroller.h \
    src/omutex.h \
    src/osemaphore.h \
    src/owaitcondition.h \
    src/ohoaremonitor.h \
    src/hoaremonitor.h \
    src/abstractreaderwriter.h \
    src/readerwriterequal_sem.h \
    src/readerwriterprioreaders_sem.h \
    src/readerwriterprioreading_sem.h \
    src/readerwriterpriowriter_sem.h \
    src/reader_writer_thread.h \
    src/readerwriterpriowriter_mesa.h \
    src/readerwriterpriowriter_hoare.h
