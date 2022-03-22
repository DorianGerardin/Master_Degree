// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;
using namespace std;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>
#include <memory>

#include "Object.cpp"

/*vec3 camera_position =vec3(0.0f, 0.0f, 40.0f);
vec3 camera_target = vec3(0.0f, 0.0f, -1.0f);
vec3 camera_up = vec3(0.0f, 1.0f,  0.0f);*/

struct CAMERA {
    vec3 position;
    vec3 target;
    vec3 up;
};

class Scene {

    private:
        CAMERA defaultCamera;

	public:
        vector<unique_ptr<Object>> objects;
        vector<CAMERA> cameras;

    public:

        Scene() {
            defaultCamera.position =vec3(0.0f, 0.0f, 40.0f);
            defaultCamera.target = vec3(0.0f, 0.0f, -1.0f);
            defaultCamera.up = vec3(0.0f, 1.0f,  0.0f);
            this->addCamera(defaultCamera);
        }

        ~Scene() {
            delete this;
        }

        void addObject(unique_ptr<Object> o) {
            this->objects.push_back(move(o));
        }

        void addCamera(CAMERA cam) {
            this->cameras.push_back(cam);
        }
        
        void draw() {
            std::cout << "debut draw scene" << std::endl;
            std::cout << "size object" << objects.size() << std::endl;
            for (int i = 0; i < objects.size(); i++)
            {
                //std::cout << "object uv : " << objects[i]->uv[0] << std::endl;
                std::cout << "object " << i << " : " << std::endl;
                std::cout << "draw scene : " << objects[i]->mesh->indexed_vertices.size() << std::endl;
                objects[i]->draw();
            }
        }
};