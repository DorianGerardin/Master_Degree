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
#include <string>
#include <cstdio>
#include <cstdlib>
#include <climits>
#include <float.h>

#include <algorithm>
#include <GL/glut.h>
#include "src/Vec3.h"
#include "src/Camera.h"
#include "src/Mesh.h"
#include "src/linearSystem.h"
#include "src/LaplacianWeights.h"
#include "extern/eigen3/Eigen/SVD"
#include "extern/eigen3/Eigen/Geometry"
#include "extern/freeglut/freeglut.h"



using namespace std;
#define GLUT_KEY_ENTER 13
#define GLUT_KEY_ESCAPE 27

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
static unsigned int FPS = 0;
static bool fullScreen = false;
static float defaultRadius = 0.01;
static float radius = defaultRadius;
static unsigned int lastVertexClicked = -1;
static bool hasToDrawShpereBrush = false;

enum ViewerState {
    ViewerState_NORMAL ,
    ViewerState_EDITINGHANDLE ,
    ViewerState_TRANSLATINGHANDLE ,
    ViewerState_ROTATINGHANDLE
};
ViewerState viewerState;

#include "src/RectangleSelectionTool.h"
RectangleSelectionTool rectangleSelectionTool;


// -------------------------------------------
// ARAP variables
// -------------------------------------------

Mesh mesh;
LaplacianWeights edgeAndVertexWeights;
linearSystem arapLinearSystem;
std::vector< Eigen::MatrixXd > vertexRotationMatrices;

int numberOfHandles = 0;
int activeHandle = 0;
bool handlesWereChanged = false; // if they are changed, we need to update the system for ARAP
std::vector< bool > verticesAreMarkedForCurrentHandle;
std::vector< int > verticesHandles;
double spheresSize = 0.01;









//------------------------------------------------------------------------------------------------------//
//---------------------------------  EXAMPLE OF USE OF A LINEAR SYSTEM  --------------------------------//
//------------------------------------------------------------------------------------------------------//
void testlinearSystem() {
    // You can get inspiration from this piece of code :
    {
        linearSystem mySystem;
        mySystem.setDimensions(4 , 3);

        mySystem.A(0,0) = 1.0;  mySystem.A(0,1) = 2.0;  mySystem.A(0,2) = 4.0;
        mySystem.A(1,0) = 1.0;  mySystem.A(1,2) = 1.0;
        mySystem.A(2,0) = -1.0;  mySystem.A(2,1) = 1.0;
        mySystem.A(3,1) = -1.0;  mySystem.A(3,2) = 4.0;
        // the values that are not set with mySystem.A(row,column) = value, are set to 0 by default.
        mySystem.b(0) = 5.0;
        mySystem.b(1) = 6.0;
        mySystem.b(2) = 2.0;
        mySystem.b(3) = 4.0;

        mySystem.preprocess();
        Eigen::VectorXd X;
        mySystem.solve(X);

        std::cout << X[0] << "  " << X[1] << "  " << X[2] << std::endl;
    }
}
//------------------------------------------------------------------------------------------------------//
//---------------------------------  EXAMPLE OF USE OF A LINEAR SYSTEM  --------------------------------//
//------------------------------------------------------------------------------------------------------//







Eigen::MatrixXd getClosestRotation( Eigen::MatrixXd const & m ) {
    Eigen::JacobiSVD< Eigen::MatrixXd > svdStruct = m.jacobiSvd( Eigen::ComputeFullU | Eigen::ComputeFullV );
    return svdStruct.matrixU() * svdStruct.matrixV().transpose();
}









