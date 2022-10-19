# -------------------------------------------------
# Project created by QtCreator 2010-01-27T15:21:45
# -------------------------------------------------
QT += xml
QT += opengl
TARGET = texture3D
TEMPLATE = app
MOC_DIR = ./moc
OBJECTS_DIR = ./obj
DEPENDPATH += ./GLSL
INCLUDEPATH += ./GLSL
SOURCES += Main.cpp \
    Window.cpp \
    TextureViewer.cpp \
    Texture.cpp \
    TextureDockWidget.cpp
HEADERS += Window.h \
    TextureViewer.h \
    Texture.h \
    TextureDockWidget.h \
    Vec3D.h
INCLUDEPATH = ./GLSL
LIBS = -lQGLViewer-qt5 \
    -lglut \
    -lGLU
