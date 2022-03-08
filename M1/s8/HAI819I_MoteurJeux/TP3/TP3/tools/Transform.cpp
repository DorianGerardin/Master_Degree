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

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>

class Transform {

	public:
        //Local space information
        vec3 scale; // scale
        vec3 rotation;  // rotation
        vec3 translation;  // translation
        //Global space information concatenate in matrix
        mat4 modelMatrix = mat4(1.0f);
    public:
    	Transform(){scale = vec3(1.0f); rotation = vec3(1.0f); translation = vec3(1.0f);} 
        Transform(vec3 s, vec3 r, vec3 t) {scale = s; rotation = r; translation = t;}
    	~Transform() {delete this;}

        mat4 getMatrix();

        vec3 applyToPoint(vec3 p) {
        	return (rotation * p) * scale + translation;
        }

        vec3 applyToVector(vec3 v);

        mat4 getLocalModelMatrix()
        {
            const mat4 transformX = rotate(mat4(1.0f), radians(rotation.x), vec3(1.0f, 0.0f, 0.0f));

            const mat4 transformY = rotate(mat4(1.0f), radians(rotation.y), vec3(0.0f, 1.0f, 0.0f));

            const mat4 transformZ = rotate(mat4(1.0f), radians(rotation.z), vec3(0.0f, 0.0f, 1.0f));

            // Y * X * Z
            const mat4 rotationMatrix = transformY * transformX * transformZ;

            // translation * rotation * scale (also know as TRS matrix)
            return glm::translate(glm::mat4(1.0f), translation) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
        }

};