//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//---------------------------------  CODE TO CHANGE  --------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
void updateSystem() {
    if(! handlesWereChanged) return;

    // TODO:
    // set the right values for the number or rows and number of columns
    // remember: number of colums = nb of variables
    // remember: number of rows = nb of equations

    unsigned int ncolumns = mesh.V.size() * 3;

    unsigned int nrows = 0;

    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        unsigned int numberOfNeighbors = edgeAndVertexWeights.get_n_adjacent_edges(v);
        nrows += (numberOfNeighbors * 3);   // WHAT TO PUT HERE ??????? How to update the number of rows ?
    }
    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        if(verticesHandles[v] != -1) {
            nrows += 3;  // WHAT TO PUT HERE ??????? How to update the number of rows ?
        }
    }

    // Once the number of rows and columns have been found, we can allocate the matrices:
    arapLinearSystem.setDimensions( nrows , ncolumns );

    // TODO:
    // set the right values for the matrix A in the linear system

    unsigned int equationIndex = 0;
    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        for( std::map< unsigned int , double >::const_iterator it = edgeAndVertexWeights.get_weight_of_adjacent_edges_it_begin(v) ;
             it != edgeAndVertexWeights.get_weight_of_adjacent_edges_it_end(v) ; ++it) {

            unsigned int vNeighbor = it->first;

            // WHAT TO PUT HERE ??????? How to update the entries of A ?
            arapLinearSystem.A(equationIndex, v*3) = -1.0;
            arapLinearSystem.A(equationIndex, vNeighbor*3) = 1.0;

            arapLinearSystem.A(equationIndex+1, v*3+1) = -1.0;
            arapLinearSystem.A(equationIndex+1, vNeighbor*3+1) = 1.0;

            arapLinearSystem.A(equationIndex+2, v*3+2) = -1.0;
            arapLinearSystem.A(equationIndex+2, vNeighbor*3+2) = 1.0;

            equationIndex += 3;

        }
    }
    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        if(verticesHandles[v] != -1) {

            // WHAT TO PUT HERE ??????? How to update the entries of A ?
            arapLinearSystem.A(equationIndex, v*3) = 1.0;
            arapLinearSystem.A(equationIndex+1, v*3+1) = 1.0;
            arapLinearSystem.A(equationIndex+2, v*3+2) = 1.0;
            equationIndex += 3;
        }
    }

    arapLinearSystem.preprocess();
    handlesWereChanged = false;
}


void updateMeshVertexPositionsFromARAPSolver() {
    //return; // TODO : COMMENT THIS LINE WHEN YOU START THE EXERCISE  (setup of the matrix A for the linear system A.X=B)
    updateSystem();

    unsigned int maxIterationsForArap = 5;


    //return; // TODO : COMMENT THIS LINE WHEN YOU CONTINUE THE EXERCISE  (setup of the vector B for the linear system A.X=B)
    // set the right values for the vector b in the linear system, solve the linear system and update the positions using the solution.


    for( unsigned int arapIteration = 0 ; arapIteration < maxIterationsForArap ; ++arapIteration ) {
        // 1 FIRST : SOLVE THE LINEAR SYSTEM TO UPDATE THE POSITIONS, GIVEN THE EXISTING ROTATION MATRICES
        unsigned int equationIndex = 0;
        for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
            for( std::map< unsigned int , double >::const_iterator it = edgeAndVertexWeights.get_weight_of_adjacent_edges_it_begin(v) ;
                 it != edgeAndVertexWeights.get_weight_of_adjacent_edges_it_end(v) ; ++it) {
                unsigned int vNeighbor = it->first;
                Eigen::VectorXd rotatedEdge(3);
                for( unsigned int coord = 0 ; coord < 3 ; ++coord )
                    rotatedEdge[coord] = mesh.V[vNeighbor].pInit[coord]  -  mesh.V[v].pInit[coord];
                rotatedEdge = vertexRotationMatrices[v] * rotatedEdge;

                // WHAT TO PUT HERE ??????? How to update the entries of b ?
                arapLinearSystem.b(equationIndex) = rotatedEdge.x();
                arapLinearSystem.b(equationIndex+1) = rotatedEdge.y();
                arapLinearSystem.b(equationIndex+2) = rotatedEdge.z();

                equationIndex += 3;

            }
        }
        for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
            if(verticesHandles[v] != -1) {

                // WHAT TO PUT HERE ??????? How to update the entries of b ?
                arapLinearSystem.b(equationIndex) = mesh.V[v].p[0];
                arapLinearSystem.b(equationIndex+1) = mesh.V[v].p[1];
                arapLinearSystem.b(equationIndex+2) = mesh.V[v].p[2];

                equationIndex += 3;
            }
        }

        // Once the matrix A and the vector B are correctly set, we obtain the position of the vertices by solving for A.X = B
        Eigen::VectorXd X_newPositions;
        arapLinearSystem.solve(X_newPositions);
        for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
            if(verticesHandles[v] == -1) {
                for( unsigned int coord = 0 ; coord < 3 ; ++coord )
                    mesh.V[v].p[coord] = X_newPositions[3*v + coord];
            }
        }



        //return; // TODO : COMMENT THIS LINE WHEN YOU CONTINUE THE EXERCISE (update of the rotation matrices -- auxiliary variables)



        // 2 SECOND : UPDATE THE ROTATION MATRICES
        for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
            Eigen::MatrixXd tensorMatrix = Eigen::MatrixXd::Zero(3,3);
            for( std::map< unsigned int , double >::const_iterator it = edgeAndVertexWeights.get_weight_of_adjacent_edges_it_begin(v) ;
                 it != edgeAndVertexWeights.get_weight_of_adjacent_edges_it_end(v) ; ++it) {
                unsigned int vNeighbor = it->first;
                Eigen::VectorXd initialEdge(3);
                Eigen::VectorXd rotatedEdge(3);
                for( unsigned int coord = 0 ; coord < 3 ; ++coord ) {
                    initialEdge[coord] = mesh.V[vNeighbor].pInit[coord]  -  mesh.V[v].pInit[coord];
                    rotatedEdge[coord] = mesh.V[vNeighbor].p[coord]  -  mesh.V[v].p[coord];
                }

                // WHAT TO PUT HERE ??????? How to update the entries of the tensor matrix ?
                tensorMatrix += rotatedEdge * initialEdge.transpose();

            }
            vertexRotationMatrices[v] = getClosestRotation( tensorMatrix );
        }
    }
}
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//---------------------------------  CODE TO CHANGE  --------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------//
























