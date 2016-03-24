#-------------------------------------------------
#
# Project created by QtCreator 2013-06-03T18:58:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LetterRecognition
TEMPLATE = app


SOURCES += main.cpp\
        letterrecognition.cpp \
    neuralNetworkTrainer.cpp \
    neuralNetwork.cpp \
    dataReader.cpp

HEADERS  += letterrecognition.h \
    neuralNetworkTrainer.h \
    neuralNetwork.h \
    dataReader.h \
    dataEntry.h
