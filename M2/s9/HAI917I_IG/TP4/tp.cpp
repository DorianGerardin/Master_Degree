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

#include <algorithm>
#include <GL/glut.h>
#include <float.h>
#include "src/Vec3.h"
#include "src/Camera.h"

using namespace std;

void calcUniformMeanCurvature();
void calcMeanCurvature();
void calcTriangleQuality();
void uniformSmooth(unsigned int nIteration);
void uniformLBSmooth(unsigned int nIteration);

enum DisplayMode{ WIRE=0, SOLID=1, LIGHTED_WIRE=2, LIGHTED=3 };


struct Triangle {
    inline Triangle () {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
    }
    inline Triangle (unsigned int v0, unsigned int v1, unsigned int v2) {
        v[0] = v0;   v[1] = v1;   v[2] = v2;
    }
    unsigned int & operator [] (unsigned int iv) { return v[iv]; }
    unsigned int operator [] (unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle () {}
    inline Triangle & operator = (const Triangle & t) {
        v[0] = t.v[0];   v[1] = t.v[1];   v[2] = t.v[2];
        return (*this);
    }
    // membres indices des sommets du triangle:
    unsigned int v[3];
};


struct Mesh {
    std::vector< Vec3 > vertices; //array of mesh vertices positions
    std::vector< Vec3 > Lu;
    std::vector< Vec3 > Lbu;
    std::vector< Vec3 > G;
    std::vector< float > vunicurvature;
    std::vector< float > vcurvature;
    std::vector< float > vgausscurvature;
    std::vector< Vec3 > normals; //array of vertices normals useful for the display
    std::vector< Triangle > triangles; //array of mesh triangles
    std::vector < float > tshape;
    float minQ, maxQ;
    std::vector< Vec3 > triangle_normals; //triangle normals to display face normals
    std::vector<std::vector<unsigned int> > oneRing;

    //Compute face normals for the display
    void computeTrianglesNormals(){

        triangle_normals.clear();
        for( unsigned int i = 0 ; i < triangles.size() ;i++ ){
            const Vec3 & e0 = vertices[triangles[i][1]] - vertices[triangles[i][0]];
            const Vec3 & e1 = vertices[triangles[i][2]] - vertices[triangles[i][0]];
            Vec3 n = Vec3::cross( e0, e1 );
            n.normalize();
            triangle_normals.push_back( n );
        }
    }

    //Compute vertices normals as the average of its incident faces normals
    void computeVerticesNormals(  ){
        normals.clear();
        normals.resize( vertices.size(), Vec3(0., 0., 0.) );
        for( unsigned int i = 0 ; i < triangles.size() ;i++ ){
            for( unsigned int t = 0 ; t < 3 ; t++ )
                normals[ triangles[i][t] ] += triangle_normals[i];
        }
        for( unsigned int i = 0 ; i < vertices.size() ;i++ )
            normals[ i ].normalize();
    }

    void computeNormals(){
        computeTrianglesNormals();
        computeVerticesNormals();
    }

    void updateL() {
        calcUniformMeanCurvature();
        calcTriangleQuality();
        computeNormals();
    }

    void updateLB() {
        calcMeanCurvature();
        calcTriangleQuality();
        computeNormals();
    }

