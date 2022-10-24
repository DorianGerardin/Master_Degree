// -------------------------------------------
// gMini : a minimal OpenGL/GLUT application
// for 3D graphics.
// Copyright (C) 2006-2008 Tamy Boubekeur
// All rights reserved.
// -------------------------------------------

// -------------------------------------------
// Disclaimer: this code is dirty in the
// meaning that there is no attention paid to
// proper class attribute access, memory
// management or optimisation of any kind. It
// is designed for quick-and-dirty testing
// purpose.
// -------------------------------------------

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <math.h> 

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"
#include "src/jmkdtree.h"


using namespace std;

std::vector< Vec3 > positions;
std::vector< Vec3 > normals;

std::vector< Vec3 > positions2;
std::vector< Vec3 > normals2;

std::vector<Vec3> dualContouringPoints;
std::vector<unsigned int> triangles;

std::vector<Vec3> gridPoints;
std::vector<float> signs;
//std::vector<float> normals;


// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 640;
static unsigned int SCREENHEIGHT = 480;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;

bool display_grid;


// ------------------------------------------------------------------------------------------------------------
// i/o and some stuff
// ------------------------------------------------------------------------------------------------------------
void loadPN (const std::string & filename , std::vector< Vec3 > & o_positions , std::vector< Vec3 > & o_normals ) {
    unsigned int surfelSize = 6;
    FILE * in = fopen (filename.c_str (), "rb");
    if (in == NULL) {
        std::cout << filename << " is not a valid PN file." << std::endl;
        return;
    }
    size_t READ_BUFFER_SIZE = 1000; // for example...
    float * pn = new float[surfelSize*READ_BUFFER_SIZE];
    o_positions.clear ();
    o_normals.clear ();
    while (!feof (in)) {
        unsigned numOfPoints = fread (pn, 4, surfelSize*READ_BUFFER_SIZE, in);
        for (unsigned int i = 0; i < numOfPoints; i += surfelSize) {
            o_positions.push_back (Vec3 (pn[i], pn[i+1], pn[i+2]));
            o_normals.push_back (Vec3 (pn[i+3], pn[i+4], pn[i+5]));
        }

        if (numOfPoints < surfelSize*READ_BUFFER_SIZE) break;
    }
    fclose (in);
    delete [] pn;
}
void savePN (const std::string & filename , std::vector< Vec3 > const & o_positions , std::vector< Vec3 > const & o_normals ) {
    if ( o_positions.size() != o_normals.size() ) {
        std::cout << "The pointset you are trying to save does not contain the same number of points and normals." << std::endl;
        return;
    }
    FILE * outfile = fopen (filename.c_str (), "wb");
    if (outfile == NULL) {
        std::cout << filename << " is not a valid PN file." << std::endl;
        return;
    }
    for(unsigned int pIt = 0 ; pIt < o_positions.size() ; ++pIt) {
        fwrite (&(o_positions[pIt]) , sizeof(float), 3, outfile);
        fwrite (&(o_normals[pIt]) , sizeof(float), 3, outfile);
    }
    fclose (outfile);
}
void scaleAndCenter( std::vector< Vec3 > & io_positions ) {
    Vec3 bboxMin( FLT_MAX , FLT_MAX , FLT_MAX );
    Vec3 bboxMax( FLT_MIN , FLT_MIN , FLT_MIN );
    for(unsigned int pIt = 0 ; pIt < io_positions.size() ; ++pIt) {
        for( unsigned int coord = 0 ; coord < 3 ; ++coord ) {
            bboxMin[coord] = std::min<float>( bboxMin[coord] , io_positions[pIt][coord] );
            bboxMax[coord] = std::max<float>( bboxMax[coord] , io_positions[pIt][coord] );
        }
    }
    Vec3 bboxCenter = (bboxMin + bboxMax) / 2.f;
    float bboxLongestAxis = std::max<float>( bboxMax[0]-bboxMin[0] , std::max<float>( bboxMax[1]-bboxMin[1] , bboxMax[2]-bboxMin[2] ) );
    for(unsigned int pIt = 0 ; pIt < io_positions.size() ; ++pIt) {
        io_positions[pIt] = (io_positions[pIt] - bboxCenter) / bboxLongestAxis;
    }
}