void translateActiveHandle( Vec3 const & translationVector ) {
    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        if( verticesHandles[v] == activeHandle ) {
            mesh.V[v].p += translationVector;
        }
    }

    updateMeshVertexPositionsFromARAPSolver();
}

void rotateActiveHandle( Vec3 const & rotationAxis , double angle ) {
    Eigen::Vector3d centerOfRotation(0,0,0);
    double sumWeights = 0.0;
    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        if( verticesHandles[v] == activeHandle ) {
            centerOfRotation += Eigen::Vector3d(mesh.V[v].p[0] , mesh.V[v].p[1] , mesh.V[v].p[2]);
            sumWeights += 1.0;
        }
    }
    centerOfRotation /= sumWeights;

    Eigen::Vector3d axisEigenType( rotationAxis[0] , rotationAxis[1] , rotationAxis[2] );
    Eigen::Matrix3d rotation;
    rotation = Eigen::AngleAxisd(angle, axisEigenType);

    // Apply rotation and translation, such that the center of mass is preserved: R * c + t = c    =>    t = c - R * c;
    Eigen::Vector3d translation = centerOfRotation - rotation * centerOfRotation;

    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        if( verticesHandles[v] == activeHandle ) {
            Eigen::Vector3d newPos = rotation * Eigen::Vector3d(mesh.V[v].p[0] , mesh.V[v].p[1] , mesh.V[v].p[2])  +  translation;
            mesh.V[v].p = Vec3(newPos[0] , newPos[1] , newPos[2]);
        }
    }

    updateMeshVertexPositionsFromARAPSolver();
}








//---------------------------------   YOU DO NOT NEED TO CHANGE THE FOLLOWING CODE  --------------------------------//
void glVertex(Vec3 const & p) {
    glVertex3f(p[0] , p[1] , p[2]);
}
void glNormal(Vec3 const & p) {
    glNormal3f(p[0] , p[1] , p[2]);
}

bool activeHandleIsValid() {
    return activeHandle >= 0  &&  activeHandle < numberOfHandles;
}



/*
from your modelview:
|lx,ux,vx,px|     -- lx,ly,lz is your left-vector
|ly,uy,vy,py|     -- ux,uy,uz is your up-vector
|lz,uz,vz,pz|     -- vx,vy,vz is your view-vector
|0 ,0 ,0 ,1 |     -- px,py,pz is your translation
*/
Vec3 getRightVector() {
    float modelview[16];  glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    return Vec3( modelview[0] , modelview[4] , modelview[8] );
}
Vec3 getUpVector() {
    float modelview[16];  glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    return Vec3( modelview[1] , modelview[5] , modelview[9] );
}
Vec3 getViewVector() {
    float modelview[16];  glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    return Vec3( modelview[2] , modelview[6] , modelview[10] );
}



