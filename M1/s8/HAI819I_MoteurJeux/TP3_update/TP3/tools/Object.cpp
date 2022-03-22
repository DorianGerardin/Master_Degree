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
#include "Mesh.cpp"

class Object {

	public:
        Mesh *mesh;
        Transform *transform;
        Object* parent = nullptr;
        vector<unique_ptr<Object>> children;

    public:
        Object(string filename) {
            loadOFF( filename, this->mesh->indexed_vertices, this->mesh->indices);
            transform = new Transform();
        }

        Object(string meshType, float size, GLuint tex, GLuint texID, GLint modelID) {
            this->transform = new Transform();
            this->mesh = new Mesh(meshType, size, tex, texID, modelID);
            std::cout << "constructeur mesh : " << this->mesh->indexed_vertices.size() << std::endl;
            this->parent = nullptr;
        }

        ~Object() {
            delete this;
        }

        void updateSelf() {
            if (!this->transform->isDirty())
                return;

            forceUpdateSelf();
        }

        void forceUpdateSelf() {
            this->transform->computeSelfModelMatrix();
        }

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


        void applyTexture(GLuint texture, GLuint TextureID) {

            GLuint uvbuffer;

            glGenBuffers(1,&uvbuffer);
            glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
            glBufferData(GL_ARRAY_BUFFER,this->mesh->uv.size()*sizeof(float),&this->mesh->uv[0],GL_STATIC_DRAW);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(TextureID,0);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
            glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);

        }

        void draw() {

            std::cout << "je draw : " << std::endl;
            std::cout << "size vertices: " << this->mesh->indexed_vertices.size() << std::endl;

            GLuint vertexbuffer;
            GLuint elementbuffer;
            GLuint uvbuffer;

            std::cout << "instancie buffers: " << std::endl;

            glUniformMatrix4fv(this->mesh->modelID, 1, GL_FALSE, &this->transform->getModelMatrix()[0][0]);

            std::cout << "apres uniform" << std::endl;

            // BUFFERS
            glGenBuffers(1, &vertexbuffer);
            std::cout << "genBuffers" << std::endl;
            std::cout << "size vertices: " << this->mesh->indexed_vertices.size() << std::endl;
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            std::cout << "size vertices: " << this->mesh->indexed_vertices.size() << std::endl;
            glBufferData(GL_ARRAY_BUFFER, this->mesh->indexed_vertices.size() * sizeof(glm::vec3), &this->mesh->indexed_vertices[0], GL_STATIC_DRAW);

            // Generate a buffer for the indices as well
            glGenBuffers(1, &elementbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
            std::cout << "size indices: " << this->mesh->indices.size() << std::endl;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->indices.size() * sizeof(unsigned short), &this->mesh->indices[0] , GL_STATIC_DRAW);

            std::cout << "buffer done: " << std::endl;

            glGenBuffers(1,&uvbuffer);
            glBindBuffer(GL_ARRAY_BUFFER,uvbuffer);
            glBufferData(GL_ARRAY_BUFFER,this->mesh->uv.size()*sizeof(float),&this->mesh->uv[0],GL_STATIC_DRAW);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, this->mesh->texture);
            glUniform1i(this->mesh->textureID,0);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
            glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(void*)0);

            std::cout << "fin texture " << std::endl;

            //DRAW
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
                        this->mesh->indices.size(),    // count
                        GL_UNSIGNED_SHORT, // type
                        (void*)0           // element array buffer offset
                        );

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
/*
            glDeleteBuffers(1, &vertexbuffer);
            glDeleteBuffers(1, &elementbuffer);*/

            std::cout << "je finis de draw" << std::endl;
        }

        void generateBuffers(GLuint &vertexbuffer, GLuint &elementbuffer) {
            glGenBuffers(1, &vertexbuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
            glBufferData(GL_ARRAY_BUFFER, this->mesh->indexed_vertices.size() * sizeof(glm::vec3), &this->mesh->indexed_vertices[0], GL_STATIC_DRAW);

            // Generate a buffer for the indices as well
            glGenBuffers(1, &elementbuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mesh->indices.size() * sizeof(unsigned short), &this->mesh->indices[0] , GL_STATIC_DRAW);
        }

        void draw2(GLuint vertexbuffer, GLuint elementbuffer) {

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
                        this->mesh->indices.size(),    // count
                        GL_UNSIGNED_SHORT, // type
                        (void*)0           // element array buffer offset
                        );
        }

};