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

#include "Transform.cpp"

class Object {

	public:
        vector<glm::vec3> indexed_vertices;
        vector<unsigned short> indices;
        vector<vector<unsigned short>> triangles;
        vector<float> uv;
        Transform *transform;
        Object* parent;
        vector<Object*> children;
    public:
        Object(string filename, vector<float> uv) {
            loadOFF( filename, this->indexed_vertices, this->indices, this->triangles );
            transform = new Transform();
            this->uv = uv;
        }

        Object(vector<glm::vec3> indexed_vertices, 
               vector<unsigned short> indices,
               vector<vector<unsigned short>> triangles,
               vector<float> uv,
               Object* parent) {
            transform = new Transform();
            this->indexed_vertices = indexed_vertices;
            this->indices = indices;
            this->triangles = triangles;
            this->uv = uv;
            this->parent = parent;
        }

        ~Object() {
            delete this;
        }

        void addChild(Object* o) {
            this->children.push_back(o);
        }

        Object* getParent() {
            return this->parent;
        }

        void update() {

        }

        void draw() {

        }

    	

};