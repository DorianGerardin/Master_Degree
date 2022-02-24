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
        mat3 s; // scale
        mat3 r;  // rotation
        vec3 t;  // translation
    public:
    	Transform(){s = mat3(1.0f); r = mat3(1.0f); t = vec3(1.0f);} 
        Transform(mat3 s, mat3 r, vec3 t) {s = s; r = r; t = t;}
    	~Transform() {delete this;}

        mat4 getMatrix();

        vec3 applyToPoint(vec3 p) {
        	return (r * p) * s + t;
        }

        vec3 applyToVector(vec3 v);

        vec3 applyToVersor(vec3 v);

        Transform interpolateWith(Transform t, float k) {
            /*Transform result;
            result.s = this.s * k + t.s * (1-k);
            result.r = this.r.mixWith*/
        }

        Transform combineWith(Transform t);
        Transform inverse();

};