#version 330 core

// Ouput data
//in vec3 ourColor;
in vec2 UV;
in float zPos;

//out vec4 Color;
out vec3 color;


uniform sampler2D texSampler;
uniform sampler2D hmapSampler;

void main(){
        color = texture( texSampler, UV ).rgb;
}
