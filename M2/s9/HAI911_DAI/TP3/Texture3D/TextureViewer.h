#ifndef TEXTUREVIEWER_H
#define TEXTUREVIEWER_H

#include <QKeyEvent>
#include <QGLViewer/qglviewer.h>

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <algorithm>
#include "Texture.h"

class TextureViewer : public QGLViewer
{
    Q_OBJECT

public :
    TextureViewer(QWidget *parent);

    void open3DImage(const QString & fileName);

    void openColors(const QString & fileName);
    void selectIAll();
    void discardIAll();
    void setIVisibility(unsigned int i, bool visibility);
    const std::map<unsigned char, QColor> & getIColorMap()const {return iColorMap;}
    void getImageSubdomainIndices(std::vector<unsigned char> & _subdomain_indices){ _subdomain_indices = subdomain_indices;}

protected :

    Texture texture;

    bool imageLoaded;

    virtual void draw();
    virtual void init();
    virtual QString helpString() const;
    virtual void keyPressEvent(QKeyEvent *e);

    void drawClippingPlane();

    void clear();
    void updateCamera(const qglviewer::Vec & center, float radius);


    void openIMA(  const QString & filename,std::vector<unsigned char> & data, std::vector<unsigned char> & labels,
                   unsigned int & nx , unsigned int & ny , unsigned int & nz, float & dx , float & dy , float & dz );

    Vec3Df cut;
    Vec3Df cutDirection;

    std::map<unsigned char, bool> iDisplayMap;
    std::map<unsigned char, QColor> iColorMap;
    std::vector<unsigned char> subdomain_indices;


public slots:

    void setXCut(int _x);
    void setYCut(int _y);
    void setZCut(int _z);

    void invertXCut();
    void invertYCut();
    void invertZCut();

    void setXCutDisplay(bool _xCutDisplay);
    void setYCutDisplay(bool _yCutDisplay);
    void setZCutDisplay(bool _zCutDisplay);

 signals:
    void setMaxCutPlanes(int _xMax, int _yMax, int _zMax);
    void setImageLabels();

};

#endif
