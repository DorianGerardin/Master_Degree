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

        if (zPos < 1){
                color = texture( snowSampler, UV ).rgb;
        }
        else if (zPos < 2.5){
                float smoothness = smoothstep(1, 2.5, zPos);
                color = mix(texture( snowSampler, UV ).rgb, texture( rockSampler, UV ).rgb, smoothness);
        }
        else if (zPos < 4.5){
                float smoothness = smoothstep(2.5, 4.5, zPos);
                color = mix(texture( rockSampler, UV ).rgb, texture( grassSampler, UV ).rgb, smoothness);
        }
        else  {
                color = texture( grassSampler, UV ).rgb;
        }
        //color = texture( hmapSampler, UV ).rgb;

}
