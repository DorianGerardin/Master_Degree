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
        Object* parent = nullptr;
        vector<unique_ptr<Object>> children;

    public:
        Object(string filename) {
            loadOFF( filename, this->indexed_vertices, this->indices, this->triangles );
            transform = new Transform();
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

        /*void updatSelf() {
            if (!this->transform->isDirty())
                return;

            forceUpdateSelf();
        }

        void forceUpdateSelf() {
            this->transform->computeSelfModelMatrix();
        }*/

        void updateSelfAndChild()
        {
            if (!this->transform->isDirty())
                return;

            forceUpdateSelfAndChild();
        }

        void forceUpdateSelfAndChild()
        {
            if (parent) {
                this->transform->computeModelMatrix(parent->transform->getModelMatrix());
            }
            else
                this->transform->computeModelMatrix();

            for (auto&& child : children)
            {
                child->forceUpdateSelfAndChild();
            }
        }

        void addChild(unique_ptr<Object> o) {
            this->children.push_back(move(o));
            children.back()->parent = this;
        }

        Object* getParent() {
            return this->parent;
        }

        void update();

        void generateBuffers(GLuint &vertexbuffer, GLuint &elementbuffer) {
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, this->indexed_vertices.size() * sizeof(glm::vec3), &this->indexed_vertices[0], GL_STATIC_DRAW);

            // Generate a buffer for the indices as well
            glGenBuffers(1, &elementbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned short), &this->indices[0] , GL_STATIC_DRAW);
        }

        void draw(GLuint vertexbuffer, GLuint elementbuffer) {

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
                        this->indices.size(),    // count
                        GL_UNSIGNED_SHORT, // type
                        (void*)0           // element array buffer offset
                        );
        }

        void applyTexture(GLuint indexUV, GLuint indexTexture, GLuint texture, GLuint TextureID, GLuint &uvbuffer, GLuint programID) {

            glGenBuffers(indexUV,&uvbuffer);
            glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
            glBufferData(GL_ARRAY_BUFFER,this->uv.size()*sizeof(float),&this->uv[0],GL_STATIC_DRAW);

            glActiveTexture(GL_TEXTURE0 + indexTexture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(TextureID,indexTexture);

            glEnableVertexAttribArray(indexUV);
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
            glVertexAttribPointer(indexUV,2,GL_FLOAT,GL_FALSE,0,(void*)0);



        }

    	

};