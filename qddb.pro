TEMPLATE = app
TARGET = qddb
QT += core widgets

INCLUDEPATH += .

HEADERS = main.h qddb.h
SOURCES = main.cpp qddb.cpp

FORMS += main.ui

CONFIG += static

static {

	win32 {

		QMAKE_LFLAGS += -static -static-libgcc
	}
}

QMAKE_CXXFLAGS_RELEASE = -Wall -pedantic -Os
QMAKE_CFLAGS_RELEASE = -Wall -pedantic -Os

