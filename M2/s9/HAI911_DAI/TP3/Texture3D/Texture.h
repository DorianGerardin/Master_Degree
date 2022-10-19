#ifndef TEXTURE_H
#define TEXTURE_H

#include "Vec3D.h"

#include <QString>
#include <QColor>
#include <QVector>
#include <QGLViewer/vec.h>
#include <QOpenGLContext>
#include <QOpenGLExtraFunctions>
#include <QGLViewer/camera.h>

class Texture
{
private :

    GLuint textureId;

    unsigned int n[3];
    float d[3];
    unsigned int gridSize;

    float minD;

    double xCut;
    double yCut;
    double zCut;

    double xCutPosition;
    double yCutPosition;
    double zCutPosition;

    int xCutDirection;
    int yCutDirection;
    int zCutDirection;

    double xMax;
    double yMax;
    double zMax;

    bool xCutDisplay;
    bool yCutDisplay;
    bool zCutDisplay;

    bool textureCreated;

public:

    Vec3Di Vmin;
    Vec3Di Vmax;

    Texture(){}
    Texture( QOpenGLContext* context );
    ~Texture();

    void init();
    void initGLSL();

    void clear();

    void initTexture();

    void deleteTexture();

    void draw( const qglviewer::Camera * camera );
    void drawCube();
    void fillCut();
    void drawBoundingBox();
    void drawCutPlanes();

    int getWidth(){return n[0];}
    int getHeight(){return n[1];}
    int getDepth(){return n[2];};
    int getGridSize(){return n[0]*n[2]*n[1];}

    const Vec3Di & getVmin() const { return Vmin; }
    Vec3Di & getVmin() { return Vmin; }
    const Vec3Di & getVmax() const { return Vmax; }
    Vec3Di & getVmax() { return Vmax; }

    float dx(){return d[0];}
    float dy(){return d[1];}
    float dz(){return d[2];}

    void setXCut(int _xCut);
    void setYCut(int _yCut);
    void setZCut(int _zCut);

    void invertXCut(){xCutDirection *= -1;}
    void invertYCut(){yCutDirection *= -1;}
    void invertZCut(){zCutDirection *= -1;}

    void setXCutDisplay(bool _xCutDisplay){xCutDisplay = _xCutDisplay;}
    void setYCutDisplay(bool _yCutDisplay){yCutDisplay = _yCutDisplay;}
    void setZCutDisplay(bool _zCutDisplay){zCutDisplay = _zCutDisplay;}

    float getXMax(){return xMax;}
    float getYMax(){return yMax;}
    float getZMax(){return zMax;}

    GLuint getTextureId(){return textureId;}

    float getGridStep(){return minD;}

    void build(const std::vector<unsigned char> & data, const std::vector<unsigned char> & labesl,
               unsigned int & nx , unsigned int & ny , unsigned int & nz,
               float & dx , float & dy , float & dz,
               std::map<unsigned char, QColor> & colorMap );

    bool printShaderErrors(GLuint shader);
    bool printProgramErrors(int program);
    bool checkOpenGLError();
    std::string readShaderSource(std::string filename);

    GLuint vShader, gShader, fShader, programID;
    QOpenGLContext* glContext;
    QOpenGLExtraFunctions* glFunctions;

    static void /*GLAPIENTRY */MessageCallback( GLenum source, GLenum type,
                                                GLuint id, GLenum severity,
                                                GLsizei length, const GLchar* message,
                                                const void* userParam );

};

#endif // TEXTURE_H