void setTagForVerticesInRectangle( bool tagToSet ) {
    float modelview[16];  glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    float projection[16]; glGetFloatv(GL_PROJECTION_MATRIX , projection);

    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        Vec3 const & p = mesh.V[ v ].p;

        float x = modelview[0] * p[0] + modelview[4] * p[1] + modelview[8] * p[2] + modelview[12];
        float y = modelview[1] * p[0] + modelview[5] * p[1] + modelview[9] * p[2] + modelview[13];
        float z = modelview[2] * p[0] + modelview[6] * p[1] + modelview[10] * p[2] + modelview[14];
        float w = modelview[3] * p[0] + modelview[7] * p[1] + modelview[11] * p[2] + modelview[15];
        x /= w; y /= w; z /= w; w = 1.f;

        float xx = projection[0] * x + projection[4] * y + projection[8] * z + projection[12] * w;
        float yy = projection[1] * x + projection[5] * y + projection[9] * z + projection[13] * w;
        float ww = projection[3] * x + projection[7] * y + projection[11] * z + projection[15] * w;
        xx /= ww; yy /= ww;

        xx = ( xx + 1.f ) / 2.f;
        yy = ( yy + 1.f ) / 2.f;

        if( rectangleSelectionTool.contains( xx , yy ) ) verticesAreMarkedForCurrentHandle[ v ] = tagToSet;
    }
}

void addVerticesToCurrentHandle() {
    // look at the rectangle rectangleSelectionTool, and see which vertices fall into the region.
    if( activeHandle < 0 || activeHandle >= numberOfHandles)
        return;

    setTagForVerticesInRectangle( rectangleSelectionTool.isAdding );
}

void finalizeEditingOfCurrentHandle() {
    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        Vec3 const & p = mesh.V[ v ].p;
        if(verticesAreMarkedForCurrentHandle[ v ]) {
            verticesHandles[v] = activeHandle;
            verticesAreMarkedForCurrentHandle[ v ] = false; // prepare next selection
        }
    }

    handlesWereChanged = true;
}


