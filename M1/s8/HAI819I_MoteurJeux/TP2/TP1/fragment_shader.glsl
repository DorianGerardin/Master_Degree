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

        // grass
        if(zPos > 4) {
                color = texture(grassSampler, UV).rgb;
        } else if(zPos > 2.5 && zPos < 4) {
                color = texture(rockSampler, UV).rgb;
        } else {
                color = texture(snowSampler, UV).rgb;
        }
        //color = texture(hmapSampler, UV).rgb;

}
