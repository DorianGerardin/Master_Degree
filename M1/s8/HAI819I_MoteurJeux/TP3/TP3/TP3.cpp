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

float sunRotation = 20.;
float earthRotation = 20.;

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

void generateSphere(float size, std::vector<glm::vec3> & indexed_vertices,
                                     std::vector<unsigned short> & indices, 
                                     std::vector<std::vector<unsigned short>> & triangles,
                                     std::vector<float>& uv) {

    indexed_vertices.clear();
    indices.clear();
    triangles.clear();
    uv.clear();

    double TWO_PI = M_PI*2;

    float PI = M_PI;
    float half_PI = M_PI/2;
    float teta = 0.0;
    float phi = -half_PI;

    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < size; i++){
            /*if(i == size) {
                teta = 0.;
            }*/
            float x = cos(teta)*cos(phi);
            float y = sin(teta)*cos(phi);
            float z = sin(phi);
            teta += 2*PI/size;
            indexed_vertices.push_back(vec3(x,y,z));
            uv.push_back((float)i/(float)(size-1));
            uv.push_back((float)j/(float)(size-1));
        }
        teta = 0.0;
        phi += PI/(size-1);
    }

    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            std::vector<unsigned short> triangle;
            triangle.push_back(i*size+j);
            triangle.push_back(i*size+(j+1));
            triangle.push_back((i+1)*size+j);
            triangles.push_back(triangle);

            if(j == size-1) {
                indices.push_back(i*size+j);
                indices.push_back((i+1)*size+j);
                indices.push_back(i*size);

                indices.push_back(i*size);
                indices.push_back((i+1)*size+j);
                indices.push_back((i+1)*size);
            } else {
                indices.push_back(i*size+j);
                indices.push_back((i+1)*size+j);
                indices.push_back(i*size+(j+1));

                indices.push_back(i*size+(j+1));
                indices.push_back((i+1)*size+j);
                indices.push_back((i+1)*size+(j+1));
            }
        }
    }

    /*for (int i = 0; i < uv.size(); ++i)
    {
        std::cout << "uv[" << i << "] : " << uv[i] << std::endl;
    }

    for (int i = 0; i < indexed_vertices.size(); ++i)
    {
        std::cout << "vertex[" << i << "] : " << indexed_vertices[i][0] << " " << indexed_vertices[i][1] << " " << indexed_vertices[i][2] << std::endl;
    }*/
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
    glClearColor(0.f, 0.f, 0.f, 0.0f);

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

    // --------------------------------------------------------------------------------------------
    // |                                      TEXTURES                                            |
    // --------------------------------------------------------------------------------------------

    GLuint sun_texture = loadBMP_custom("textures/sunTexture.bmp");
    GLuint earth_texture = loadBMP_custom("textures/earthTexture.bmp");

    GLuint TextureID = glGetUniformLocation(programID,"texSampler");

    /*glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,sun_texture);
    glUniform1i(TextureID,0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,earth_texture);
    glUniform1i(TextureID,1);*/

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // --------------------------------------------------------------------------------------------
    // |                                      SUN MODEL                                           |
    // --------------------------------------------------------------------------------------------
    std::vector<unsigned short> indices_Sun; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles_Sun;
    std::vector<glm::vec3> indexed_vertices_Sun;
    std::vector<float> uv_Sun;
    GLuint vertexbuffer_Sun;
    GLuint elementbuffer_Sun;
    GLuint uvbuffer_Sun;

    generateSphere(20, 
                   indexed_vertices_Sun,
                   indices_Sun,
                   triangles_Sun,
                   uv_Sun);
    Object *sun = new Object(indexed_vertices_Sun,
                             indices_Sun,
                             triangles_Sun,
                             uv_Sun,
                             nullptr);

    const float scale = 0.5;
    sun->transform->setLocalScale(vec3( 3., 3., 3. ));


    // -------------------------------------------------------------------------------------------------
    // |                                        EARTH MODEL                                            |
    // -------------------------------------------------------------------------------------------------
    std::vector<unsigned short> indices_Earth; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles_Earth;
    std::vector<glm::vec3> indexed_vertices_Earth;
    std::vector<float> uv_Earth;
    GLuint vertexbuffer_Earth;
    GLuint elementbuffer_Earth;
    GLuint uvbuffer_Earth;
    generateSphere(20, 
                   indexed_vertices_Earth,
                   indices_Earth,
                   triangles_Earth,
                   uv_Earth);

    unique_ptr<Object> earth_uniquePtr = make_unique<Object>(indexed_vertices_Earth,
                             indices_Earth,
                             triangles_Earth,
                             uv_Earth,
                             nullptr);
    Object* earth = earth_uniquePtr.get();
    sun->addChild(move(earth_uniquePtr));
    earth->transform->setLocalTranslation(vec3(5, 0., 0.));
    earth->transform->setLocalScale(vec3( 0.5, 0.5, 0.5 ));

    // -------------------------------------------------------------------------------------------------
    // |                                        MOON MODEL                                             |
    // -------------------------------------------------------------------------------------------------
    std::vector<unsigned short> indices_Moon; //Triangles concaténés dans une liste
    std::vector<std::vector<unsigned short> > triangles_Moon;
    std::vector<glm::vec3> indexed_vertices_Moon;
    std::vector<float> uv_Moon;
    GLuint vertexbuffer_Moon;
    GLuint elementbuffer_Moon;
    GLuint uvbuffer_Moon;
    generateSphere(20, 
                   indexed_vertices_Moon,
                   indices_Moon,
                   triangles_Moon,
                   uv_Moon);

    unique_ptr<Object> moon_uniquePtr = make_unique<Object>(indexed_vertices_Moon,
                             indices_Moon,
                             triangles_Moon,
                             uv_Moon,
                             nullptr);
    Object* moon = moon_uniquePtr.get();
    earth->addChild(move(moon_uniquePtr));
    moon->transform->setLocalTranslation(vec3(1, 0., 0.));
    moon->transform->setLocalScale(vec3( 0.3, 0.3, 0.3 ));

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


        // ----------------------
        // |     SUN MODEL      |
        // ----------------------

        sunRotation += 20. * deltaTime;
        earthRotation += 60. * deltaTime;

        GLint modelID = glGetUniformLocation(programID, "modelMatrix");
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &sun->transform->getModelMatrix()[0][0]);

        sun->generateBuffers(vertexbuffer_Sun, elementbuffer_Sun);
        sun->applyTexture(1, 0, sun_texture, TextureID, uvbuffer_Sun, programID);
        sun->draw(vertexbuffer_Sun, elementbuffer_Sun);

        sun->transform->setLocalRotation(vec3(90., sunRotation, 0.));

        modelID = glGetUniformLocation(programID, "modelMatrix");
        glUniformMatrix4fv(modelID, 1, GL_FALSE, &earth->transform->getModelMatrix()[0][0]);

        earth->generateBuffers(vertexbuffer_Earth, elementbuffer_Earth);
        earth->applyTexture(1, 0, earth_texture, TextureID, uvbuffer_Earth, programID);
        earth->draw(vertexbuffer_Earth, elementbuffer_Earth);

        earth->transform->setLocalRotation(vec3(180., 0., 0.));

        sun->updateSelfAndChild();

        //-----------------------

        //modelMatrix = glm::mat4(1.0f);

        glm::mat4 viewMatrix = glm::lookAt(camera_position, camera_position + camera_target, camera_up);

        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationX), glm::vec3(1, 0, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationY), glm::vec3(0, 1, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationZ), glm::vec3(0, 0, 1));
 
        glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
        
        //GLint modelID = glGetUniformLocation(programID, "modelMatrix");
        GLint viewID = glGetUniformLocation(programID, "viewMatrix");
        GLint projectionID = glGetUniformLocation(programID, "projectionMatrix");

        //glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
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
    float cameraSpeed = 100 * deltaTime;
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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) rotationX += 1;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)rotationX -= 1;

    //rotationY
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) rotationY += 1;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) rotationY -= 1;

    //rotationZ
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rotationZ += 1;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rotationZ -= 1;

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

