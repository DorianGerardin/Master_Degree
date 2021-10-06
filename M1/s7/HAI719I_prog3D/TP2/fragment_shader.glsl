#version 330 core

// Output data
out vec4 FragColor;
// Ajouter une variable interpolée o_color venant du vertex shader
in vec3 o_color;
//

void main()
{

    // Mettre à jour la couleur avec la variable interpolée venant du vertex shader
    FragColor.rgb = o_color;
    FragColor.a = 1;

}
