#version 430


// --------------------------------------------------
// shader definition
// --------------------------------------------------


uniform sampler3D Mask; // déclaration de la map mask

uniform float xCutPosition;
uniform float yCutPosition;
uniform float zCutPosition;

uniform int xCutDirection; 
uniform int yCutDirection;
uniform int zCutDirection;

in vec3 position;
in vec3 textCoord;

out vec4 fragColor;

bool ComputeVisibility(vec3 point){
	vec3 x = vec3(1.*xCutDirection,0.,0.);
	vec3 y = vec3(0.,1.*yCutDirection,0.);
	vec3 z = vec3(0.,0.,1.*zCutDirection);

	vec3 xCut = vec3(xCutPosition, 0.,0.);
	vec3 yCut = vec3(0., yCutPosition,0.);
	vec3 zCut = vec3(0., 0.,zCutPosition);

//TODO compute visibilty 
return true; 
}

// --------------------------------------------------
// Fragment Shader:
// --------------------------------------------------
void main() {


	if(!ComputeVisibility(position)) {
		discard;
	}

//TODO fetch color in texture
        fragColor = vec4( 1., 0.,0.,1. );
	
}
