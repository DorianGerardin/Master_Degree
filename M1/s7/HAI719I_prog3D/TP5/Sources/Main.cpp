// ----------------------------------------------
// Base code for practical computer graphics 
// assignments.
//
// Copyright (C) 2018 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------


// http://www.opengl-tutorial.org/fr/intermediate-tutorials/tutorial-16-shadow-mapping/



#define _USE_MATH_DEFINES

#include <glad/glad.h>

#include <cstdlib>
#include <cstdio>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <ios>
#include <vector>
#include <string>
#include <cmath>
#include <memory>
#include <algorithm>
#include <exception>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Error.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

static const std::string SHADER_PATH ("Resources/Shaders/");
static const std::string DEFAULT_MESH_FILENAME ("Resources/Models/dragon.off");

// Window parameters
static GLFWwindow * windowPtr = nullptr;
unsigned int window_width;
unsigned int window_height;

// Pointer to the current camera model
static std::shared_ptr<Camera> cameraPtr;



// Camera control variables
static float meshScale = 1.0; // To update based on the mesh size, so that navigation runs at scale
static bool isRotating (false);
static bool isPanning (false);
static bool isZooming (false);
static double baseX (0.0), baseY (0.0);
static glm::vec3 baseTrans (0.0);
static glm::vec3 baseRot (0.0);

void clear ();



// timer:
float app_timer = 0.0;
float app_timer_last_colckTime;
bool app_timer_is_stopped = true;



// textures:
unsigned int texture_slot_available = 0;




namespace Texture {
static
GLuint loadTextureFromFileToGPU (const std::string & filename) {
    int width, height, numComponents;
    // Loading the image in CPU memory using stbd_image
    unsigned char * data = stbi_load (filename.c_str (),
                                      &width,
                                      &height,
                                      &numComponents, // 1 for a 8 bit greyscale image, 3 for 24bits RGB image, 4 for 32bits RGBA image
                                      0);
    // Create a texture in GPU memory
    GLuint texID;
    glGenTextures (1, &texID);
    glBindTexture (GL_TEXTURE_2D, texID);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Uploading the image data to GPU memory
    glTexImage2D (GL_TEXTURE_2D,
                  0,
                  (numComponents == 1 ? GL_RED : numComponents == 3 ? GL_RGB : GL_RGBA), // For greyscale images, we store them in the RED channel
                  width,
                  height,
                  0,
                  (numComponents == 1 ? GL_RED : numComponents == 3 ? GL_RGB : GL_RGBA), // For greyscale images, we store them in the RED channel
                  GL_UNSIGNED_BYTE,
                  data);
    // Generating mipmaps for filtered texture fetch
    glGenerateMipmap(GL_TEXTURE_2D);
    // Freeing the now useless CPU memory
    stbi_image_free(data);
    glBindTexture (GL_TEXTURE_2D, 0); // unbind the texture
    return texID;
}
}



struct FBO_ShadowMap {
    GLuint depthMapFBO;
    GLuint depthMapTexture;
    unsigned int depthMapTextureWidth;
    unsigned int depthMapTextureHeight;

    bool allocate( unsigned int width = 1024 , unsigned int height = 1024 ) {
        glGenFramebuffers(1, &depthMapFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

        depthMapTextureWidth = width;
        depthMapTextureHeight = height;

        // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
        glGenTextures(1, &depthMapTexture);
        glBindTexture(GL_TEXTURE_2D, depthMapTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapTexture, 0);

        glDrawBuffer(GL_NONE); // No color buffer is drawn to.

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return true;
        }
        else{
            std::cout << "PROBLEM IN FBO FBO_ShadowMap::allocate() : FBO NOT successfully created" << std::endl;
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return false;
        }
    }
    void free() {
        glDeleteFramebuffers(1, &depthMapFBO);
    }
public:

    FBO_ShadowMap( unsigned int w = 1024 , unsigned int h = 1024) {
        allocate( w , h );
    }
    ~FBO_ShadowMap() { free(); }

