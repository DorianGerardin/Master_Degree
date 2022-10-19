#ifndef TEXTUREDOCKWINDOW_H
#define TEXTUREDOCKWINDOW_H


#include <QButtonGroup>
#include <QCheckBox>
#include <QDockWidget>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QSlider>
#include <QSpacerItem>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QStatusBar>
#include <QWidget>
#include <QFormLayout>
#include <QListWidget>
#include <QScrollArea>
#include <QTabWidget>
#include <QSignalMapper>

#include "TextureViewer.h"


class TextureDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    TextureDockWidget(TextureViewer * _viewer, QWidget * parent );
    TextureViewer *viewer;
    QSlider *xHSlider;
    QSlider *yHSlider;
    QSlider *zHSlider;    QPushButton *invertXPushButton;
    QPushButton *invertYPushButton;
    QPushButton *invertZPushButton;
    QCheckBox *displayXCut;
    QCheckBox *displayYCut;
    QCheckBox *displayZCut;
protected:
    QGroupBox * getCuttingPlaneGroupBox(QWidget * parent);
    QGroupBox * displayImageGroupBox;
    QGridLayout * segIGridLayout;
    QSignalMapper * signalIMapper;
    QVector<QCheckBox *> labelICheckButtons;
    std::vector<unsigned int> indexIToLabel;
    QVector<QLabel *> imageLabels;

public slots:
    void setMaxCutPlanes(int _xMax,int _yMax,int _zMax);
    void setIVisibility(int i);
    void selectIAll();
    void discardIAll();
    void setImageLabels();
    void addImageLabels(const std::map<unsigned char, QColor> & colors);
};

#endif // TEXTUREDOCKWINDOW_H