unsigned int getVertexFromClick(GLdouble mouseX, GLdouble mouseY) {
    Vec3 clickPosition = Vec3(0., 0., 0.);
    GLdouble modelView[16];  glGetDoublev(GL_MODELVIEW_MATRIX , modelView);
    GLdouble projection[16]; glGetDoublev(GL_PROJECTION_MATRIX , projection);
    GLint view[4]; glGetIntegerv(GL_VIEWPORT, view);

    float depth;
    glReadPixels(mouseX, view[3]-mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    gluUnProject(mouseX, view[3]-mouseY, depth, modelView, projection, view, &clickPosition[0], &clickPosition[1], &clickPosition[2]);
    
    double minDistance = DBL_MAX;
    unsigned int vertexIndex = 0;
    size_t nbVertices = mesh.V.size();
    for (unsigned int i = 0; i < nbVertices; ++i)
    {
        Vec3 vecDiff = mesh.V[i].p - clickPosition;
        double distance = vecDiff.length();
        if(distance < minDistance) {
            minDistance = distance;
            vertexIndex = i;
        }
    }
    return vertexIndex;
}

void collectOneRing (vector<MeshVertex> const & vertices,
                       vector<MeshTriangle> const & triangles,
                       vector<vector<unsigned int> > & oneRing) {
    //one-ring of each vertex, i.e. a list of vertices with which it shares an edge
    //Initialiser le vecetur de o_one_ring de la taille du vecteur vertices
    oneRing.clear();
    oneRing.resize(vertices.size());
    //Parcourir les triangles et ajouter les voisins dans le 1-voisinage
    for (unsigned int i = 0; i < triangles.size(); ++i) {
        for (unsigned int j = 0; j < 3; ++j) { //sommet courant
            for (unsigned int k = 0; k < 3; ++k) { //sommets voisins
                if(j != k) {
                    if(std::find(oneRing[triangles[i][j]].begin(), oneRing[triangles[i][j]].end(), triangles[i][k]) == oneRing[triangles[i][j]].end()) {
                        oneRing[triangles[i][j]].push_back(triangles[i][k]);
                    }
                }
            }
        }
    }
}

unsigned int getRightPositionToAddInQueue(vector<pair<unsigned int, float>> queue, float distance) {
    size_t queueSize = queue.size();
    unsigned int index = 0;
    for (int i = 0; i < queueSize; ++i) {
        if(distance <= queue[i].second) {
            index++;
        } else break;
    } return index;
}

void handleNeighborsInTreshold(float treshold) {
    vector<pair<unsigned int, float>> queue;
    queue.insert(queue.begin(), make_pair(lastVertexClicked, 0.));
    vector<bool> alreadyVisited;
    size_t nbVertices = mesh.V.size();
    alreadyVisited.resize(nbVertices, false);

    vector<vector<unsigned int>> oneRing;
    collectOneRing(mesh.V, mesh.T, oneRing);
    while(queue.size() != 0) {
        pair<unsigned int, float> current = queue[queue.size() - 1];
        queue.pop_back();
        float cumulDistance = current.second;
        for (unsigned int i = 0; i < oneRing[current.first].size(); ++i) {
            unsigned int neighbor = oneRing[current.first][i];
            cumulDistance += (mesh.V[neighbor] - mesh.V[current.first]).length();
            if(!alreadyVisited[neighbor] && cumulDistance <= treshold) {
                unsigned int queueIndex = getRightPositionToAddInQueue(queue, cumulDistance);
                queue.insert(queue.begin() + queueIndex, make_pair(neighbor, cumulDistance));
                alreadyVisited.insert(alreadyVisited.begin() + current.first, true);
            }
        }
    }
    for (int i = 0; i < nbVertices; ++i)
    {
        verticesAreMarkedForCurrentHandle[i] = alreadyVisited[i];
    }
}

void setNeighborsInRadiusFromVertex() {
    handleNeighborsInTreshold(radius);
}


void printUsage () {
    cerr << endl
         << "Usage : ./gmini [<file.off>]" << endl
         << "Keyboard commands" << endl
         << "------------------" << endl
         << " ?: Print help" << endl
         << " w: Toggle Wireframe Mode" << endl
         << " f: Toggle full screen mode" << endl
         << " <drag>+<left button>: rotate model" << endl
         << " <drag>+<right button>: move model" << endl
         << " <drag>+<middle button>: zoom" << endl << endl;
}

void usage () {
    printUsage ();
    exit (EXIT_FAILURE);
}



// ------------------------------------

void initLight () {
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f,-16.0f,-50.0f};
    GLfloat color1[4] = {1, 1,1, 1};
    GLfloat ambient[4] = {1, 1, 1, 1};

    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv (GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
}

void init () {
    viewerState = ViewerState_NORMAL;
    camera.resize (SCREENWIDTH, SCREENHEIGHT);
    initLight ();
    glCullFace (GL_BACK);
    glEnable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor (0.9f, 0.9f, 0.9f, 1.0f);

    testlinearSystem();
}



void calc_RGB( float val , float val_min , float val_max , float & r , float & g , float & b ) {
    // define uniform color intervalls [v0,v1,v2,v3,v4]
    float v0 = val_min ,
            v1 = val_min + 1.0/4.0 * (val_max - val_min),
            v2 = val_min + 2.0/4.0 * (val_max - val_min),
            v3 = val_min + 3.0/4.0 * (val_max - val_min),
            v4 = val_max ;

    if (val < v0) {
        r = 0.f;  g = 0.f;  b = 1.f; return;
    }
    else if (val > v4) {
        r = 1.f;  g = 0.f;  b = 0.f; return;
    }
    else if (val <= v2) {
        if (val <= v1) { // [v0, v1]
            r = 0.f;  g = (val - v0) / (v1 - v0);  b = 1.f; return;
        }
        else { // ]v1, v2]
            r = 0.f;   g = 1.f;  b = 1.f - (val - v1) / (v2 - v1); return;
        }
    }
    else {
        if (val <= v3) { // ]v2, v3]
            r = (val - v2) / (v3 - v2);  g = 1.f;  b = 0.f; return;
        }
        else { // ]v3, v4]
            r = 1.f;  g = 1.f - (val - v3) / (v4 - v3);  b = 0.f; return;
        }
    }
}
void drawSphere(float x,float y,float z,float radius,int slices,int stacks)
{
    if(stacks < 2){stacks = 2;}
    if(stacks > 30){stacks = 30;}
    if(slices < 3){slices = 3;}
    if(slices > 30){slices = 30;}
    //Pas essentiel ...

    int Nb = slices*stacks +2;
    std::vector< Vec3 > points(Nb);

    Vec3 centre(x,y,z);

    float sinP , cosP , sinT , cosT , Phi , Theta;
    points[0] = Vec3(0,0,1);
    points[Nb-1] = Vec3(0,0,-1);

    for(int i=1; i<=stacks; i++)
    {
        Phi = 90 - (float)(i*180)/(float)(stacks+1);
        sinP = sinf(Phi*3.14159265/180);
        cosP = cosf(Phi*3.14159265/180);

        for(int j=1; j<=slices; j++)
        {
            Theta = (float)(j*360)/(float)(slices);
            sinT = sinf(Theta*3.14159265/180);
            cosT = cosf(Theta*3.14159265/180);

            points[ j + (i-1)*slices ] = Vec3(cosT*cosP,sinT*cosP,sinP);
        }
    }

    int k1,k2;
    glBegin(GL_TRIANGLES);
    for(int i=1; i<=slices; i++)
    {
        k1 = i;
        k2 = (i%slices+1);
        glNormal(points[0]);
        glVertex((centre + radius*points[0]));
        glNormal(points[k1]);
        glVertex((centre + radius*points[k1]));
        glNormal(points[k2]);
        glVertex((centre + radius*points[k2]));

        k1 = (stacks-1)*slices+i;
        k2 = (stacks-1)*slices+(i%slices+1);
        glNormal(points[k1]);
        glVertex((centre + radius*points[k1]));
        glNormal(points[Nb-1]);
        glVertex((centre + radius*points[Nb-1]));
        glNormal(points[k2]);
        glVertex((centre + radius*points[k2]));
    }
    glEnd();

    glBegin(GL_QUADS);
    for(int j=1; j<stacks; j++)
    {
        for(int i=1; i<=slices; i++)
        {
            k1 = i + (j-1)*slices;
            k2 = (i%slices+1) + (j-1)*slices;
            glNormal(points[k2]);
            glVertex((centre + radius*points[k2]));
            glNormal(points[k1]);
            glVertex((centre + radius*points[k1]));

            k1 = i + (j)*slices;
            k2 = (i%slices+1) + (j)*slices;
            glNormal(points[k1]);
            glVertex((centre + radius*points[k1]));
            glNormal(points[k2]);
            glVertex((centre + radius*points[k2]));
        }
    }
    glEnd();
}

void drawHandles() {
    glEnable(GL_LIGHTING);
    glColor3f(0.2,0.2,0.2);
    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        Vec3 const & p = mesh.V[ v ].p;
        if(verticesAreMarkedForCurrentHandle[ v ])
            drawSphere( p[0] , p[1] , p[2] , spheresSize , 10 , 10 );
    }

    for( unsigned int v = 0 ; v < mesh.V.size() ; ++v ) {
        Vec3 const & p = mesh.V[ v ].p;
        if(! verticesAreMarkedForCurrentHandle[ v ]) {
            int handleIdx = verticesHandles[v];
            if( handleIdx >= 0 ) {
                float r , g , b;
                calc_RGB( handleIdx , 0 , numberOfHandles , r , g  , b );
                if(handleIdx != activeHandle) {
                    r *= 0.5;  g *= 0.5;  b *= 0.5;
                }
                glColor3f(r,g,b);
                drawSphere( p[0] , p[1] , p[2] , spheresSize , 10 , 10 );
            }
        }
    }
}

