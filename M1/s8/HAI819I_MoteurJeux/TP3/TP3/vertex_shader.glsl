#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;
layout(location = 1) in vec2 vertexUV;
//layout (location = 2) in vec2 aTexCoord;

//TODO create uniform transformations matrices Model View Projection
// Values that stay constant for the whole mesh.

//uniform mat4 transfo_mat;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform sampler2D hmapSampler;
uniform sampler2D grassSampler;
uniform sampler2D rockSampler;
uniform sampler2D snowSampler;

out vec2 UV;
out float zPos;
//out vec2 TexCoord;

void main(){

        vec4 position = vec4(vertices_position_modelspace,1);
        position.z = 5*texture(hmapSampler, vertexUV).r;

        gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
        // TODO : Output position of the vertex, in clip space : MVP * position
        UV = vertexUV;
        zPos = position.z;
}