    void bindFBO() {
        glViewport(0, 0, depthMapTextureWidth, depthMapTextureHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        // you can now render the geometry, assuming you have set the view matrix according to the light viewpoint
    }

    // The following function is static, why
    // Because fundamentally, it's not really that we "unbind an FBO" as much as "we bind the screen again".
    // In most scenarios, we have several shadow maps (because there are several lights), and only one screen.
    // The "bindScreen" function should not be applied to a specific object, but to the concept of the class itself
    static
    void bindScreen() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void save_PPM_file(std::string const & filename ) {
        std::ofstream output_image(filename.c_str());

        /// READ THE CONTENT FROM THE FBO
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        float * pixels = new float [ depthMapTextureWidth * depthMapTextureHeight ];
        glReadPixels(0, 0, depthMapTextureWidth, depthMapTextureHeight, GL_DEPTH_COMPONENT , GL_FLOAT, pixels);

        output_image << "P3" << std::endl;
        output_image << depthMapTextureWidth << " " << depthMapTextureHeight << std::endl;
        output_image << "255" << std::endl;

        int k = 0;
        for(int i=0; i<depthMapTextureWidth; i++) {
            for(int j=0; j<depthMapTextureHeight; j++) {
                output_image << (unsigned int)(255*pixels[k]) << " " << (unsigned int)(255*pixels[k]) << " " << (unsigned int)(255*pixels[k]) << " ";
                k = k+1;
            }
            output_image << std::endl;
        }
        delete pixels;
        output_image.close();
    }
};





struct Light {
    FBO_ShadowMap * shadowMap;
    glm::mat4 depthMVP;
    unsigned int shadowMapTexOnGPU;

    glm::vec3 m_position ;
    glm::vec3 m_color;
    float m_intensity;

    Light() : shadowMap(NULL) {}

    void setupCameraForShadowMapping(glm::vec3 scene_center , float scene_radius) {
        // Compute depthMVP : the MVP matrix from the light's point of view
        glm::mat4 modelMatrix = glm::mat4(1.0);
        glm::mat4 viewMatrix = glm::lookAt(m_position, scene_center, glm::vec3(0., -1., 0.));
        glm::mat4 projectionMatrix = glm::perspective<float>(scene_radius, (float)window_width / (float)window_height, 1.0f, 150.f); 
        depthMVP = projectionMatrix * viewMatrix * modelMatrix;
    }

    void allocateShadowMapFBO( unsigned int w = 800 , unsigned int h = 600) {
        shadowMap = new FBO_ShadowMap( w , h );
    }
    bool bindShadowMap() {
        if( shadowMap == NULL ) return false;
        // else:
        shadowMap->bindFBO();
        return true;
    }
};





struct Scene{
    // lights :
    std::vector< Light > scene_lights;

    // meshes :
    std::shared_ptr<Mesh> meshPtr;
    std::shared_ptr<Mesh> planeMeshPtr;

    // scene dimensions :
    glm::vec3 scene_center;
    float scene_radius;

    // a shader to render the scene on screen :
    std::shared_ptr<ShaderProgram> shaderProgramPtr;

    // a shader for the rendering of the shadow maps :
    std::shared_ptr<ShaderProgram> shadomMapShaderProgramPtr;

    // useful for debug:
    bool save_shadow_maps_in_ppm = false;

