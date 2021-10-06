#version 330 core
//A faire
// ajouter une variable uniform pour tous les sommets de type float permettant la mise à l'échelle
uniform float scale;
// ajouter une variable uniform pour tous les sommets de type vec3 permettant d'appliquer une translation au modèle
uniform vec3 translate;
// i.e. a appliquer sur tous les sommets
//---------

//---------
// ajouter une variable o_color de type vec3 interpolée a envoyer au fragment shader

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
// A faire : ajouter un attribut de sommet color, contenant les couleurs pour chaque sommet à ( location = 1 )
layout(location = 1) in vec3 vertexColor_modelspace;
out vec3 o_color;

void main(){

    //Mettre à jour ce code pour appliquer la translation et la mise à l'échelle
    gl_Position = vec4(vertexPosition_modelspace,1);

    gl_Position.x += translate.x;
    gl_Position.y += translate.y;

    gl_Position.xy *= scale;

    //Assigner la normale à la variable interpolée color
    o_color = vertexColor_modelspace;
}

