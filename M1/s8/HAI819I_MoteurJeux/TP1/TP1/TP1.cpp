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

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position   = glm::vec3(0.0f, 0.0f,  3.0f);
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
/*******************************************************************************/


void generateGeometryPlane(int size, std::vector<glm::vec3> & indexed_vertices,
                                     std::vector<unsigned short> & indices, 
                                     std::vector<std::vector<unsigned short>> & triangles,
                                     std::vector<float>& uv) {

    indexed_vertices.clear();
    indices.clear();
    triangles.clear();

    int minZ = -1;
    int maxZ = 1;

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {

            float z = (minZ + maxZ) + (((float) rand()) / (float) RAND_MAX) * (maxZ - (minZ + maxZ));   
            glm::vec3 vertex = glm::vec3((float)i-size/2, (float)j-size/2, -20+z);
            indexed_vertices.push_back(vertex);
            uv.push_back((float)i/(size-1));
            uv.push_back((float)j/(size-1));
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

void drawPlane(GLuint vertexbufferPlane, GLuint elementbufferPlane, std::vector<unsigned short> indicesPlane) {

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPlane);
    glVertexAttribPointer(
                0,                  // attribute
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferPlane);

    // Draw the triangles !
    glDrawElements(
                GL_TRIANGLES,      // mode
                indicesPlane.size(),    // count
                GL_UNSIGNED_SHORT,   // type
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
    window = glfwCreateWindow( 1024, 768, "TP1 - GLFW", NULL, NULL);
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
    std::vector<unsigned short> indices; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<glm::vec3> indexed_vertices;
    std::string filename("suzanne.off");
    //loadOFF(filename, indexed_vertices, indices, triangles );

    // Load it into a VBO

   /* GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);*/

    //Generate a buffer for the indices as well
    /*GLuint elementbuffer;
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);*/


    //Plan
    std::vector<unsigned short> indicesPlane; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > trianglesPlane;
    std::vector<glm::vec3> indexed_verticesPlane;
    std::vector<float> uv_surface;
    generateGeometryPlane(16, indexed_verticesPlane, indicesPlane, trianglesPlane, uv_surface);

    GLuint vertexbufferPlane;
    glGenBuffers(1, &vertexbufferPlane);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPlane);
    glBufferData(GL_ARRAY_BUFFER, indexed_verticesPlane.size() * sizeof(glm::vec3), &indexed_verticesPlane[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    GLuint elementbufferPlane;
    glGenBuffers(1, &elementbufferPlane);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferPlane);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesPlane.size() * sizeof(unsigned short), &indicesPlane[0] , GL_STATIC_DRAW);

    //Texture
    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint Texture = loadBMP_custom("puech.bmp");
    GLuint TextureID = glGetUniformLocation(programID,"textureSampler");
    GLuint uvbuffer;
    glGenBuffers(1,&uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
    glBufferData(GL_ARRAY_BUFFER,uv_surface.size()*sizeof(float),&uv_surface[0],GL_STATIC_DRAW);
    
    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");



    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

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


        //Dessiner le plan

        glm::vec3 barycentre = getBarycentre(indexed_verticesPlane);
        glm::vec3 barycentreInverse = glm::vec3(-barycentre[0], -barycentre[1], -barycentre[2]);
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, barycentre);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationX), glm::vec3(1, 0, 0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationY), glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationZ), glm::vec3(0, 0, 1));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(barycentreInverse));
       
        glm::mat4 viewMatrix = glm::lookAt(camera_position, camera_position + camera_target, camera_up);
        glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
        
        GLint modelID = glGetUniformLocation(programID, "modelMatrix");
        GLint viewID = glGetUniformLocation(programID, "viewMatrix");
        GLint projectionID = glGetUniformLocation(programID, "projectionMatrix");

        glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);

        /*glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferPlane);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferPlane);*/

        //Textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,Texture);
        glUniform1i(TextureID,0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);

        /*glDrawElements(
                GL_TRIANGLES,      // mode
                indicesPlane.size(),    // count
                GL_UNSIGNED_SHORT,   // type
                (void*)0           // element array buffer offset
                );*/

        drawPlane(vertexbufferPlane, elementbufferPlane, indicesPlane);


        /*****************TODO***********************/
        // Model matrix : an identity matrix (model will be at the origin) then change
 /*     glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::scale(modelMatrix, vec3(0.5,0.5,0.5));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, -2.5, 0));

        // View matrix : camera/view transformation lookat() utiliser camera_position camera_target camera_up
        glm::mat4 viewMatrix = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        // Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);*/
        // Send our transformation to the currently bound shader,
        // in the "Model View Projection" to the shader uniforms

/*      GLint modelID = glGetUniformLocation(programID, "modelMatrix");
        GLint viewID = glGetUniformLocation(programID, "viewMatrix");
        GLint projectionID = glGetUniformLocation(programID, "projectionMatrix");
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);*/
        /****************************************/

        // 1rst attribute buffer : vertices
        /*glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );*/

        // Index buffer
/*      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

        // Draw the triangles !
        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );*/

        //2eme chaise
/*       modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::scale(modelMatrix, vec3(0.5,0.5,0.5));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, -2.5, 0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);

        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT, // type
                    (void*)0           // element array buffer offset
                    );*/

        //3eme chaise
        /*modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0, 0, 1));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
        glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);

        glDrawElements(
                    GL_TRIANGLES,      // mode
                    indices.size(),    // count
                    GL_UNSIGNED_SHORT, // type
                    (void*)0           // element array buffer offset
                    );*/

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbufferPlane);
    glDeleteBuffers(1, &elementbufferPlane);
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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) rotationX += 3;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)rotationX -= 3;

    //rotationY
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationY += 3;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationY -= 3;

    //rotationZ
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rotationZ += 3;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rotationZ -= 3;

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