    void render() {
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // first, render the shadow maps:
        shadomMapShaderProgramPtr->use();
        for( int i = 0 ; i < scene_lights.size() ; ++i ) {
            Light & light = scene_lights[i];
            light.setupCameraForShadowMapping( scene_center , scene_radius );

            // Send our transformation to the currently bound shader, in the "MVP" uniform:
            shadomMapShaderProgramPtr->set("depthMVP",light.depthMVP);

            light.bindShadowMap();

            // render the objects in the scene:
            meshPtr->render ();
            planeMeshPtr->render ();

            if(save_shadow_maps_in_ppm) {
                light.shadowMap->save_PPM_file(std::string("shadom_map_")+std::to_string(i)+std::string(".ppm"));
            }
        }
        shadomMapShaderProgramPtr->stop();
        save_shadow_maps_in_ppm = false;
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // then, render the screen:
        FBO_ShadowMap::bindScreen();
        glViewport(0, 0, window_width, window_height);
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.


        shaderProgramPtr->use();


        // Set the camera parameters :
        glm::mat4 projectionMatrix = cameraPtr->computeProjectionMatrix ();
        shaderProgramPtr->set ("projectionMat", projectionMatrix); // Compute the projection matrix of the camera and pass it to the GPU program
        glm::mat4 modelMatrix = meshPtr->computeTransformMatrix ();
        glm::mat4 viewMatrix = cameraPtr->computeViewMatrix ();
        glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
        glm::mat4 normalMatrix = glm::transpose (glm::inverse (modelViewMatrix));
        shaderProgramPtr->set ("modelViewMat", modelViewMatrix);
        shaderProgramPtr->set ("normalMat", normalMatrix);


        // Set the lights in the shader :
        {
            for (int i = 0; i < scene_lights.size(); ++i)
            {
                Light & light = scene_lights[i];
                string position = "lightSources[" + to_string(i) + "].position";
                string color = "lightSources[" + to_string(i) + "].color";
                string intensity = "lightSources[" + to_string(i) + "].intensity";
                string isActive = "lightSources[" + to_string(i) + "].isActive";
                string depthMVP = "lightSources[" + to_string(i) + "].depthMVP";
                string shadowMap = "shadowMap[" + to_string(i) + "]";

                shaderProgramPtr->set (position, light.m_position);
                shaderProgramPtr->set (color, light.m_color);
                shaderProgramPtr->set (intensity, light.m_intensity);
                shaderProgramPtr->set (isActive, 1);
                shaderProgramPtr->set (depthMVP, light.depthMVP);
                shaderProgramPtr->set (shadowMap, (int)light.shadowMapTexOnGPU);
            }
        }

        // Set the material for the plane mesh :
        shaderProgramPtr->set ("material.albedo", glm::vec3( 0.5 , 0.5 , 0.5 ) );
        shaderProgramPtr->set ("material.shininess" , (float) 10.f );

        // render the mesh :
        planeMeshPtr->render ();


        // Set the material for the center mesh :
        shaderProgramPtr->set ("material.albedo", glm::vec3(0.7 , 0.2 , 0.2) );
        shaderProgramPtr->set ("material.shininess" , (float) 60.f );

        meshPtr->render ();


        shaderProgramPtr->stop ();
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
};


Scene scene;


void printHelp () {
    std::cout << "> Help:" << std::endl
              << "    Mouse commands:" << std::endl
              << "    * Left button: rotate camera" << std::endl
              << "    * Middle button: zoom" << std::endl
              << "    * Right button: pan camera" << std::endl
              << "    Keyboard commands:" << std::endl
              << "    * H: print this help" << std::endl
              << "    * ESC: quit the program" << std::endl;
}

// Executed each time the window is resized. Adjust the aspect ratio and the rendering viewport to the current window. 
void windowSizeCallback (GLFWwindow * windowPtr, int width, int height) {
    window_width = width;
    window_height = height;
    cameraPtr->setAspectRatio (static_cast<float>(width) / static_cast<float>(height));
    glViewport (0, 0, (GLint)width, (GLint)height); // Dimension of the rendering region in the window
}

/// Executed each time a key is entered.
void keyCallback (GLFWwindow * windowPtr, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_H) {
        printHelp ();
    }
    else if (action == GLFW_PRESS && key == GLFW_KEY_S) {
        scene.save_shadow_maps_in_ppm = true;
    }
    else if (action == GLFW_PRESS && key == GLFW_KEY_T) {
        app_timer_is_stopped = !app_timer_is_stopped;
        if( !app_timer_is_stopped )
            app_timer_last_colckTime = static_cast<float> (glfwGetTime ());
    }
    else if (action == GLFW_PRESS && key == GLFW_KEY_F1) {
        GLint mode[2];
        glGetIntegerv (GL_POLYGON_MODE, mode);
        glPolygonMode (GL_FRONT_AND_BACK, mode[1] == GL_FILL ? GL_LINE : GL_FILL);
    }
    else if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose (windowPtr, true); // Closes the application if the escape key is pressed
    }
}

/// Called each time the mouse cursor moves
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize (windowPtr, &width, &height);
    float normalizer = static_cast<float> ((width + height)/2);
    float dx = static_cast<float> ((baseX - xpos) / normalizer);
    float dy = static_cast<float> ((ypos - baseY) / normalizer);
    if (isRotating) {
        glm::vec3 dRot (-dy * M_PI, dx * M_PI, 0.0);
        cameraPtr->setRotation (baseRot + dRot);
    }
    else if (isPanning) {
        cameraPtr->setTranslation (baseTrans + meshScale * glm::vec3 (dx, dy, 0.0));
    } else if (isZooming) {
        cameraPtr->setTranslation (baseTrans + meshScale * glm::vec3 (0.0, 0.0, dy));
    }
}

