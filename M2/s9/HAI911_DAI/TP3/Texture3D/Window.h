#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>

#include <QStatusBar>
#include <QWidget>
#include <QGroupBox>

#include "TextureDockWidget.h"

class Window : public QMainWindow
  {
    Q_OBJECT

  public:
    Window();

  private:
        TextureDockWidget *madDockWidget;
        TextureViewer * viewer;
        QStatusBar * statusbar;

    public slots:
        void open3DImage();
  };


#endif // MAINWINDOW_H
