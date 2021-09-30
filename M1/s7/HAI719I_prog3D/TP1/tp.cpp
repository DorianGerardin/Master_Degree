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

struct Triangle
{
    inline Triangle()
    {
        v[0] = v[1] = v[2] = 0;
    }
    inline Triangle(const Triangle &t)
    {
        v[0] = t.v[0];
        v[1] = t.v[1];
        v[2] = t.v[2];
    }
    inline Triangle(unsigned int v0, unsigned int v1, unsigned int v2)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }
    unsigned int &operator[](unsigned int iv) { return v[iv]; }
    unsigned int operator[](unsigned int iv) const { return v[iv]; }
    inline virtual ~Triangle() {}
    inline Triangle &operator=(const Triangle &t)
    {
        v[0] = t.v[0];
        v[1] = t.v[1];
        v[2] = t.v[2];
        return (*this);
    }
    // membres :
    unsigned int v[3];
};

struct Mesh
{
    std::vector<Vec3> vertices;
    std::vector<float> positionArray;
    std::vector<Vec3> normals;
    std::vector<float> normalArray;
    std::vector<Triangle> triangles;
    std::vector<unsigned int> triangleArray;
    std::vector<float> colorArray;

    void buildVertexArray()
    {
        for (unsigned int i = 0; i < vertices.size(); i++)
        {
            positionArray.push_back(vertices[i][0]);
            positionArray.push_back(vertices[i][1]);
            positionArray.push_back(vertices[i][2]);
            
            normalArray.push_back(normals[i][0]);
            normalArray.push_back(normals[i][1]);
            normalArray.push_back(normals[i][2]);
        }
    };

    void buildTriangleArray()
    {
        for (unsigned int i = 0; i < triangles.size(); i++)
        {
            triangleArray.push_back(triangles[i][0]);
            triangleArray.push_back(triangles[i][1]);
            triangleArray.push_back(triangles[i][2]);
        }
    };

    void buildColorArray() 
    {
        for (unsigned int i = 0; i < triangles.size(); i++)
        {
            if(i%2) {
                colorArray.push_back(0); //R
                colorArray.push_back(1); //G
                colorArray.push_back(0); //B
            } else {
                colorArray.push_back(0); //R
                colorArray.push_back(0); //G
                colorArray.push_back(1); //B
            }
        }
    }
};

Mesh mesh;

bool display_normals;
bool display_loaded_mesh;

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
static int lastX = 0, lastY = 0, lastZoom = 0;
static bool fullScreen = false;

bool saveOFF(const std::string &filename,
             std::vector<Vec3> &i_vertices,
             std::vector<Vec3> &i_normals,
             std::vector<Triangle> &i_triangles,
             bool save_normals = true)
{
    std::ofstream myfile;
    myfile.open(filename.c_str());
    if (!myfile.is_open())
    {
        std::cout << filename << " cannot be opened" << std::endl;
        return false;
    }

    myfile << "OFF" << std::endl;

    unsigned int n_vertices = i_vertices.size(), n_triangles = i_triangles.size();
    myfile << n_vertices << " " << n_triangles << " 0" << std::endl;

    for (unsigned int v = 0; v < n_vertices; ++v)
    {
        myfile << i_vertices[v][0] << " " << i_vertices[v][1] << " " << i_vertices[v][2] << " ";
        if (save_normals)
            myfile << i_normals[v][0] << " " << i_normals[v][1] << " " << i_normals[v][2] << std::endl;
        else
            myfile << std::endl;
    }
    for (unsigned int f = 0; f < n_triangles; ++f)
    {
        myfile << 3 << " " << i_triangles[f][0] << " " << i_triangles[f][1] << " " << i_triangles[f][2];
        myfile << std::endl;
    }
    myfile.close();
    return true;
}

