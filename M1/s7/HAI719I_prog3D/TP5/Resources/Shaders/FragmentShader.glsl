#version 450 core // Minimal GL version support expected from the GPU
#define NB_LIGHTS 3 

struct LightSource {
    vec3 position;
    vec3 color;
    float intensity;
    int isActive;
    mat4 depthMVP;
};
int numberOfLights = NB_LIGHTS;


uniform LightSource lightSources[NB_LIGHTS];




struct Material {
    vec3 albedo;
    float shininess;
};

uniform Material material;



in vec3 fPosition; // Shader input, linearly interpolated by default from the previous stage (here the vertex shader)
in vec3 fPositionWorldSpace;
in vec3 fNormal;
in vec2 fTexCoord;
uniform sampler2D shadowMap[NB_LIGHTS];

out vec4 colorResponse; // Shader output: the color response attached to this fragment


uniform mat4 projectionMat, modelViewMat, normalMat;



float pi = 3.1415927;


float shadowCalculation(int i, vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap[i], projCoords.xy).r;
    float currentDepth = projCoords.z;
    float shadow = currentDepth < closestDepth + 0.001 ? 1.0 : 0.0;
    return shadow;
}


void main() {
    vec3 n = normalize(fNormal);

    // Linear barycentric interpolation does not preserve unit vectors
    vec3 wo = normalize (-fPosition); // unit vector pointing to the camera
    vec3 radiance = vec3(0,0,0);

    if( dot( n , wo ) >= 0.0 ) {
        {
            for (int i = 0; i < numberOfLights; ++i)
            {
                if( lightSources[i].isActive == 1 ) { // WE ONLY CONSIDER LIGHTS THAT ARE SWITCHED ON
                     {
                        vec3 wi = normalize ( vec3((modelViewMat * vec4(lightSources[i].position,1)).xyz) - fPosition ); // unit vector pointing to the light source (change if you use several light sources!!!)
                        if( dot( wi , n ) >= 0.0 ) { // WE ONLY CONSIDER LIGHTS THAT ARE ON THE RIGHT HEMISPHERE (side of the tangent plane)
                            vec3 wh = normalize( wi + wo ); // half vector (if wi changes, wo should change as well)
                            vec3 Li = lightSources[i].color * lightSources[i].intensity;
                            vec4 shadowCoords = lightSources[i].depthMVP * vec4(fPositionWorldSpace, 1.0);
                            float shadow = shadowCalculation(i, shadowCoords);
                            radiance = radiance +
                                    Li // light color
                                    * shadow 
                                    * material.albedo
                                    * ( max(dot(n,wi),0.0) + pow(max(dot(n,wh),0.0),material.shininess) )
                                    ;
                        }
                    }
                }
            }
        }
    }

    colorResponse = vec4 (radiance, 1.0); // Building an RGBA value from an RGB one.
}




