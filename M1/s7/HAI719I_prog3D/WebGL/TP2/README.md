# WebGL - TP2 - GERARDIN Dorian - 21/10/2021

## CÔTÉ JS

### Etape 1 : 

#### Créer (ou lire) l’image qui servira de texture

`function createDiskData() {
    diskCenters = new Array(DISK_COUNT);
    diskColors = new Array(DISK_COUNT);
    diskVelocities = new Array(DISK_COUNT);
    for (let i = 0; i < DISK_COUNT; i++) {
        diskColors[i] = [ Math.random(), Math.random(), Math.random(), 0.5 ];
        diskCenters[i] = [ 2*Math.random() - 1, 2*Math.random() - 1 ];
        let angle = Math.random()*2*Math.PI;
        let speed = 0.003 + 0.01*Math.random();
        diskVelocities[i] = [ speed*Math.cos(angle), speed*Math.sin(angle) ];
    }
}`

### Etape 2 : 

#### Donner des coordonnées de texture aux sommets du modèle à texturer

`gl.enableVertexAttribArray(a_coords_loc_texture);
gl.bindBuffer(gl.ARRAY_BUFFER, model.texCoordsBuffer);`

### Etape 3 : 

#### Créer un ou plusieurs objets de texture pour stocker les textures

`gl.bindTexture(gl.TEXTURE_2D, texture);
gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 512, 512, 0, gl.RGBA, gl.UNSIGNED_BYTE, null);`

### Etape 4 : 

#### Paramétrer le texture mapping 

`gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);`

### Etape 5 : 

#### Activer une ou plusieurs unités de texture 

`u_texture = gl.getUniformLocation(prog, "texture");`

### Etape 6 : 

#### Associer un objet de texture à une unité de texture

`gl.uniform1i(u_texture, 0);`

## CÔTÉ GLSL

### Etape 1 :

#### Dans le vertex shader Faire passer les coordonnées de textures au fragment shader (attributes (in) -> varying (out)

`attribute vec3 a_coords;
    attribute vec3 a_normal;
    attribute vec2 a_texCoords;
    uniform mat4 modelview;
    uniform mat4 projection;
    uniform float textureScale;
    varying vec3 v_normal;
    varying vec3 v_eyeCoords;
    varying vec2 v_texCoords;
    void main() {
        vec4 objectCoords = vec4(a_coords,1.0);
        vec4 eyeCoords = modelview * objectCoords;
        gl_Position = projection * eyeCoords;
        v_normal = normalize(a_normal);
        v_eyeCoords = eyeCoords.xyz/eyeCoords.w;
        v_texCoords = textureScale*a_texCoords;
}`

### Etape 2 :

#### Dans le fragment shader 

##### Déclarer l’ (les) unité(s) de texture utilisée(s) (sampler2D) 

`uniform sampler2D texture;`

##### Trouver la couleur d’un texel pour les coordonnées de texture en utilisant un sampler

`vec4 color = texture2D(texture, v_texCoords);`

##### Utiliser la couleur du texel pour calculer la couleur du fragment

`gl_FragColor = vec4( diffuseFactor*color.rgb, 1.0);`