void deleteSphereBrush() {
    radius = defaultRadius;
    lastVertexClicked = -1;
    hasToDrawShpereBrush = false;
}

void drawSphereBrush() {
    Vec3 v = mesh.V[lastVertexClicked];
    drawSphere( v[0] , v[1] , v[2] , defaultRadius , 10 , 10 );
}


void draw () {
    glEnable(GL_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glColor3f(0.4,0.4,0.8);
    mesh.draw();
    drawHandles();
    rectangleSelectionTool.draw();
    if(hasToDrawShpereBrush) drawSphereBrush();
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
    static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    static unsigned int counter = 0;
    counter++;
    float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    if (currentTime - lastTime >= 1000.0f) {
        FPS = counter;
        counter = 0;
        static char winTitle [64];
        sprintf (winTitle, "gmini - FPS: %d", FPS);
        glutSetWindowTitle (winTitle);
        lastTime = currentTime;
    }
    glutPostRedisplay ();
}


void SpecialInput(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:
        if( viewerState == ViewerState_TRANSLATINGHANDLE ) {
            Vec3 rv = getUpVector();
            translateActiveHandle( - 0.05 * rv );
        }
        else if( viewerState == ViewerState_ROTATINGHANDLE ) {
            Vec3 vv = getViewVector();
            rotateActiveHandle( vv , - M_PI / 30 );
        }
        else if( viewerState == ViewerState_NORMAL ) {
            if(activeHandle >= 0) {
                --activeHandle;
            }
        }
        break;
    case GLUT_KEY_UP:
        if( viewerState == ViewerState_TRANSLATINGHANDLE ) {
            Vec3 rv = getUpVector();
            translateActiveHandle( 0.05 * rv );
        }
        else if( viewerState == ViewerState_ROTATINGHANDLE ) {
            Vec3 vv = getViewVector();
            rotateActiveHandle( vv , M_PI / 30 );
        }
        else if( viewerState == ViewerState_NORMAL ) {
            if(activeHandle < numberOfHandles - 1) {
                ++activeHandle;
            }
        }
        break;
    case GLUT_KEY_LEFT:
        if( viewerState == ViewerState_TRANSLATINGHANDLE ) {
            Vec3 rv = getRightVector();
            translateActiveHandle( - 0.05 * rv );
        }
        else if( viewerState == ViewerState_ROTATINGHANDLE ) {
            Vec3 vv = getViewVector();
            rotateActiveHandle( vv , - M_PI / 30 );
        }
        else if( viewerState == ViewerState_NORMAL ) {
            if(activeHandle >= 0) {
                --activeHandle;
            }
        }
        break;
    case GLUT_KEY_RIGHT:
        if( viewerState == ViewerState_TRANSLATINGHANDLE ) {
            Vec3 rv = getRightVector();
            translateActiveHandle( 0.05 * rv );
        }
        else if( viewerState == ViewerState_ROTATINGHANDLE ) {
            Vec3 vv = getViewVector();
            rotateActiveHandle( vv , M_PI / 30 );
        }
        else if( viewerState == ViewerState_NORMAL ) {
            if(activeHandle < numberOfHandles - 1) {
                ++activeHandle;
            }
        }
        break;
    }
}