    void addNoise(){
        for( unsigned int i = 0 ; i < vertices.size() ; i ++ ) {

            float factor = 0.03;
            const Vec3 & p = vertices[i];
            const Vec3 & n = normals[i];
            vertices[i] = Vec3( p[0] + factor*((double)(rand()) / (double)
            (RAND_MAX))*n[0], p[1] + factor*((double)(rand()) / (double)
            (RAND_MAX))*n[1], p[2] + factor*((double)(rand()) / (double)
            (RAND_MAX))*n[2]);
        }
    }
};


//Input mesh loaded at the launch of the application
Mesh mesh;
std::vector< float > current_field; //normalized filed of each vertex

bool display_normals;
bool display_smooth_normals;
bool display_mesh;

DisplayMode displayMode;
int weight_type;


//Transformation made of a rotation and translation
struct Transformation {
    Mat3 rotation;
    Vec3 translation;
};


bool contain(std::vector<unsigned int> const & i_vector, unsigned int element) {
    for (unsigned int i = 0; i < i_vector.size(); i++) {
        if (i_vector[i] == element) return true;
    }
    return false;
}



void collect_one_ring (std::vector<Vec3> const & i_vertices,
                       std::vector< Triangle > const & i_triangles,
                       std::vector<std::vector<unsigned int> > & o_one_ring) {
    o_one_ring.clear();
    o_one_ring.resize(i_vertices.size()); //one-ring of each vertex, i.e. a list of vertices with which it shares an edge
    //Parcourir les triangles et ajouter les voisins dans le 1-voisinage
    //Attention verifier que l'indice n'est pas deja present
    for (unsigned int i = 0; i < i_triangles.size(); i++) {
        //Tous les points opposés dans le triangle sont reliés
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (j != k) {
                    if (!contain(o_one_ring[i_triangles[i][j]], i_triangles[i][k])) {
                        o_one_ring[i_triangles[i][j]].push_back(i_triangles[i][k]);
                    }
                }
            }
        }
    }
}

void normalizeValue(float &value, float min, float max) {
    float length = max - min;
    value = abs(value - min) / length;
}

void normalizeUniCurvatures() {
    size_t nVertices = mesh.vertices.size();
    float max = FLT_MIN; float min = FLT_MAX;
    for (unsigned int k = 0; k < nVertices; ++k) {
        if(mesh.vunicurvature[k] < min) min = mesh.vunicurvature[k];
        if(mesh.vunicurvature[k] > max) max = mesh.vunicurvature[k];
    }
    for (unsigned int k = 0; k < nVertices; ++k) {
        normalizeValue(mesh.vunicurvature[k], min, max);
    }
}

void normalizeCurvatures() {
    size_t nVertices = mesh.vertices.size();
    float max = FLT_MIN; float min = FLT_MAX;
    for (unsigned int k = 0; k < nVertices; ++k) {
        if(mesh.vcurvature[k] < min) min = mesh.vcurvature[k];
        if(mesh.vcurvature[k] > max) max = mesh.vcurvature[k];
    }
    for (unsigned int k = 0; k < nVertices; ++k) {
        normalizeValue(mesh.vcurvature[k], min, max);
    }
}

void normalizeGaussCurvatures() {
    size_t nVertices = mesh.vertices.size();
    float max = FLT_MIN; float min = FLT_MAX;
    for (unsigned int k = 0; k < nVertices; ++k) {
        if(mesh.vgausscurvature[k] < min) min = mesh.vgausscurvature[k];
        if(mesh.vgausscurvature[k] > max) max = mesh.vgausscurvature[k];
    }
    for (unsigned int k = 0; k < nVertices; ++k) {
        normalizeValue(mesh.vgausscurvature[k], min, max);
    }
}

void calcUniformMeanCurvature() {

    mesh.vunicurvature.clear();
    mesh.Lu.clear();
    size_t nVertices = mesh.vertices.size();
    mesh.vunicurvature.resize(nVertices);
    mesh.Lu.resize(nVertices);

    for (unsigned int i = 0; i < nVertices; ++i) {

        size_t nNeighbours = mesh.oneRing[i].size();
        Vec3 sumNeighboors = Vec3(0., 0., 0.);

        for (unsigned int j = 0; j < nNeighbours; ++j) {

            sumNeighboors += mesh.vertices[mesh.oneRing[i][j]];
        }
        mesh.Lu[i] = ((sumNeighboors / (float) nNeighbours) - mesh.vertices[i]);
        mesh.vunicurvature[i] = mesh.Lu[i].length() / 2.0;
    }
    normalizeUniCurvatures();
    current_field = mesh.vunicurvature;
}

void uniformSmooth(unsigned int nIteration) {

    size_t nVertices = mesh.vertices.size();
    for (unsigned int i = 0; i < nIteration; ++i) {

        vector<Vec3> newVertices(nVertices);

        for (unsigned int j = 0; j < nVertices; ++j) {

            newVertices[j] = mesh.vertices[j] + 0.5 * mesh.Lu[j];
        }
        mesh.vertices = newVertices;
        mesh.updateL();
    }
}