void applyRandomRigidTransformation( std::vector< Vec3 > & io_positions , std::vector< Vec3 > & io_normals ) {
    srand(time(NULL));
    Mat3 R = Mat3::RandRotation();
    Vec3 t = Vec3::Rand(1.f);
    for(unsigned int pIt = 0 ; pIt < io_positions.size() ; ++pIt) {
        io_positions[pIt] = R * io_positions[pIt] + t;
        io_normals[pIt] = R * io_normals[pIt];
    }
}

void subsample( std::vector< Vec3 > & i_positions , std::vector< Vec3 > & i_normals , float minimumAmount = 0.1f , float maximumAmount = 0.2f ) {
    std::vector< Vec3 > newPos , newNormals;
    std::vector< unsigned int > indices(i_positions.size());
    for( unsigned int i = 0 ; i < indices.size() ; ++i ) indices[i] = i;
    srand(time(NULL));
    std::random_shuffle(indices.begin() , indices.end());
    unsigned int newSize = indices.size() * (minimumAmount + (maximumAmount-minimumAmount)*(float)(rand()) / (float)(RAND_MAX));
    newPos.resize( newSize );
    newNormals.resize( newSize );
    for( unsigned int i = 0 ; i < newPos.size() ; ++i ) {
        newPos[i] = i_positions[ indices[i] ];
        newNormals[i] = i_normals[ indices[i] ];
    }
    i_positions = newPos;
    i_normals = newNormals;
}

bool save( const std::string & filename , std::vector< Vec3 > & vertices , std::vector< unsigned int > & triangles ) {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl;

    unsigned int n_vertices = vertices.size() , n_triangles = triangles.size()/3;
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for( unsigned int v = 0 ; v < n_vertices ; ++v ) {
        myfile << vertices[v][0] << " " << vertices[v][1] << " " << vertices[v][2] << std::endl;
    }
    for( unsigned int f = 0 ; f < n_triangles ; ++f ) {
        myfile << 3 << " " << triangles[3*f] << " " << triangles[3*f+1] << " " << triangles[3*f+2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}



// ------------------------------------------------------------------------------------------------------------
// rendering.
// ------------------------------------------------------------------------------------------------------------

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    //glCullFace (GL_BACK);
    //glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);

    display_grid = false;
}

void DrawGrid(std::vector<Vec3> points, long nbPoints) {
    glEnable (GL_POINT_SMOOTH);
    glPointSize(2);
    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < points.size(); ++i) {
        if(signs[i] < 0) {
            glColor3f(1., 0., 0.);
            // glVertex3f(points[i][0],
            //     points[i][1],
            //     points[i][2]);
        }
        else if (signs[i] > 0) glColor3f(0., 0., 1.);
        else glColor3f(0.5, 0.5, 0.5);
        glVertex3f(points[i][0],
                points[i][1],
                points[i][2]);
    }
    glDisable (GL_POINT_SMOOTH);
    glEnd();
}

void drawTriangleMesh( std::vector< Vec3 > const & i_positions , std::vector< unsigned int > const & i_triangles ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_triangles.size() / 3 ; ++tIt) {
        Vec3 p0 = i_positions[i_triangles[3*tIt]];
        Vec3 p1 = i_positions[i_triangles[3*tIt+1]];
        Vec3 p2 = i_positions[i_triangles[3*tIt+2]];
        Vec3 n = Vec3::cross(p1-p0 , p2-p0);
        n.normalize();
        glNormal3f( n[0] , n[1] , n[2] );
        glVertex3f( p0[0] , p0[1] , p0[2] );
        glVertex3f( p1[0] , p1[1] , p1[2] );
        glVertex3f( p2[0] , p2[1] , p2[2] );
    }
    glEnd();
}

void drawPointSet( std::vector< Vec3 > const & i_positions , std::vector< Vec3 > const & i_normals ) {
    glBegin(GL_POINTS);
    for(unsigned int pIt = 0 ; pIt < i_positions.size() ; ++pIt) {
        glNormal3f( i_normals[pIt][0] , i_normals[pIt][1] , i_normals[pIt][2] );
        glVertex3f( i_positions[pIt][0] , i_positions[pIt][1] , i_positions[pIt][2] );
    }
    glEnd();
}

void draw () {
    glPointSize(2); // for example...

    // glColor3f(0.8,0.8,1);
    // drawPointSet(positions , normals);

    // glPointSize(2);
    // glColor3f(1,0.5,0.5);
    // drawPointSet(positions2 , normals2);

    //glPointSize(4);
    glColor3f(0.5,0.5,1.0);
    drawPointSet(dualContouringPoints , normals2);

    drawTriangleMesh(positions2, triangles);

    if(display_grid){
        DrawGrid(gridPoints, gridPoints.size());
    }
}