void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if( viewerState == ViewerState_NORMAL ) {
            if (fullScreen == true) {
                glutReshapeWindow (SCREENWIDTH, SCREENHEIGHT);
                fullScreen = false;
            } else {
                glutFullScreen ();
                fullScreen = true;
            }
        }
        break;

    case 'w':
        if( viewerState == ViewerState_NORMAL ) {
            GLint polygonMode[2];
            glGetIntegerv(GL_POLYGON_MODE, polygonMode);
            if(polygonMode[0] != GL_FILL)
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
            else
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        }
        break;

    case GLUT_KEY_ENTER:
        if( viewerState == ViewerState_EDITINGHANDLE ) {
            viewerState = ViewerState_NORMAL;
            finalizeEditingOfCurrentHandle();
            deleteSphereBrush();
        }
        break;

    case GLUT_KEY_ESCAPE:
        if( viewerState == ViewerState_TRANSLATINGHANDLE   ||   viewerState == ViewerState_ROTATINGHANDLE ) {
            viewerState = ViewerState_NORMAL;
        }
        break;

    case 'n':
        if( viewerState == ViewerState_NORMAL ) {
            viewerState = ViewerState_EDITINGHANDLE;
            ++numberOfHandles;
            activeHandle = numberOfHandles - 1; // last handle
        }
        break;

    case 'd':
        for (int i = 0; i < verticesAreMarkedForCurrentHandle.size(); ++i)
        {
            deleteSphereBrush();
            verticesAreMarkedForCurrentHandle[i] = false;
        }
        break;
        
    case 'g':
        if( viewerState == ViewerState_NORMAL   ||   viewerState == ViewerState_ROTATINGHANDLE ) {
            if( activeHandleIsValid() ) {
                viewerState = ViewerState_TRANSLATINGHANDLE;
            }
        }
        break;

    case 'r':
        if( viewerState == ViewerState_NORMAL   ||   viewerState == ViewerState_TRANSLATINGHANDLE ) {
            if( activeHandleIsValid() ) {
                viewerState = ViewerState_ROTATINGHANDLE;
            }
        }
        break;

    default:
        printUsage ();
        break;
    }
    idle ();
}