/// Called each time a mouse button is pressed
void mouseButtonCallback (GLFWwindow * window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (!isRotating) {
            isRotating = true;
            glfwGetCursorPos (window, &baseX, &baseY);
            baseRot = cameraPtr->getRotation ();
        }
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        isRotating = false;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (!isPanning) {
            isPanning = true;
            glfwGetCursorPos (window, &baseX, &baseY);
            baseTrans = cameraPtr->getTranslation ();
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        isPanning = false;
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        if (!isZooming) {
            isZooming = true;
            glfwGetCursorPos (window, &baseX, &baseY);
            baseTrans = cameraPtr->getTranslation ();
        }
    } else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
        isZooming = false;
    }
}

void initGLFW () {
    // Initialize GLFW, the library responsible for window management
    if (!glfwInit ()) {
        std::cerr << "ERROR: Failed to init GLFW" << std::endl;
        std::exit (EXIT_FAILURE);
    }

    // Before creating the window, set some option flags
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);

    // Create the window
    window_width = 1024;
    window_height = 768;
    windowPtr = glfwCreateWindow (window_width, window_height, "IGR202 - Practical Assignment", nullptr, nullptr);
    if (!windowPtr) {
        std::cerr << "ERROR: Failed to open window" << std::endl;
        glfwTerminate ();
        std::exit (EXIT_FAILURE);
    }

    // Load the OpenGL context in the GLFW window using GLAD OpenGL wrangler
    glfwMakeContextCurrent (windowPtr);

    /// Connect the callbacks for interactive control
    glfwSetWindowSizeCallback (windowPtr, windowSizeCallback);
    glfwSetKeyCallback (windowPtr, keyCallback);
    glfwSetCursorPosCallback(windowPtr, cursorPosCallback);
    glfwSetMouseButtonCallback (windowPtr, mouseButtonCallback);
}

void exitOnCriticalError (const std::string & message) {
    std::cerr << "> [Critical error]" << message << std::endl;
    std::cerr << "> [Clearing resources]" << std::endl;
    clear ();
    std::cerr << "> [Exit]" << std::endl;
    std::exit (EXIT_FAILURE);
}

void initOpenGL () {
    // Load extensions for modern OpenGL
    if (!gladLoadGLLoader ((GLADloadproc)glfwGetProcAddress))
        exitOnCriticalError ("[Failed to initialize OpenGL context]");

    glEnable (GL_DEBUG_OUTPUT); // Modern error callback functionnality
    glEnable (GL_DEBUG_OUTPUT_SYNCHRONOUS); // For recovering the line where the error occurs, set a debugger breakpoint in DebugMessageCallback
    glDebugMessageCallback (debugMessageCallback, 0); // Specifies the function to call when an error message is generated.
    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer test in the rasterization
    glClearColor (1.0f, 1.0f, 1.0f, 1.0f); // specify the background color, used any time the framebuffer is cleared

    // Loads and compile the programmable shader pipeline
    try {
        scene.shaderProgramPtr = ShaderProgram::genBasicShaderProgram (SHADER_PATH + "VertexShader.glsl",
                                                                       SHADER_PATH + "FragmentShader.glsl");
        scene.shaderProgramPtr->stop();

    } catch (std::exception & e) {
        exitOnCriticalError (std::string ("[Error loading shader program]") + e.what ());
    }
    try {
        scene.shadomMapShaderProgramPtr = ShaderProgram::genBasicShaderProgram (SHADER_PATH + "VertexShader_shadowMap.glsl",
                                                                                SHADER_PATH + "FragmentShader_shadowMap.glsl");
        scene.shadomMapShaderProgramPtr->stop();

    } catch (std::exception & e) {
        exitOnCriticalError (std::string ("[Error loading shader program]") + e.what ());
    }
}