void openOFF(std::string const &filename,
             std::vector<Vec3> &o_vertices,
             std::vector<Vec3> &o_normals,
             std::vector<Triangle> &o_triangles,
             bool load_normals = true)
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

    if (magic_s != "OFF")
    {
        std::cout << magic_s << " != OFF :   We handle ONLY *.off files." << std::endl;
        myfile.close();
        exit(1);
    }

    int n_vertices, n_faces, dummy_int;
    myfile >> n_vertices >> n_faces >> dummy_int;

    o_vertices.clear();
    o_normals.clear();

    for (int v = 0; v < n_vertices; ++v)
    {
        float x, y, z;

        myfile >> x >> y >> z;
        o_vertices.push_back(Vec3(x, y, z));

        if (load_normals)
        {
            myfile >> x >> y >> z;
            o_normals.push_back(Vec3(x, y, z));
        }
    }

    o_triangles.clear();
    for (int f = 0; f < n_faces; ++f)
    {
        int n_vertices_on_face;
        myfile >> n_vertices_on_face;

        if (n_vertices_on_face == 3)
        {
            unsigned int _v1, _v2, _v3;
            myfile >> _v1 >> _v2 >> _v3;

            o_triangles.push_back(Triangle(_v1, _v2, _v3));
        }
        else if (n_vertices_on_face == 4)
        {
            unsigned int _v1, _v2, _v3, _v4;
            myfile >> _v1 >> _v2 >> _v3 >> _v4;

            o_triangles.push_back(Triangle(_v1, _v2, _v3));
            o_triangles.push_back(Triangle(_v1, _v3, _v4));
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

void initLight()
{
    GLfloat light_position1[4] = {22.0f, 16.0f, 50.0f, 0.0f};
    GLfloat direction1[3] = {-52.0f, -16.0f, -50.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient[4] = {0.3f, 0.3f, 0.3f, 0.5f};

    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, color1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
}

void init()
{
    camera.resize(SCREENWIDTH, SCREENHEIGHT);
    initLight();
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glEnable(GL_COLOR_MATERIAL);

    display_normals = false;
    display_loaded_mesh = true;
}

// ------------------------------------
// rendering.
// ------------------------------------

void drawVector(Vec3 const &i_from, Vec3 const &i_to)
{

    glBegin(GL_LINES);
    glVertex3f(i_from[0], i_from[1], i_from[2]);
    glVertex3f(i_to[0], i_to[1], i_to[2]);
    glEnd();
}

//Fonction to change
void drawTriangleMesh(Mesh const &i_mesh)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_COLOR_MATERIAL);
    glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), (GLvoid *)(&i_mesh.positionArray[0]));
    glNormalPointer(GL_FLOAT, 3*sizeof(float), (GLvoid*)(&i_mesh.normalArray[0]));
    glColorPointer(3, GL_FLOAT, 3*sizeof(float), (GLvoid*)(&i_mesh.colorArray[0]));
    glDrawElements(GL_TRIANGLES, i_mesh.triangleArray.size(), GL_UNSIGNED_INT, (GLvoid *)(&i_mesh.triangleArray[0]));
}

void drawNormals(Mesh const &i_mesh)
{

    glLineWidth(1.);
    for (unsigned int pIt = 0; pIt < i_mesh.normals.size(); ++pIt)
    {
        Vec3 to = i_mesh.vertices[pIt] + 0.02 * i_mesh.normals[pIt];
        drawVector(i_mesh.vertices[pIt], to);
    }
}

void draw()
{

    if (display_loaded_mesh)
    {
        glColor3f(0.8, 0.8, 1);
        drawTriangleMesh(mesh);
    }

    if (display_normals)
    {
        glColor3f(1., 0., 0.);
        drawNormals(mesh);
    }
}

void display()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply();
    draw();
    glFlush();
    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

void key(unsigned char keyPressed, int x, int y)
{
    switch (keyPressed)
    {
    case 'f':
        if (fullScreen == true)
        {
            glutReshapeWindow(SCREENWIDTH, SCREENHEIGHT);
            fullScreen = false;
        }
        else
        {
            glutFullScreen();
            fullScreen = true;
        }
        break;

    case 'w':
        GLint polygonMode[2];
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);
        if (polygonMode[0] != GL_FILL)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

    case 'n': //Press n key to display normals
        display_normals = !display_normals;
        break;

    case '1': //Toggle loaded mesh display
        display_loaded_mesh = !display_loaded_mesh;
        break;

    default:
        break;
    }
    idle();
}

void mouse(int button, int state, int x, int y)
{
    if (state == GLUT_UP)
    {
        mouseMovePressed = false;
        mouseRotatePressed = false;
        mouseZoomPressed = false;
    }
    else
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            camera.beginRotate(x, y);
            mouseMovePressed = false;
            mouseRotatePressed = true;
            mouseZoomPressed = false;
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
            lastX = x;
            lastY = y;
            mouseMovePressed = true;
            mouseRotatePressed = false;
            mouseZoomPressed = false;
        }
        else if (button == GLUT_MIDDLE_BUTTON)
        {
            if (mouseZoomPressed == false)
            {
                lastZoom = y;
                mouseMovePressed = false;
                mouseRotatePressed = false;
                mouseZoomPressed = true;
            }
        }
    }
    idle();
}

void motion(int x, int y)
{
    if (mouseRotatePressed == true)
    {
        camera.rotate(x, y);
    }
    else if (mouseMovePressed == true)
    {
        camera.move((x - lastX) / static_cast<float>(SCREENWIDTH), (lastY - y) / static_cast<float>(SCREENHEIGHT), 0.0);
        lastX = x;
        lastY = y;
    }
    else if (mouseZoomPressed == true)
    {
        camera.zoom(float(y - lastZoom) / SCREENHEIGHT);
        lastZoom = y;
    }
}

void reshape(int w, int h)
{
    camera.resize(w, h);
}

int main(int argc, char **argv)
{
    if (argc > 2)
    {
        exit(EXIT_FAILURE);
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
    window = glutCreateWindow("TP HAI714I");

    init();
    glutIdleFunc(idle);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutReshapeFunc(reshape);
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    key('?', 0, 0);

    //Look into data to find other meshes
    openOFF("data/avion_n.off", mesh.vertices, mesh.normals, mesh.triangles);
    mesh.buildVertexArray();
    mesh.buildTriangleArray();
    mesh.buildColorArray();

    glutMainLoop();
    return EXIT_SUCCESS;
}
