# WebGL - GERARDIN Dorian - 21/10/2021

## TP2 :

### Etape 1 : 

_data : tableau défini dans le modèle_.
Ici on stocke les données dont nous aurons besoin comme des coordonnées par exemple.

### Etape 2 : 

`prog_texture = createProgram(gl, "vshader-texture", "fshader-texture");`
- Ici on fait le lien avec le vertex et fragment shader <br>

`v = getAttribLocation(prog_texture, nom_attr);`
 - Ici on affecte à *v* un identifiant liée à la variable *nom_attr* dans le shader<br>

`gl.vertexAttribPointer(...)`
- Ici, on affecte à la variable du shader grâce à _v_, la data passée en paramètre<br>

### Etape 3 : 

`buffer = createBuffer();`
- Ici, on initialise un buffer vide dans la variable buffer<br>

### Etape 4 : 

`gl.bindBuffer(gl.ARRAY_BUFFER, buffer);`
<br>

### Etape 5 : 

`gl.bufferData(gl.ARRAY_BUFFER, data);`
<br>

### Etape 6 : 

### Etape 7 : 
 
