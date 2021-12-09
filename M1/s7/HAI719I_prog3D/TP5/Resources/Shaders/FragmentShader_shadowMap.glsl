#version 450 core // Minimal GL version support expected from the GPU

// Ouput data
layout(location = 0) out float fragmentdepth;

void main(){
    fragmentdepth = gl_FragCoord.z;
}
