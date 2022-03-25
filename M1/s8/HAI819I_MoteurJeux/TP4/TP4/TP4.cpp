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
//#include "tools/Object.cpp"
#include "tools/InputManager.cpp"

//void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

/*******************************************************************************/

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
    glClearColor(0.73f, 0.95f, 0.98f, 0.0f);

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
    GLuint programID = LoadShaders( "shaders/v_shader.glsl", "shaders/f_shader.glsl" );

    /*****************TODO***********************/
    // Get a handle for our "Model View Projection" matrices uniforms
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    // --------------------------------------------------------------------------------------------
    // |                                      TEXTURES                                            |
    // --------------------------------------------------------------------------------------------

    GLuint earth_texture = loadBMP_custom("textures/earthTexture.bmp");
    GLuint hmap_texture = loadBMP_custom("textures/Heightmap_Mountain.bmp");

    GLuint TextureIDHmap = glGetUniformLocation(programID,"hmapSampler");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hmap_texture);
    glUniform1i(TextureIDHmap,0);

    //GLuint TextureIDHmap = glGetUniformLocation(programID,"hmapSampler");
    GLuint TextureID = glGetUniformLocation(programID,"texSampler");

    // Get a handle for our "LightPosition" uniform
    GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    GLint modelID = glGetUniformLocation(programID, "modelMatrix");

    GLint useHeightMapID = glGetUniformLocation(programID, "useHeightMap");

    GLint uHeightID = glGetUniformLocation(programID, "uHeight");
    GLint vHeightID = glGetUniformLocation(programID, "vHeight");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // --------------------------------------------------------------------------------------------
    // |                                         SCENE                                            |
    // --------------------------------------------------------------------------------------------

    Scene *scene = new Scene();
    InputManager *inputManager = new InputManager(window, scene);
    CAMERA camera = inputManager->scene->cameras[0]; 

    unique_ptr<Object> plane_uniquePtr = make_unique<Object>("plane", 20., hmap_texture, TextureID, modelID, programID);
    Object* terrain = plane_uniquePtr.get();
    terrain->useHeightMap = GL_TRUE;
    
    unique_ptr<Object> ball_uniquePtr = make_unique<Object>("sphere", 50., earth_texture, TextureID, modelID, programID);
    Object* ball = ball_uniquePtr.get();

    terrain->transform->setLocalRotation(vec3( 105., 0., 0. ));

    scene->addObject(terrain);
    scene->addObject(ball);

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        inputManager->processInput(deltaTime);
        CAMERA camera = inputManager->scene->cameras[0];

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        //glUseProgram(programID);

        terrain->updateSelfAndChild();
        ball->updateSelfAndChild();

        vec3 ballPosition = ball->transform->getLocalTranslation();
        vector<vec3> terrainVertices = terrain->mesh->indexed_vertices;
        int nbTerrainVertices = terrainVertices.size();
        int sizeTerrain = terrain->mesh->size;
        float u, v;
        for (int i = 0; i < sizeTerrain-1; ++i)
        {   
            if(terrainVertices[i*sizeTerrain][0] <= ballPosition[0] && terrainVertices[(i+1)*sizeTerrain][0] >= ballPosition[0]) {
                float diffBallPositionI = abs(ballPosition[0] - terrainVertices[i*sizeTerrain][0]);
                float diffVerticesI = abs(terrainVertices[i][0] - terrainVertices[(i+1)*sizeTerrain][0]);
                float ratioI = diffBallPositionI/diffVerticesI;
                float indiceI = i + ratioI;
                u = (float)indiceI/(float)(sizeTerrain-1);
            }
            for (int j = 0; j < sizeTerrain-1; ++j)
            {   
                if(terrainVertices[i*sizeTerrain+j][1] <= ballPosition[2] && terrainVertices[i*sizeTerrain+j+1][1] >= ballPosition[2]) {
                    float diffBallPosition = abs(ballPosition[2] - terrainVertices[i*sizeTerrain+j][1]);
                    float diffVertices = abs(terrainVertices[i*sizeTerrain+j][1] - terrainVertices[i*sizeTerrain+j+1][1]);
                    float ratio = diffBallPosition/diffVertices;
                    float indiceJ = j + ratio;
                    v = (float)indiceJ/(float)(sizeTerrain-1);
                }
            }
        }
        std::cout << "u : " << u << std::endl;
        std::cout << "v : " << v << std::endl;
        //vec2 uvHeight = vec2(u,v);
        glUniform1f(uHeightID, u);
        glUniform1f(vHeightID, v);
        //glUniformMatrix2fv(uvHeightID, 1, GL_FALSE, &uvHeight[0]);

        //-------------------------------------------------------------------------------------------------

        glm::mat4 viewMatrix = glm::lookAt(camera.position, camera.position + camera.target, camera.up);

        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationX), glm::vec3(1, 0, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationY), glm::vec3(0, 1, 0));
        viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationZ), glm::vec3(0, 0, 1));
 
        glm::mat4 projectionMatrix = glm::perspective<float>(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 200.f);

        GLint viewID = glGetUniformLocation(programID, "viewMatrix");
        GLint projectionID = glGetUniformLocation(programID, "projectionMatrix");

        //glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelMatrix[0][0]);
        glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);

        scene->draw();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