void taubinSmooth(float lambda, float mu, unsigned int nIteration) {
    size_t nVertices = mesh.vertices.size();
    for (unsigned int i = 0; i < nIteration; ++i) {

        vector<Vec3> newVertices(nVertices);

        for (unsigned int j = 0; j < nVertices; ++j) {

            newVertices[j] = mesh.vertices[j] + lambda * mesh.Lu[j];
        }
        mesh.vertices = newVertices;
        mesh.updateL();
        newVertices.clear();
        newVertices.resize(nVertices);

        for (unsigned int j = 0; j < nVertices; ++j) {

            newVertices[j] = mesh.vertices[j] + mu * mesh.Lu[j];
        }
        mesh.vertices = newVertices;
        mesh.updateL();
    }
}

void calcTriangleQuality() {

    size_t nTriangles = mesh.triangles.size();
    mesh.tshape.clear();
    mesh.tshape.resize(nTriangles);

    float maxQ = FLT_MIN; float minQ = FLT_MAX;
    for (unsigned int i = 0; i < nTriangles; ++i)
    {
        Vec3 a = (mesh.vertices[mesh.triangles[i][1]] - mesh.vertices[mesh.triangles[i][0]]); 
        Vec3 b = (mesh.vertices[mesh.triangles[i][2]] - mesh.vertices[mesh.triangles[i][0]]); 
        Vec3 c = (mesh.vertices[mesh.triangles[i][1]] - mesh.vertices[mesh.triangles[i][2]]);
        float minEdge = min(min(a.length(), b.length()), c.length()); 

        float area = Vec3::cross(a, b).length() / 2.0;
        float r = (a.length() * b.length() * c.length()) / (4.0 * area); 
        mesh.tshape[i] = r / minEdge;
        if(mesh.tshape[i] < minQ) minQ = mesh.tshape[i];
        if(mesh.tshape[i] > maxQ) maxQ = mesh.tshape[i];
    }
    mesh.maxQ = maxQ;
    mesh.minQ = minQ;
}

vector<float> getOppositeAngles(unsigned int vertexIndex, unsigned int neighborIndex) {

    vector<float> angles;
    size_t nTriangles = mesh.triangles.size();
    for (unsigned int i = 0; i < nTriangles; ++i)
    {
        if(mesh.triangles[i][0] == vertexIndex && mesh.triangles[i][1] == neighborIndex) {
            Vec3 o = mesh.vertices[mesh.triangles[i][2]];
            angles.push_back(acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][0]] - o) , (mesh.vertices[mesh.triangles[i][1]] - o) )));
        }
        else if(mesh.triangles[i][1] == vertexIndex && mesh.triangles[i][0] == neighborIndex) {
            Vec3 o = mesh.vertices[mesh.triangles[i][2]];
            angles.push_back(acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][0]] - o) , (mesh.vertices[mesh.triangles[i][1]] - o) )));
        }
        else if(mesh.triangles[i][0] == vertexIndex && mesh.triangles[i][2] == neighborIndex) {
            Vec3 o = mesh.vertices[mesh.triangles[i][1]];
            angles.push_back(acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][0]] - o) , (mesh.vertices[mesh.triangles[i][2]] - o) )));
        }
        else if(mesh.triangles[i][2] == vertexIndex && mesh.triangles[i][0] == neighborIndex) {
            Vec3 o = mesh.vertices[mesh.triangles[i][1]];
            angles.push_back(acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][0]] - o) , (mesh.vertices[mesh.triangles[i][2]] - o) )));
        }
        else if(mesh.triangles[i][1] == vertexIndex && mesh.triangles[i][2] == neighborIndex) {
            Vec3 o = mesh.vertices[mesh.triangles[i][0]];
            angles.push_back(acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][1]] - o) , (mesh.vertices[mesh.triangles[i][2]] - o) )));
        }
        else if(mesh.triangles[i][2] == vertexIndex && mesh.triangles[i][1] == neighborIndex) {
            Vec3 o = mesh.vertices[mesh.triangles[i][0]];
            angles.push_back(acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][1]] - o) , (mesh.vertices[mesh.triangles[i][2]] - o) )));
        }
        if(angles.size() == 2) return angles;
    }
    return angles;
}

