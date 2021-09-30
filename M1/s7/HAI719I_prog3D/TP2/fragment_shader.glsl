#version 330 core

// Output data
out vec4 FragColor;
// Ajouter une variable interpolée o_color venant du vertex shader
//

void main()
{

    // Mettre à jour la couleur avec la variable interpolée venant du vertex shader
    FragColor = vec4(1., 0., 0., 1.);// Output color = red

}
