#include "TextureViewer.h"
#include <cfloat>
#include <QFileDialog>
#include <QGLViewer/manipulatedCameraFrame.h>

using namespace std;
using namespace qglviewer;

TextureViewer::TextureViewer(QWidget *parent)
    : QGLViewer(parent){
}


void TextureViewer::draw(){
    // initLights();
    
    drawClippingPlane();
    glEnable(GL_LIGHTING);
    
    qglviewer::Vec cam = camera()->worldCoordinatesOf( qglviewer::Vec(0.,0.,0.) );
    
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDisable(GL_BLEND);
    

    texture.draw( camera() );

    glDisable(GL_LIGHTING);
}

void TextureViewer::drawClippingPlane(){
    
    
    glEnable(GL_LIGHTING);
    
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    

    glDisable(GL_LIGHTING);
    
    GLdouble equation[4];
    glGetClipPlane(GL_CLIP_PLANE0, equation);
    
    qreal p[] = {0.,-equation[3]/equation[1], 0.};
    qreal projP[3];
    camera()->getWorldCoordinatesOf(p, projP);
    
    
    qreal norm[] = {equation[0] + p[0], equation[1]+ p[1], equation[2]+ p[2]};
    qreal normResult[3];
    camera()->getWorldCoordinatesOf(norm, normResult);
    
    Vec3Df normal(normResult[0]-projP[0], normResult[1]-projP[1], normResult[2]-projP[2]);
    Vec3Df point(projP[0], projP[1],projP[2]);
    

}

void TextureViewer::init()
{

    texture = Texture ( QOpenGLContext::currentContext() );
    
    // The ManipulatedFrame will be used as the clipping plane
    setManipulatedFrame(new ManipulatedFrame());
    
    // Enable plane clipping
    glEnable(GL_CLIP_PLANE0);
    
    //Set background color
    setBackgroundColor(QColor(255,255,255));
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    //Set blend parameters
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    imageLoaded = false;
    
    cut = Vec3Df(0.,0.,0.),
    cutDirection = Vec3Df(1.,1.,1.);
    

}

void TextureViewer::clear(){
    
    texture.clear();
    
}


void TextureViewer::updateCamera(const qglviewer::Vec & center, float radius){
    camera()->setSceneCenter(center);
    camera()->setSceneRadius(radius);
    
    camera()->showEntireScene();
}


void TextureViewer::open3DImage(const QString & fileName){
    
    //Texture objet
    texture.clear();
    subdomain_indices.clear();
    std::vector<unsigned char> data;
    unsigned int nx, ny, nz;
    float dx, dy, dz;
    
    
    //Load the data from the 3D image
    if (fileName.endsWith(".dim"))
        openIMA(fileName,data,subdomain_indices, nx,ny,nz,dx,dy,dz);
    else
        return;
    
    for( unsigned int i = 0 ; i < subdomain_indices.size() ; i++ ){
        int currentLabel = subdomain_indices[i];
        
        std::map<unsigned char, QColor>::iterator it = iColorMap.find( currentLabel );
        if( it == iColorMap.end() ){
            if( currentLabel ==0 )
                iColorMap[currentLabel] = QColor(0,0,0);
            else
                iColorMap[currentLabel].setHsvF(0.98*double(i)/subdomain_indices.size(), 0.8,0.8);
        }
        
        iDisplayMap[currentLabel] = true;
    }
    
    texture.build(data,subdomain_indices,nx,ny,nz,dx,dy,dz,iColorMap);
    
    imageLoaded = true;
    
    qglviewer::Vec maxTexture (texture.getXMax(), texture.getYMax() , texture.getZMax());
    
    updateCamera(maxTexture/2. , maxTexture.norm() );
    
    
    
    //Once the 3D image is loaded, grid size parameters are sent to the interface
    emit setMaxCutPlanes(texture.getWidth(), texture.getHeight(), texture.getDepth());
    emit setImageLabels();
}


std::istream & operator>>(std::istream & stream, qglviewer::Vec & v)
{
    stream >>
            v.x >>
            v.y >>
            v.z;
    
    return stream;
}

void TextureViewer::selectIAll(){
    for(std::map<unsigned char, bool>::iterator it = iDisplayMap.begin() ; it != iDisplayMap.end(); ++it )
        iDisplayMap[it->first] = true;
    updateGL();
}

void TextureViewer::discardIAll(){
    for(std::map<unsigned char, bool>::iterator it = iDisplayMap.begin() ; it != iDisplayMap.end(); ++it )
        iDisplayMap[it->first] = false;
    updateGL();
}

void TextureViewer::setIVisibility(unsigned int i, bool visibility){
    if(iDisplayMap.find(i) != iDisplayMap.end())
        iDisplayMap[i] = visibility;
    updateGL();
}