void calcMeanCurvature() {

    mesh.vcurvature.clear();
    mesh.Lbu.clear();
    size_t nVertices = mesh.vertices.size();
    mesh.vcurvature.resize(nVertices);
    mesh.Lbu.resize(nVertices);

    for (unsigned int i = 0; i < nVertices; ++i) {

        size_t nNeighbours = mesh.oneRing[i].size();
        Vec3 sum = Vec3(0., 0., 0.);
        float weightSum = 0.;

        for (unsigned int j = 0; j < nNeighbours; ++j) {

            vector<float> angles = getOppositeAngles(i, mesh.oneRing[i][j]);
            float alpha = angles[0];
            float beta = angles[1];

            float cotAlpha = cos(alpha) / sin(alpha);
            float cotBeta = cos(beta) / sin(beta);
            float weight = 0.5 * (cotAlpha + cotBeta);

            weightSum += weight;
            sum += weight * (mesh.vertices[mesh.oneRing[i][j]] - mesh.vertices[i]);  
        }
        mesh.Lbu[i] = ((float)1. / (float)weightSum) * sum;
        mesh.vcurvature[i] = mesh.Lbu[i].length() * 0.5;
    }
    normalizeCurvatures();
    current_field = mesh.vcurvature;
}

void taubinLBSmooth(float lambda, float mu, unsigned int nIteration) {
    size_t nVertices = mesh.vertices.size();
    for (unsigned int i = 0; i < nIteration; ++i) {

        vector<Vec3> newVertices(nVertices);

        for (unsigned int j = 0; j < nVertices; ++j) {

            newVertices[j] = mesh.vertices[j] + lambda * mesh.Lbu[j];
        }
        mesh.vertices = newVertices;
        mesh.updateLB();
        newVertices.clear();
        newVertices.resize(nVertices);

        for (unsigned int j = 0; j < nVertices; ++j) {

            newVertices[j] = mesh.vertices[j] + mu * mesh.Lbu[j];
        }
        mesh.vertices = newVertices;
        mesh.updateLB();
    }
}

float getSumAnglesForVertex(unsigned int vertexIndex) {

    float anglesSum = 0.;
    size_t nTriangles = mesh.triangles.size();
    for (unsigned int i = 0; i < nTriangles; ++i)
    {
        if(mesh.triangles[i][0] == vertexIndex) {
            Vec3 current = mesh.vertices[mesh.triangles[i][0]];
            anglesSum += (acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][2]] - current) , (mesh.vertices[mesh.triangles[i][1]] - current) )));
        }
        else if(mesh.triangles[i][1] == vertexIndex) {
            Vec3 current = mesh.vertices[mesh.triangles[i][1]];
            anglesSum += (acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][0]] - current) , (mesh.vertices[mesh.triangles[i][2]] - current) )));
        }
        else if(mesh.triangles[i][2] == vertexIndex) {
            Vec3 current = mesh.vertices[mesh.triangles[i][2]];
            anglesSum += (acos(Vec3::dot( (mesh.vertices[mesh.triangles[i][0]] - current) , (mesh.vertices[mesh.triangles[i][1]] - current) )));
        }
    }
    return anglesSum;
}

void calcGaussCurvature() {

    mesh.vgausscurvature.clear();
    mesh.G.clear();
    size_t nVertices = mesh.vertices.size();
    mesh.vgausscurvature.resize(nVertices);
    mesh.G.resize(nVertices);

    for (unsigned int i = 0; i < nVertices; ++i) {

        float sumAngles = getSumAnglesForVertex(i);
        mesh.vgausscurvature[i] = 2 * M_PI - sumAngles;
    }
    normalizeGaussCurvatures();
    current_field = mesh.vgausscurvature;
}


// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;
static unsigned int SCREENWIDTH = 1600;
static unsigned int SCREENHEIGHT = 900;
static Camera camera;
static bool mouseRotatePressed = false;
static bool mouseMovePressed = false;
static bool mouseZoomPressed = false;
static int lastX=0, lastY=0, lastZoom=0;
static bool fullScreen = false;

// ------------------------------------
// File I/O
// ------------------------------------
bool saveOFF( const std::string & filename ,
              std::vector< Vec3 > const & i_vertices ,
              std::vector< Vec3 > const & i_normals ,
              std::vector< Triangle > const & i_triangles,
              std::vector< Vec3 > const & i_triangle_normals ,
              bool save_normals = true ) {
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open()) {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl ;

    unsigned int n_vertices = i_vertices.size() , n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for( unsigned int v = 0 ; v < n_vertices ; ++v ) {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals) myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else myfile << std::endl;
    }
    for( unsigned int f = 0 ; f < n_triangles ; ++f ) {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2]<< " ";
        if (save_normals) myfile << i_triangle_normals[f][0] << " " << i_triangle_normals[f][1] << " " << i_triangle_normals[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF( std::string const & filename,
              std::vector<Vec3> & o_vertices,
              std::vector<Vec3> & o_normals,
              std::vector< Triangle > & o_triangles,
              std::vector< Vec3 > & o_triangle_normals,
              bool load_normals = true )
{
    std::ifstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return;
    }

    std::string magic_s;

    myfile >> magic_s;

    if( magic_s != "OFF" )
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices , n_faces , dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for( int v = 0 ; v < n_vertices ; ++v )
    {
        float x , y , z ;

        myfile >> x >> y >> z ;
        o_vertices.push_back( Vec3( x , y , z ) );

        if( load_normals ) {
            myfile >> x >> y >> z;
            o_normals.push_back( Vec3( x , y , z ) );
        }
    }

    o_triangles.clear();
    o_triangle_normals.clear();
    for( int f = 0 ; f < n_faces ; ++f )
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if( n_vertices_on_face == 3 )
        {
            unsigned int _v1 , _v2 , _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle( _v1, _v2, _v3 ));

            if( load_normals ) {
                float x , y , z ;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back( Vec3( x , y , z ) );
            }
        }
        else if( n_vertices_on_face == 4 )
        {
            unsigned int _v1 , _v2 , _v3 , _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3 ));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
            if( load_normals ) {
                float x , y , z ;
                myfile >> x >> y >> z;
                o_triangle_normals.push_back( Vec3( x , y , z ) );
            }

        }
        else
        {
            std::cout << "We handle ONLY *.off files with 3 or 4 vertices per face" << std::endl;
            myfile.close();
            exit(1);
        }
    }

}


// ------------------------------------
// Application initialization
// ------------------------------------
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
    glCullFace (GL_BACK);
    glDisable (GL_CULL_FACE);
    glDepthFunc (GL_LESS);
    glEnable (GL_DEPTH_TEST);
    glClearColor (0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    display_normals = false;
    display_mesh = true;
    display_smooth_normals = true;
    displayMode = LIGHTED;

}


// ------------------------------------
// Rendering.
// ------------------------------------

void drawVector( Vec3 const & i_from, Vec3 const & i_to ) {

    glBegin(GL_LINES);
    glVertex3f( i_from[0] , i_from[1] , i_from[2] );
    glVertex3f( i_to[0] , i_to[1] , i_to[2] );
    glEnd();
}

void drawAxis( Vec3 const & i_origin, Vec3 const & i_direction ) {

    glLineWidth(4); // for example...
    drawVector(i_origin, i_origin + i_direction);
}

void drawReferenceFrame( Vec3 const & origin, Vec3 const & i, Vec3 const & j, Vec3 const & k ) {

    glDisable(GL_LIGHTING);
    glColor3f( 0.8, 0.2, 0.2 );
    drawAxis( origin, i );
    glColor3f( 0.2, 0.8, 0.2 );
    drawAxis( origin, j );
    glColor3f( 0.2, 0.2, 0.8 );
    drawAxis( origin, k );
    glEnable(GL_LIGHTING);

}


