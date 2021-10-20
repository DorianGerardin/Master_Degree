## Question 2 : 

Parmi les transformations proposées, celles utilisées dans le programme sont la projection perspective en faisant :
<strong>mat4.perspective(projection, Math.PI/10, canvas.width/canvas.height, 1, 10);</strong> <br>
ainsi qu'une transformation de l'objet ou de modélisation (je n'ai pas bien compris la différence entre ces dernières), notamment une translation en faisant : <br>
<strong>mat4.fromTranslation(modelview, vec3.fromValues(0,0,-5));</strong>

## Question 3 :

La matrice à modifier est modelView. Elle doit être modifiée après la translation. 

## Question 4 :  

Voilà le code à ajouter : 

* mat4.rotateX(modelview, modelview, Math.PI/7);
* mat4.rotateY(modelview, modelview, Math.PI/7);
* mat4.rotateZ(modelview, modelview, Math.PI/7);