void mouseRoutine(int x, int y) {
    if(hasToDrawShpereBrush) {
        lastVertexClicked = getVertexFromClick(x, y);
        setNeighborsInRadiusFromVertex();
    }
}


void mouse (int button, int state, int x, int y) {
    if( glutGetModifiers() & GLUT_ACTIVE_CTRL    ||   rectangleSelectionTool.isActive ) { // we can activate the selection only with ctrl pressed
        if( viewerState == ViewerState_EDITINGHANDLE ) {
            if (state == GLUT_UP) {
                // then the mouse is released, confirm rectangle editing
                rectangleSelectionTool.isActive = false;
                addVerticesToCurrentHandle();
            } else {
                if (button == GLUT_LEFT_BUTTON) {
                    rectangleSelectionTool.initRectangle(x,y);
                    rectangleSelectionTool.isAdding = true;
                    rectangleSelectionTool.isActive = true;
                } else if (button == GLUT_RIGHT_BUTTON) {
                    rectangleSelectionTool.initRectangle(x,y);
                    rectangleSelectionTool.isAdding = false;
                    rectangleSelectionTool.isActive = true;
                }
            }
        }
    }
    else {
        if(lastVertexClicked != -1 && (button == 3 || button == 4)) {
            float maxRadius = 0.5;
            float minRadius = defaultRadius;
            float changeValue = 0.01;

            if(button == 3) {
                if(radius + changeValue <= maxRadius) radius += changeValue;
            } else {
                if(radius - changeValue >= minRadius) radius -= changeValue;
            }
            setNeighborsInRadiusFromVertex();
        }

        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            hasToDrawShpereBrush = true;
            lastVertexClicked = getVertexFromClick(x, y);
        }
        // moving the camera:
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
    }
    idle ();
}


void motion (int x, int y) {
    if( viewerState == ViewerState_EDITINGHANDLE  &&  rectangleSelectionTool.isActive ) {
        rectangleSelectionTool.updateRectangle(x,y);
    }
    else {
        // moving the camera:
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
}


void reshape(int w, int h) {
    camera.resize (w, h);
}


int main (int argc, char ** argv) {
    if (argc > 2) {
        printUsage ();
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("arap tp");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    glutPassiveMotionFunc(mouseRoutine);
    glutSpecialFunc(SpecialInput);
    key ('?', 0, 0);

    mesh.loadOFF(argc == 2 ? argv[1] : "models/arma.off");
    verticesAreMarkedForCurrentHandle.resize( mesh.V.size() , false );
    verticesHandles.resize( mesh.V.size() , -1 );
    edgeAndVertexWeights.buildClippedCotangentWeightsOfTriangleMesh( mesh.V , mesh.T );
    Eigen::MatrixXd idMatrix(3,3);
    idMatrix(0,0) = 1.0;   idMatrix(0,1) = 0.0;   idMatrix(0,2) = 0.0;
    idMatrix(1,0) = 0.0;   idMatrix(1,1) = 1.0;   idMatrix(1,2) = 0.0;
    idMatrix(2,0) = 0.0;   idMatrix(2,1) = 0.0;   idMatrix(2,2) = 1.0;
    vertexRotationMatrices.resize( mesh.V.size() , idMatrix );

    glutMainLoop ();
    return EXIT_SUCCESS;
}

