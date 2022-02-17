#version 330 core

// Ouput data
//in vec3 ourColor;
in vec2 UV;
in vec4 position;

//out vec4 Color;
out vec3 color;

uniform sampler2D hmapSampler;
uniform sampler2D grassTextureSampler;
uniform sampler2D rockTextureSampler;
uniform sampler2D snowTextureSampler;


void main(){

        //Color = texture(tex, Texcoord);
        color = texture(hmapSampler, UV).rgb;

}