void initScene (const std::string & meshFilename) {
    // Init camera
    int width, height;
    glfwGetWindowSize (windowPtr, &width, &height);
    cameraPtr = std::make_shared<Camera> ();
    cameraPtr->setAspectRatio (static_cast<float>(width) / static_cast<float>(height));

    // Load meshes in the scene:
    {
        scene.meshPtr = std::make_shared<Mesh> ();
        try {
            MeshLoader::loadOFF (meshFilename, scene.meshPtr);
        } catch (std::exception & e) {
            exitOnCriticalError (std::string ("[Error loading mesh]") + e.what ());
        }
        scene.meshPtr->init ();

        scene.planeMeshPtr = std::make_shared<Mesh> ();
        scene.planeMeshPtr->addPlan();
        scene.planeMeshPtr->init ();
    }



    // Load textures:
    {
        // for example:
        // albedoTex = Texture::loadTextureFromFileToGPU("Resources/Materials/Wood/Base_Color.png");
    }


    // Setup lights :
    {
        unsigned int shadow_map_width = 2000 , shadow_map_height = 2000; // play with these parameters
        {
            scene.scene_lights.resize( scene.scene_lights.size() + 3 );
            for (int i = 0; i < scene.scene_lights.size(); ++i)
            {
                Light & newLight = scene.scene_lights[i];
                newLight.m_position = glm::vec3 (-2*cos(app_timer), 2*sin(app_timer), 3.5);
                newLight.m_color = glm::vec3(1.0, 1.0, 1.0);
                newLight.m_intensity = 0.33f;
                newLight.allocateShadowMapFBO(shadow_map_width , shadow_map_height);
                newLight.shadowMapTexOnGPU = texture_slot_available;   ++texture_slot_available;
            }
        }

        for( int i = 0 ; i < scene.scene_lights.size() ; ++i ) {
            Light & light = scene.scene_lights[i];
            glActiveTexture (GL_TEXTURE0 + light.shadowMapTexOnGPU);
            glBindTexture (GL_TEXTURE_2D, light.shadowMap->depthMapTexture);
        }
    }



    // Setup textures on the GPU:
    {
    }



    // Adjust the camera to the actual mesh
    {
        glm::vec3 meshCenter;
        float meshRadius;
        scene.meshPtr->computeBoundingSphere (meshCenter, meshRadius);

        glm::vec3 planeMeshCenter;
        float planeMeshRadius;
        scene.planeMeshPtr->computeBoundingSphere (planeMeshCenter, planeMeshRadius);

        {
            glm::vec3 axis = glm::normalize(planeMeshCenter - meshCenter);
            float s1_min = -meshRadius;
            float s1_max = +meshRadius;
            float s2_min = glm::dot(planeMeshCenter - meshCenter,axis) - planeMeshRadius;
            float s2_max = glm::dot(planeMeshCenter - meshCenter,axis) + planeMeshRadius;
            float s_min = std::min<float>(s1_min , s2_min);
            float s_max = std::max<float>(s1_max , s2_max);
            float s_center = (s_min + s_max)/2.0;
            scene.scene_center = meshCenter + s_center * axis;
            scene.scene_radius = s_max - s_center;
        }
    }
    meshScale = scene.scene_radius;
    cameraPtr->setTranslation (scene.scene_center + glm::vec3 (0.0, 0.0, 3.0 * meshScale));
    cameraPtr->setNear (meshScale / 100.f);
    cameraPtr->setFar (6.f * meshScale);
}

void init (const std::string & meshFilename) {
    initGLFW (); // Windowing system
    initOpenGL (); // OpenGL Context and shader pipeline
    initScene (meshFilename); // Actual scene to render
}

void clear () {
    cameraPtr.reset ();
    scene.meshPtr.reset ();
    scene.planeMeshPtr.reset ();
    scene.shaderProgramPtr.reset ();
    scene.shadomMapShaderProgramPtr.reset ();
    glfwDestroyWindow (windowPtr);
    glfwTerminate ();
}

// The main rendering call
void render () {
    scene.render();
}



// Update any accessible variable based on the current time
void update (float currentTime) {
    if( ! app_timer_is_stopped ) {
        // Animate any entity of the program here
        float dt = currentTime - app_timer_last_colckTime;
        app_timer_last_colckTime = currentTime;
        app_timer += dt;
        // <---- Update here what needs to be animated over time ---->

        Light & light0 = scene.scene_lights[ 0 ];
        light0.m_position = glm::vec3 (-2*cos(app_timer), 2*sin(app_timer), 3.5);
    }
}

void usage (const char * command) {
    std::cerr << "Usage : " << command << " [<file.off>]" << std::endl;
    std::exit (EXIT_FAILURE);
}

int main (int argc, char ** argv) {
    if (argc > 2)
        usage (argv[0]);
    init (argc == 1 ? DEFAULT_MESH_FILENAME : argv[1]); // Your initialization code (user interface, OpenGL states, scene with geometry, material, lights, etc)
    while (!glfwWindowShouldClose (windowPtr)) {
        update (static_cast<float> (glfwGetTime ()));
        render ();
        glfwSwapBuffers (windowPtr);
        glfwPollEvents ();
    }
    clear ();
    std::cout << " > Quit" << std::endl;
    return EXIT_SUCCESS;
}

