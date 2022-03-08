// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../common/stb_image.h"
#include "tools/Object.cpp"

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  40.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//rotation
float angle = 0.;
float zoom = 1.;

float rotationX = 0.;
float rotationY = 0.;
float rotationZ = 0.;
float rotationNoStop = 0.;
float speedRotation = 1.;

float planeSize = 16.;
std::vector<unsigned short> indicesPlane; //Triangles concaténés dans une liste
std::vector<std::vector<unsigned short>> trianglesPlane;
std::vector<glm::vec3> indexed_verticesPlane;
std::vector<float> uv_surface;

GLuint vertexbufferPlane;
GLuint elementbufferPlane;

GLuint vertexbuffer;
GLuint elementbuffer;
GLuint uvbuffer;

int displayMode = 0;
/*******************************************************************************/


void generateGeometryPlane(float size, std::vector<glm::vec3> & indexed_vertices,
                                     std::vector<unsigned short> & indices, 
                                     std::vector<std::vector<unsigned short>> & triangles,
                                     std::vector<float>& uv) {

    indexed_vertices.clear();
    indices.clear();
    triangles.clear();
    uv.clear();

    int minZ = -1;
    int maxZ = 1;

    size = (int)floor(size);
    int length = 10;

    //std::cout << "planeSize: " << size << std::endl;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            glm::vec3 vertex = glm::vec3(( (float)i/(float)(size-1)*length)-length/2, ((float)j/(float)(size-1)*length)-length/2, 0);
            indexed_vertices.push_back(vertex);
            uv.push_back((float)j/(float)(size-1));
            uv.push_back((float)i/(float)(size-1));
        }
    }

    for (int i = 0; i < size-1; ++i)
    {
        for (int j = 0; j < size-1; ++j)
        {
            std::vector<unsigned short> triangle;
            triangle.push_back(i*size+j);
            triangle.push_back(i*size+(j+1));
            triangle.push_back((i+1)*size+j);
            triangles.push_back(triangle);

            indices.push_back(i*size+j);
            indices.push_back(i*size+(j+1));
            indices.push_back((i+1)*size+j);

            indices.push_back(i*size+(j+1));
            indices.push_back((i+1)*size+(j+1));
            indices.push_back((i+1)*size+j);
        }
    }
}

void drawSphere(GLuint vertexbuffer, GLuint elementbuffer, std::vector<unsigned short> indices) {

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
                GL_TRIANGLES,      // mode
                indices.size(),    // count
                GL_UNSIGNED_SHORT, // type
                (void*)0           // element array buffer offset
                );
}

