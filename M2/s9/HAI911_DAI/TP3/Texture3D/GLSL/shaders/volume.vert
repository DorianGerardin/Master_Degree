// --------------------------------------------------
// shader definition
// --------------------------------------------------
#version 430

layout (location=0) in vec3 i_position;

// --------------------------------------------------
// Uniform variables:
// --------------------------------------------------
	uniform float xCutPosition;
	uniform float yCutPosition;
	uniform float zCutPosition;

	uniform float xMax;
	uniform float yMax;
	uniform float zMax;

        uniform mat4 mv_matrix;
        uniform mat4 proj_matrix;

// --------------------------------------------------
// varying variables
// --------------------------------------------------
        out vec3 position;
        out vec3 textCoord;
// --------------------------------------------------
// Vertex-Shader
// --------------------------------------------------


void main()
{

        position = i_position.xyz;
	

//TODO compute texture coord
	
}