typedef struct {
    float r;       // ∈ [0, 1]
    float g;       // ∈ [0, 1]
    float b;       // ∈ [0, 1]
} RGB;



RGB scalarToRGB( float scalar_value ) //Scalar_value ∈ [0, 1]
{
    RGB rgb;
    float H = scalar_value*360., S = 1., V = 0.85,
            P, Q, T,
            fract;

    (H == 360.)?(H = 0.):(H /= 60.);
    fract = H - floor(H);

    P = V*(1. - S);
    Q = V*(1. - S*fract);
    T = V*(1. - S*(1. - fract));

    if      (0. <= H && H < 1.)
        rgb = (RGB){.r = V, .g = T, .b = P};
    else if (1. <= H && H < 2.)
        rgb = (RGB){.r = Q, .g = V, .b = P};
    else if (2. <= H && H < 3.)
        rgb = (RGB){.r = P, .g = V, .b = T};
    else if (3. <= H && H < 4.)
        rgb = (RGB){.r = P, .g = Q, .b = V};
    else if (4. <= H && H < 5.)
        rgb = (RGB){.r = T, .g = P, .b = V};
    else if (5. <= H && H < 6.)
        rgb = (RGB){.r = V, .g = P, .b = Q};
    else
        rgb = (RGB){.r = 0., .g = 0., .b = 0.};

    return rgb;
}

