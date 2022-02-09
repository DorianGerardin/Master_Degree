#version 330 core

// Ouput data
//in vec3 ourColor;
in vec2 TexCoord;

//out vec4 Color;
out vec3 color;

uniform sampler2D tex;

void main(){

        //Color = texture(tex, Texcoord);
        color =vec3(0.2, 0.2,0.4);

}