glm::vec3 getBarycentre(std::vector<glm::vec3> vertices) {
    glm::vec3 barycentre = glm::vec3(0.f, 0.f, 0.f);
    unsigned int size = vertices.size();
    for (int i = 0; i < size; ++i)
    {
        barycentre[0] += vertices[i][0];
        barycentre[1] += vertices[i][1];
        barycentre[2] += vertices[i][2];
    }
    barycentre[0] /= size;
    barycentre[1] /= size;
    barycentre[2] /= size;
    return barycentre;
}


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "TP3 - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "vertex_shader.glsl", "fragment_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    /****************************************/

    // GLuint TextureHmap = loadBMP_custom("textures/Heightmap_Mountain.bmp");
    // GLuint TextureGrass = loadBMP_custom("textures/grass.bmp");
    // GLuint TextureRock = loadBMP_custom("textures/rock.bmp");
    // GLuint TextureSnow = loadBMP_custom("textures/snowrocks.bmp");

    // GLuint TextureIDHmap = glGetUniformLocation(programID,"hmapSampler");
    // GLuint TextureIDGrass = glGetUniformLocation(programID,"grassSampler");
    // GLuint TextureIDRock = glGetUniformLocation(programID,"rockSampler");
    // GLuint TextureIDSnow = glGetUniformLocation(programID,"snowSampler");

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // std::vector<unsigned short> indices_Sun; //Triangles concaténés dans une liste
    // std::vector<std::vector<unsigned short> > triangles_Sun;
    // std::vector<glm::vec3> indexed_vertices_Sun;
    Object *sun = new Object("objects/sphere966.off");
    // std::string filename("objects/suzanne.off");
    //loadOFF(filename, indexed_vertices_Sun, indices_Sun, triangles_Sun );

    GLuint vertexbuffer_Sun;
    glGenBuffers(1, &vertexbuffer_Sun);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_Sun);
    glBufferData(GL_ARRAY_BUFFER, sun->indexed_vertices.size() * sizeof(glm::vec3), &sun->indexed_vertices[0], GL_STATIC_DRAW);

    GLuint elementbuffer_Sun;
    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer_Sun);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_Sun);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sun->indices.size() * sizeof(unsigned short), &sun->indices[0] , GL_STATIC_DRAW);

    //Texture
    // glGenBuffers(1,&uvbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
    // glBufferData(GL_ARRAY_BUFFER,uv_surface.size()*sizeof(float),&uv_surface[0],GL_STATIC_DRAW);

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        glGenBuffers(1, &vertexbuffer_Sun);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer_Sun);
        glBufferData(GL_ARRAY_BUFFER, sun->indexed_vertices.size() * sizeof(glm::vec3), &sun->indexed_vertices[0], GL_STATIC_DRAW);

        // Generate a buffer for the indices as well
        glGenBuffers(1, &elementbuffer_Sun);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_Sun);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sun->indices.size() * sizeof(unsigned short), &sun->indices[0] , GL_STATIC_DRAW);

        //Texture
        // glGenBuffers(1,&uvbuffer);
        // glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
        // glBufferData(GL_ARRAY_BUFFER,uv_surface.size()*sizeof(float),&uv_surface[0],GL_STATIC_DRAW);

        modelMatrix = glm::mat4(1.0f);

        glm::mat4 viewMatrix = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        // viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationX), glm::vec3(1, 0, 0));
        // viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationY), glm::vec3(0, 1, 0));
        // viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationZ), glm::vec3(0, 0, 1));
 
        glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
        
        GLint modelID = glGetUniformLocation(programID, "modelMatrix");
        GLint viewID = glGetUniformLocation(programID, "viewMatrix");
        GLint projectionID = glGetUniformLocation(programID, "projectionMatrix");

        glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);

        //Textures
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D,TextureHmap);
        // glUniform1i(TextureIDHmap,0);

        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D,TextureGrass);
        // glUniform1i(TextureIDGrass,1);

        // glActiveTexture(GL_TEXTURE2);
        // glBindTexture(GL_TEXTURE_2D,TextureRock);
        // glUniform1i(TextureIDRock,2);

        // glActiveTexture(GL_TEXTURE3);
        // glBindTexture(GL_TEXTURE_2D,TextureSnow);
        // glUniform1i(TextureIDSnow,3);

        // glEnableVertexAttribArray(1);
        // glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
        // glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);

        drawSphere(vertexbuffer_Sun, elementbuffer_Sun, sun->indices);

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer_Sun);
    glDeleteBuffers(1, &elementbuffer_Sun);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float cameraSpeed = 500 * deltaTime;
    if (yoffset == -1) {
        camera_position -= cameraSpeed * camera_target;
    }
    if (yoffset == 1) { 
        camera_position += cameraSpeed * camera_target;
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 camera_right = glm::vec3(1.0f, 0.0f,  0.0f);

    //rotationX
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) rotationX += 0.1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)rotationX -= 0.1;

    //rotationY
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationY += .1;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationY -= .1;

    //rotationZ
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rotationZ += .1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rotationZ -= .1;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) displayMode = 0;

    //Camera zoom in and out
    float cameraSpeed = 10 * deltaTime;
    glfwSetScrollCallback(window, scroll_callback);

    //TODO add translations
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_position += glm::normalize(glm::cross(camera_up, camera_target)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_position -= glm::normalize(glm::cross(camera_up, camera_target)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_position += glm::normalize(glm::cross(camera_right, camera_target)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_position -= glm::normalize(glm::cross(camera_right, camera_target)) * cameraSpeed;

    //resolution
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
        planeSize+=0.01;
        generateGeometryPlane(planeSize, indexed_verticesPlane, indicesPlane, trianglesPlane, uv_surface);
    }
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
        if(planeSize > 3) {
            planeSize-=0.01;
            generateGeometryPlane(planeSize, indexed_verticesPlane, indicesPlane, trianglesPlane, uv_surface);
        }
    }

    //change display mode
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

