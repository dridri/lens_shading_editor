TEMPLATE	= app
TARGET		= lensshading

CONFIG		+= warn_off
QT			+= widgets opengl

FORMS		+= MainWindow.ui OpenDialog.ui

HEADERS		= MainWindow.h \
			  GLWidget.h

SOURCES		= MainWindow.cpp \
			  Translator.cpp \
			  GLWidget.cpp \
			  main.cpp
