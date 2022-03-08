#version 330 core

// Ouput data
//in vec3 ourColor;
in vec2 UV;
in float zPos;

//out vec4 Color;
out vec3 color;

uniform sampler2D hmapSampler;
uniform sampler2D grassSampler;
uniform sampler2D rockSampler;
uniform sampler2D snowSampler;


void main(){
        color = vec3(255., 0.5, 0.);
}