void drawSmoothTriangleMesh( Mesh const & i_mesh , bool draw_field = false ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {

        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position
            const Vec3 & n = i_mesh.normals[i_mesh.triangles[tIt][i]]; //Vertex normal

            if( draw_field && current_field.size() > 0 ){
                RGB color = scalarToRGB( current_field[i_mesh.triangles[tIt][i]] );
                glColor3f( color.r, color.g, color.b );
            }
            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawTriangleMesh( Mesh const & i_mesh , bool draw_field = false  ) {
    glBegin(GL_TRIANGLES);
    for(unsigned int tIt = 0 ; tIt < i_mesh.triangles.size(); ++tIt) {
        const Vec3 & n = i_mesh.triangle_normals[ tIt ]; //Triangle normal

        //RGB color = scalarToRGB((i_mesh.tshape[tIt] - i_mesh.minQ) / (i_mesh.maxQ - i_mesh.minQ));
        RGB color = scalarToRGB(i_mesh.tshape[tIt]);
        glColor3f(color.r, color.g, color.b);

        for(unsigned int i = 0 ; i < 3 ; i++) {
            const Vec3 & p = i_mesh.vertices[i_mesh.triangles[tIt][i]]; //Vertex position

            // if( draw_field && current_field.size() > 0 ){
            //     RGB color = scalarToRGB( current_field[i_mesh.triangles[tIt][i]] );
            //     glColor3f( color.r, color.g, color.b );
            // }
            glNormal3f( n[0] , n[1] , n[2] );
            glVertex3f( p[0] , p[1] , p[2] );
        }
    }
    glEnd();

}

void drawMesh( Mesh const & i_mesh , bool draw_field = false ){
    if(display_smooth_normals)
        drawSmoothTriangleMesh(i_mesh, draw_field) ; //Smooth display with vertices normals
    else
        drawTriangleMesh(i_mesh, draw_field) ; //Display with face normals
}

void drawVectorField( std::vector<Vec3> const & i_positions, std::vector<Vec3> const & i_directions ) {
    glLineWidth(1.);
    for(unsigned int pIt = 0 ; pIt < i_directions.size() ; ++pIt) {
        Vec3 to = i_positions[pIt] + 0.02*i_directions[pIt];
        drawVector(i_positions[pIt], to);
    }
}

void drawNormals(Mesh const& i_mesh){

    if(display_smooth_normals){
        drawVectorField( i_mesh.vertices, i_mesh.normals );
    } else {
        std::vector<Vec3> triangle_baricenters;
        for ( const Triangle& triangle : i_mesh.triangles ){
            Vec3 triangle_baricenter (0.,0.,0.);
            for( unsigned int i = 0 ; i < 3 ; i++ )
                triangle_baricenter += i_mesh.vertices[triangle[i]];
            triangle_baricenter /= 3.;
            triangle_baricenters.push_back(triangle_baricenter);
        }

        drawVectorField( triangle_baricenters, i_mesh.triangle_normals );
    }
}

//Draw fonction
void draw () {

    if(displayMode == LIGHTED || displayMode == LIGHTED_WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_LIGHTING);

    }  else if(displayMode == WIRE){

        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glDisable (GL_LIGHTING);

    }  else if(displayMode == SOLID ){
        glDisable (GL_LIGHTING);
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    }

    glColor3f(0.8,1,0.8);
    drawMesh(mesh, true);

    if(displayMode == SOLID || displayMode == LIGHTED_WIRE){
        glEnable (GL_POLYGON_OFFSET_LINE);
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
        glLineWidth (1.0f);
        glPolygonOffset (-2.0, 1.0);

        glColor3f(0.,0.,0.);
        drawMesh(mesh, false);

        glDisable (GL_POLYGON_OFFSET_LINE);
        glEnable (GL_LIGHTING);
    }



    glDisable(GL_LIGHTING);
    if(display_normals){
        glColor3f(1.,0.,0.);
        drawNormals(mesh);
    }

    glEnable(GL_LIGHTING);


}

void changeDisplayMode(){
    if(displayMode == LIGHTED)
        displayMode = LIGHTED_WIRE;
    else if(displayMode == LIGHTED_WIRE)
        displayMode = SOLID;
    else if(displayMode == SOLID)
        displayMode = WIRE;
    else
        displayMode = LIGHTED;
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

// ------------------------------------
// User inputs
// ------------------------------------
//Keyboard event
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
        changeDisplayMode();
        break;


    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;

    case '1': //Toggle loaded mesh display
        display_mesh = !display_mesh;
        break;

    case 's': //Switches between face normals and vertices normals
        display_smooth_normals = !display_smooth_normals;
        break;

    case 'c': //Draw curvature of each vertices
        current_field = mesh.vunicurvature;
        break;

    case 'g': //Draw curvature of each vertices
        current_field = mesh.vgausscurvature;
        break;

    case 'm': //Draw curvature of each vertices
        current_field = mesh.vcurvature;
        break;

    case 'l': //Switches between face normals and vertices normals
        uniformSmooth(1);
        break;

    case 'b': //Switches between face normals and vertices normals
        taubinLBSmooth(0.330, -0.331, 1);
        break;

    case 't': //Switches between face normals and vertices normals
        taubinSmooth(0.330, -0.331, 1);
        break;

    case '+': //Changes weight type: 0 uniforme, 1 aire des triangles, 2 angle du triangle
        weight_type ++;
        if(weight_type == 3) weight_type = 0;
        mesh.computeVerticesNormals(); //recalcul des normales avec le type de poids choisi
        break;

    default:
        break;
    }
    idle ();
}

//Mouse events
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

//Mouse motion, update camera
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


// ------------------------------------
// Start of graphical application
// ------------------------------------
int main (int argc, char ** argv) {
    if (argc > 2) {
        exit (EXIT_FAILURE);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow ("TP HAI917I");

    init ();
    glutIdleFunc (idle);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutReshapeFunc (reshape);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    key ('?', 0, 0);

    //Mesh loaded with precomputed normals
    openOFF("data/elephant_n.off", mesh.vertices, mesh.normals, mesh.triangles, mesh.triangle_normals);

    //Completer les fonction de calcul de normals
    mesh.computeNormals();

    collect_one_ring(mesh.vertices, mesh.triangles, mesh.oneRing);
    
    mesh.addNoise();

    calcTriangleQuality();
    calcUniformMeanCurvature();
    calcMeanCurvature();
    calcGaussCurvature();

    // A faire : normaliser les champs pour avoir une valeur flotante entre 0. et 1. dans current_field
    //***********************************************//

    current_field.clear();

    glutMainLoop ();
    return EXIT_SUCCESS;
}

