#version 330 core

// Ouput data
//in vec3 ourColor;
in vec2 UV;

//out vec4 Color;
out vec3 color;

uniform sampler2D textureSampler;

void main(){

        //Color = texture(tex, Texcoord);
        color = texture(textureSampler, UV).rgb;

}