void TextureViewer::openIMA(const QString & fileName, std::vector<unsigned char> & data, std::vector<unsigned char> & labels,
                            unsigned int & nx , unsigned int & ny , unsigned int & nz, float & dx , float & dy , float & dz ){
    QString imaName = QString(fileName);
    
    imaName.replace(".dim", ".ima" );
    std::ifstream imaFile (imaName.toUtf8());
    if (!imaFile.is_open())
        return;
    
    std::ifstream dimFile (fileName.toUtf8());
    if (!dimFile.is_open())
        return;
    
    dimFile >> nx; dimFile >> ny; dimFile >> nz;
    
    string dummy, type;
    
    dimFile >> dummy;
    while (dummy.find("-type")==string::npos)
        dimFile >> dummy;
    
    dimFile >> type;
    
    while (dummy.find("-dx")==string::npos)
        dimFile >> dummy;
    
    dimFile >> dx;
    
    dimFile >> dummy;
    while (dummy.find("-dy")==string::npos)
        dimFile >> dummy;
    
    dimFile >> dy;
    
    dimFile >> dummy;
    while (dummy.find("-dz")==string::npos)
        dimFile >> dummy;
    
    dimFile >> dz;
    
    
    cout << "(nx,dx) = ( " << nx << " ; " << dx << " ) "<< endl;
    cout << "(ny,dy) = ( " << ny << " ; " << dy << " ) "<< endl;
    cout << "(nz,dz) = ( " << nz << " ; " << dz << " ) "<< endl;
    
    unsigned int size = nx*ny*nz;
    unsigned int sizeIn = size;
    
    if( type.find("S16")!=string::npos )
        sizeIn = size*2;
    if( type.find("FLOAT")!=string::npos )
        sizeIn = size*4;
    
    unsigned char * tempData = new unsigned char[sizeIn];
    
    imaFile.read((char*)tempData, sizeIn);
    
    data.clear();
    data.resize(size);
    
    if( type.find("S16")!=string::npos ){
        for(unsigned int i = 0, j=0 ; i < size ; i ++, j+=2){
            unsigned char value = (unsigned char)tempData[j];
            data[i] = value;
            if ( std::find(labels.begin(), labels.end(), value) == labels.end() )
                labels.push_back(value);
        }
    } else if( type.find("FLOAT")!=string::npos ){
        float * floatArray = (float*) tempData;
        
        for(unsigned int i = 0 ; i < size ; i ++){
            unsigned char value = (unsigned char)floatArray[i];
            data[i] = value;
            if ( std::find(labels.begin(), labels.end(), value) == labels.end() )
                labels.push_back(value);
        }
        delete [] floatArray;
    } else {
        for(unsigned int i = 0 ; i < size ; i ++){
            unsigned char value = (unsigned char)tempData[i];
            data[i] = value;
            if ( std::find(labels.begin(), labels.end(), value) == labels.end() )
                labels.push_back(value);
        }
    }
    
    delete [] tempData;
}

void TextureViewer::setXCut(int _x){
    texture.setXCut(_x);
    cut[0] =_x*texture.dx();
    updateGL();
}

void TextureViewer::setYCut(int _y){
    texture.setYCut(_y);
    cut[1] =_y*texture.dy();
    updateGL();
}

void TextureViewer::setZCut(int _z){
    texture.setZCut(_z);
    cut[2] =_z*texture.dz();
    updateGL();
}

void TextureViewer::invertXCut(){
    texture.invertXCut();
    cutDirection[0] *= -1;
    updateGL();
}

void TextureViewer::invertYCut(){
    texture.invertYCut();
    cutDirection[1] *= -1;
    updateGL();
}

void TextureViewer::invertZCut(){
    texture.invertZCut();
    cutDirection[2] *= -1;
    updateGL();
}

void TextureViewer::setXCutDisplay(bool _xCutDisplay){
    texture.setXCutDisplay(_xCutDisplay);
    updateGL();
}

void TextureViewer::setYCutDisplay(bool _yCutDisplay){
    texture.setYCutDisplay(_yCutDisplay);
    updateGL();
}

void TextureViewer::setZCutDisplay(bool _zCutDisplay){
    texture.setZCutDisplay(_zCutDisplay);
    updateGL();
}

void TextureViewer::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_R :updateGL(); break;
    default : QGLViewer::keyPressEvent(e);
}
}

QString TextureViewer::helpString() const
{
    QString text("<h2>S i m p l e V i e w e r</h2>");
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
    text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
    text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
    text += "Press <b>Escape</b> to exit the TextureViewer.";
    return text;
}
