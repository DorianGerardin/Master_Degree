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
        float s; // scale
        vec3 r;  // rotation
        vec3 t;  // translation
    public:
    	Transform(){s = 1; r = vec3(1.0f); t = vec3(1.0f);} 
        Transform(float s, vec3 r, vec3 t) {s = s; r = r; t = t;}
    	~Transform() {delete this;}

    	vec4 apply(vec4 p);

        vec3 applyToPoint(vec3 p) {
        	//return m * p + t;
        }

        vec3 applyToVector(vec3 v) {
        	//return m * v;
        }

        vec3 applyToVersor(vec3 v);

        Transform combineWith(Transform &t);
        Transform inverse();
        Transform interpolateWith(Transform &t, float k);

};