void display () {
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    draw ();
    glFlush ();
    glutSwapBuffers ();
}

void idle () {
    glutPostRedisplay ();
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if (fullScreen == true) {
            glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }
        break;

    case 'w':
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if(polygonMode[0] != GL_FILL)
            glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        break;

    case 'g': 
        display_grid = !display_grid;
        break;

    default:
        break;
    }
    idle ();
}

void mouse (int button, int state, int x, int y) {
    if (state == GLUT_UP) {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    } else {
        if (button == GLUT_LEFT_BUTTON) {
            camera.beginRotate (x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        } else if (button == GLUT_RIGHT_BUTTON) {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        } else if (button == GLUT_MIDDLE_BUTTON) {
            if (mouseZoomPressed == false) {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle ();
}

void motion (int x, int y) {
    if (mouseRotatePressed == true) {
        camera.rotate (x, y);
    }
    else if (mouseMovePressed == true) {
        camera.move ((x-lastX)/static_cast<float>(SCREENWIDTH), (lastY-y)/static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true) {
        camera.zoom (float (y-lastZoom)/SCREENHEIGHT);
        lastZoom = y;
    }
}


void reshape(int w, int h) {
    camera.resize (w, h);
}

Vec3 project(Vec3 const &input_point, Vec3 const &p_plane, Vec3 const &n_plane)
{
    float d = Vec3::dot(input_point - p_plane, n_plane);
    return (input_point - d * n_plane);
}

void HPSS(Vec3 inputPoint, Vec3 &outputPoint, Vec3 &outputNormal, std::vector<Vec3> const &positions,
          std::vector<Vec3> const &normals, BasicANNkdTree const &kdTree, int kernelType, float h,
          unsigned int nbIterations = 10, unsigned int knn = 20) {

        ANNidxArray id_nearest_neighbors = new ANNidx[ knn ];
        ANNdistArray square_distances_to_neighbors = new ANNdist[ knn ];
        kdTree.knearest(inputPoint, knn, id_nearest_neighbors, square_distances_to_neighbors);
       
        outputPoint = project(inputPoint, positions[id_nearest_neighbors[0]], normals[id_nearest_neighbors[0]]);
        outputNormal = normals[id_nearest_neighbors[0]];   
}

void HPSS2(Vec3 inputPoint, Vec3 &outputPoint, Vec3 &outputNormal, std::vector<Vec3> const &positions,
          std::vector<Vec3> const &normals, BasicANNkdTree const &kdTree, int kernelType, float h,
          unsigned int nbIterations = 10, unsigned int knn = 20) {

        ANNidxArray id_nearest_neighbors = new ANNidx[ knn ];
        ANNdistArray square_distances_to_neighbors = new ANNdist[ knn ];
        kdTree.knearest(inputPoint, knn, id_nearest_neighbors, square_distances_to_neighbors);

        Vec3 p = Vec3(0., 0., 0.);
        Vec3 n = Vec3(0., 0., 0.);
        for (unsigned int i = 0; i < knn; ++i)
        {
            p += positions[id_nearest_neighbors[i]];
            n += normals[id_nearest_neighbors[i]];
        }
        p /= (float) knn;
        n /= (float) knn;
        outputPoint = project(inputPoint, p, n);
        outputNormal = n;
        outputNormal.normalize();
}

float Gaussien(float r, float h) {
    return exp((-(pow(r, 2)))/(pow(h, 2)));
}

float Wendland(float r, float h) {
    return (pow((1 - r/h), 4)) * (1+4*(r/h));
}

float Singulier(float r, float h, int s) {
    return pow((h/r), s);
}

void HPSS3(Vec3 &inputPoint, Vec3 &outputPoint, Vec3 &outputNormal, std::vector<Vec3> const &positions,
          std::vector<Vec3> const &normals, BasicANNkdTree const &kdTree, int kernelType,
          unsigned int nbIterations = 10, unsigned int knn = 20) {

        ANNidxArray id_nearest_neighbors = new ANNidx[ knn ];
        ANNdistArray square_distances_to_neighbors = new ANNdist[ knn ];

        for (unsigned int i = 0; i < nbIterations; ++i)
        {
            kdTree.knearest(inputPoint, knn, id_nearest_neighbors, square_distances_to_neighbors);

            Vec3 p = Vec3(0., 0., 0.);
            Vec3 n = Vec3(0., 0., 0.);
            float r, w, h;
            float wSum = 0.;
            h = square_distances_to_neighbors[knn - 1];
            for (unsigned int k = 0; k < knn; ++k)
            {
                r = sqrt(square_distances_to_neighbors[k]);
                switch(kernelType) {
                    case 0:
                        w = Gaussien(r, h);
                    case 1:
                        w = Wendland(r, h);
                    case 2 :
                        w = Singulier(r, h, 3);
                    default:
                        w = 1.0;
                }
                p += w * positions[id_nearest_neighbors[k]];
                n += w * normals[id_nearest_neighbors[k]];
                wSum += w;
            }
            p /= wSum;
            n /= wSum;
            outputPoint = project(inputPoint, p, n);
            n.normalize();
            outputNormal = n;

        }

        delete [] id_nearest_neighbors;
        delete [] square_distances_to_neighbors;
}


std::vector<Vec3> computeGrid(float scale, unsigned int resolution) {

    std::vector<Vec3> result;
    std::vector<Vec3> cubePoints;
    Vec3 extremBottomPoint, extremTopPoint;
    float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
    float maxX = FLT_MIN, maxY = FLT_MIN, maxZ = FLT_MIN;

    size_t size = positions.size();
    for (size_t i = 0; i < size; ++i)
    {
        if (positions[i][0] < minX) minX = positions[i][0];
        if (positions[i][1] < minY) minY = positions[i][1];
        if (positions[i][2] < minZ) minZ = positions[i][2];
        if (positions[i][0] > maxX) maxX = positions[i][0];
        if (positions[i][1] > maxY) maxY = positions[i][1];
        if (positions[i][2] > maxZ) maxZ = positions[i][2];
    }

    minX -= scale;
    minY -= scale;
    minZ -= scale;

    maxX += scale;
    maxY += scale;
    maxZ += scale;

    extremBottomPoint = Vec3(minX, minY, minZ);
    extremTopPoint = Vec3(maxX, maxY, maxZ);

    float dx = extremTopPoint[0] - extremBottomPoint[0];
    float dy = extremTopPoint[1] - extremBottomPoint[1];
    float dz = extremTopPoint[2] - extremBottomPoint[2];

    for (unsigned int i = 0; i < resolution+1; ++i) {
        for (unsigned int j = 0; j < resolution+1; ++j) {
            for (unsigned int k = 0; k < resolution+1; ++k) {
                Vec3 vecToAdd = Vec3(dx/(resolution)*k, 
                                     dy/(resolution)*j, 
                                     dz/(resolution)*i);
                result.push_back(extremBottomPoint + vecToAdd); 
            }
        }
     } 
    return result;
}

std::vector<float> MLS(std::vector<Vec3> grid, BasicANNkdTree const &kdTree) {

    std::vector<float> mls;
    size_t gridSize = grid.size();
    mls.resize(gridSize);
    for (size_t i = 0; i < gridSize; ++i)
    {
        Vec3 p, n;
        HPSS3(grid[i], p, n, positions, normals, kdTree, 2);
        mls[i] = Vec3::dot((grid[i] - p), n);
    }
    return mls;
}

// std::vector<Vec3> dualContouring(std::vector<Vec3> grid, std::vector<float> signs, unsigned int resolution, std::vector<unsigned int> &triangles) {
    
//     vector<Vec3> points;
//     size_t gridSize = grid.size();
//     vector<bool> alreadyComputed;
//     alreadyComputed.resize(gridSize, false);
//     Vec3 extremBottomPoint = grid[0];
//     Vec3 extremTopPoint = grid[grid.size() - 1];
//     float dx = extremTopPoint[0] - extremBottomPoint[0];
//     cout << "dx : " << dx << endl;
//     float dy = extremTopPoint[1] - extremBottomPoint[1];
//     cout << "dy : " << dy << endl;
//     float dz = extremTopPoint[2] - extremBottomPoint[2];
//     cout << "dz : " << dz << endl;

//     for (unsigned int i = 0; i < resolution-1; ++i) {
//         for (unsigned int j = 0; j < resolution-1; ++j) {
//             for (unsigned int k = 0; k < resolution-1; ++k) {
//                 Vec3 currentVec = grid[i*resolution*resolution+j*resolution+k];
//                 float currentSign = signs[i*resolution*resolution+j*resolution+k];
//                 if(currentSign * signs[i*resolution*resolution+j*resolution+k+1] < 0) {
//                     points.push_back(currentVec + Vec3( dx/(resolution*2), dy/(resolution*2), dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( dx/(resolution*2), dy/(resolution*2), -dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( dx/(resolution*2), -dy/(resolution*2), -dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( dx/(resolution*2), -dy/(resolution*2), dz/(resolution*2) ));

//                     triangles.push_back(points.size()-4);
//                     triangles.push_back(points.size()-3);
//                     triangles.push_back(points.size()-2);

//                     triangles.push_back(points.size()-2);
//                     triangles.push_back(points.size()-1);
//                     triangles.push_back(points.size()-4);
//                 }
//                 if(currentSign * signs[(i+1)*resolution*resolution+j*resolution+k] < 0) {
//                     points.push_back(currentVec + Vec3( dx/(resolution*2), dy/(resolution*2), dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( -dx/(resolution*2), dy/(resolution*2), dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( -dx/(resolution*2), -dy/(resolution*2), dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( dx/(resolution*2), -dy/(resolution*2), dz/(resolution*2) ));

//                     triangles.push_back(points.size()-4);
//                     triangles.push_back(points.size()-3);
//                     triangles.push_back(points.size()-2);

//                     triangles.push_back(points.size()-2);
//                     triangles.push_back(points.size()-1);
//                     triangles.push_back(points.size()-4);
//                 }
//                 if(currentSign * signs[i*resolution*resolution+(j+1)*resolution+k] < 0) {
//                     points.push_back(currentVec + Vec3( dx/(resolution*2), dy/(resolution*2), dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( -dx/(resolution*2), dy/(resolution*2), dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( -dx/(resolution*2), dy/(resolution*2), -dz/(resolution*2) ));
//                     points.push_back(currentVec + Vec3( dx/(resolution*2), dy/(resolution*2), -dz/(resolution*2) ));

//                     triangles.push_back(points.size()-4);
//                     triangles.push_back(points.size()-3);
//                     triangles.push_back(points.size()-2);

//                     triangles.push_back(points.size()-2);
//                     triangles.push_back(points.size()-1);
//                     triangles.push_back(points.size()-4);
//                 }
//             }
//         }
//     }

//     return points;
// }


// -----------------------------------------------------------------------------------------
//                                          CAMILLE CODE
// -----------------------------------------------------------------------------------------


void addFace(int axis, int startPos, int squaredResolution, int resolution){
    if(axis == 0){
        triangles.push_back(startPos);
        triangles.push_back(startPos+resolution);
        triangles.push_back(startPos+resolution+1);
        
        triangles.push_back(startPos);
        triangles.push_back(startPos+resolution+1);
        triangles.push_back(startPos+1);
    }
    if(axis == 1){
        triangles.push_back(startPos);
        triangles.push_back(startPos+squaredResolution);
        triangles.push_back(startPos+squaredResolution+1);
        
        triangles.push_back(startPos);
        triangles.push_back(startPos+squaredResolution+1);
        triangles.push_back(startPos+1);
        
    }
    if(axis == 2){
        triangles.push_back(startPos);
        triangles.push_back(startPos+resolution);
        triangles.push_back(startPos+resolution+squaredResolution);
        
        triangles.push_back(startPos);
        triangles.push_back(startPos+squaredResolution+resolution);
        triangles.push_back(startPos+squaredResolution);

    }
}

void computeBoundingBox(double &minX, double &maxX, double &minY, double &maxY, double &minZ, double &maxZ){
    maxX = -999.;
    minX = 999.;

    maxY = -999.;
    minY = 999.;

    maxZ = -999.;
    minZ = 999.;

    size_t loop = positions.size();
    for(int i = 0 ; i < loop; i++){

        if(positions[i][0] < minX){
            minX = positions[i][0];
        }
        if(positions[i][1] < minY){
            minY = positions[i][1];
        }
        if(positions[i][2] < minZ){
            minZ = positions[i][2];
        }

        if(positions[i][0] > maxX){
            maxX = positions[i][0];
        }
        if(positions[i][1] > maxY){
            maxY = positions[i][1];
        }
        if(positions[i][2] > maxZ){
            maxZ = positions[i][2];
        }
    }
}

void dualContouring(BasicANNkdTree const & kdtree){
    double minX, maxX, minY, maxY, minZ, maxZ = 0.0;
    computeBoundingBox(minX, maxX, minY, maxY, minZ, maxZ);
    minX -= 0.1;
    minY -= 0.1;
    minZ -= 0.1;

    maxX += 0.1;
    maxY += 0.1;
    maxZ += 0.1;
    std::cout<<minX<<" "<<maxX<<std::endl;
    std::cout<<minY<<" "<<maxY<<std::endl;
    std::cout<<minZ<<" "<<maxZ<<std::endl;

    std::vector<Vec3> gridPoints = std::vector<Vec3>();
    
    
    int gridResolution = 128;

    std::vector<std::vector<std::vector<bool>>> insideMesh;
    insideMesh.resize(gridResolution);

    positions2.resize(gridResolution*gridResolution*gridResolution);
    normals2.resize(positions2.size());
    for(int i = 0; i < gridResolution; i++){
        insideMesh[i] = std::vector<std::vector<bool>>();
        insideMesh[i].resize(gridResolution);
        for(int j = 0; j < gridResolution; j++){
            insideMesh[i][j] = std::vector<bool>();
            insideMesh[i][j].resize(gridResolution);
            for(int k = 0; k < gridResolution; k++){

                double x = minX + ( ((double)i * (maxX-minX)) / (double)gridResolution);
                double y = minY + ( ((double)j * (maxY-minY)) / (double)gridResolution);
                double z = minZ + ( ((double)k * (maxZ-minZ)) / (double)gridResolution);

                Vec3 currentGridPoint = Vec3(x, y, z);
                Vec3 outputPoint;
                Vec3 outputNormal;

                HPSS(currentGridPoint, outputPoint, outputNormal, positions, normals, kdtree, 1, 1.0f, 20, 20);
                Vec3 vecDiff = (currentGridPoint - outputPoint);

                double fx = vecDiff[0] * outputNormal[0] + vecDiff[1] * outputNormal[1] + vecDiff[2] * outputNormal[2];

                insideMesh[i][j][k] = (fx <= 0);

                positions2[i*gridResolution*gridResolution+j*gridResolution+k] = currentGridPoint + Vec3(-0.5/(double)gridResolution, -0.5/(double)gridResolution, -0.5/(double)gridResolution);
                normals2[i*gridResolution*gridResolution+j*gridResolution+k] = outputNormal;

            }
        }         
    }

    int squaredResolution = gridResolution * gridResolution;
    
    for(int i = 1; i < gridResolution-1; i++){
        for(int j = 1; j < gridResolution-1; j++){
            for(int k = 1; k < gridResolution-1; k++){

                if(insideMesh[i][j][k] != insideMesh[i-1][j][k]){
                    addFace(0, i*squaredResolution+j*gridResolution+k, squaredResolution, gridResolution);
                }
                if(insideMesh[i][j][k] != insideMesh[i][j-1][k]){
                    addFace(1, i*squaredResolution+j*gridResolution+k, squaredResolution, gridResolution);
                }
                if(insideMesh[i][j][k] != insideMesh[i][j][k-1]){
                    addFace(2, i*squaredResolution+j*gridResolution+k, squaredResolution, gridResolution);
                }
            }
        }
    }

}

int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("tp point processing");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);


    {
        // Load a first pointset, and build a kd-tree:
        loadPN("pointsets/igea_subsampled_extreme.pn" , positions , normals);

        BasicANNkdTree kdtree;
        kdtree.build(positions);

        // Create a second pointset that is artificial, and project it on pointset1 using MLS techniques:
        positions2.resize( positions.size() );
        normals2.resize(positions2.size());
        for( unsigned int pIt = 0 ; pIt < positions2.size() ; ++pIt ) {
            positions2[pIt] = Vec3(
                        -0.6 + 1.2 * (double)(rand())/(double)(RAND_MAX),
                        -0.6 + 1.2 * (double)(rand())/(double)(RAND_MAX),
                        -0.6 + 1.2 * (double)(rand())/(double)(RAND_MAX)
                        );
            positions2[pIt].normalize();
            positions2[pIt] = 0.6 * positions2[pIt];
        }

        // PROJECT USING MLS (HPSS and APSS):
        // TODO
        // for( unsigned int pIt = 0 ; pIt < positions2.size() ; ++pIt ) {
        //     HPSS3(positions2[pIt], positions2[pIt], normals2[pIt], positions, normals, kdtree, 2);
        // }

        // float scale = 0.05;
        // int resolution = 32;
        // gridPoints = computeGrid(scale, resolution);
        // signs = MLS(gridPoints, kdtree);
        // dualContouringPoints = dualContouring(gridPoints, signs, resolution, triangles);
        dualContouring(kdtree);
    }



    glutMainLoop ();
    return EXIT_SUCCESS;
}

