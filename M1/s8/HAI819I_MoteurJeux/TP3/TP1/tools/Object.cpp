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

#include "Transform.cpp"

class Object {

	public:
        vector<glm::vec3> indexed_vertices;
        vector<unsigned short> indices;
        std::vector<std::vector<unsigned short>> triangles;
        vector<float> uv;
        Transform *transform;
    public:
        Object(string filename, 
              vector<glm::vec3> indexed_vertices, 
              vector<unsigned short> indices,
              std::vector<std::vector<unsigned short>> triangles) {
            loadOFF( filename, indexed_vertices, indices, triangles );
            transform = new Transform();
        }

        Object(vector<glm::vec3> indexed_vertices, 
               vector<unsigned short> indices,
               vector<float> uv) {
            transform = new Transform();
        }

        ~Object() {
            delete this;
        }

    	

};