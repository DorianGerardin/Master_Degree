class Shader{
	constructor(canvasId, vertexShaderSourceId, fragmentShaderSourceId){
		let canvas = document.getElementById(canvasId);
		if(!canvas) {
			console.log("Erreur: le canvas n'existe pas "+canvasId);
			return;
		}

		//program context
		this.gl2 = canvas.getContext("webgl2");
		this.width = canvas.width;
		this.height = canvas.height;
		this.aspectRatio = this.width / this.height;
		this.program = webglUtils.createProgramFromScripts(this.gl2, [vertexShaderSourceId,fragmentShaderSourceId]);
		this.projectionMatrix = null;
		this.modelViewMatrix = null;

		this.gl2.useProgram(this.program);

		//uniforms
		this.coloredVertices = this.gl2.getUniformLocation(this.program,'js_u_colored_vertices'); 
		this.uniformColors = this.gl2.getUniformLocation(this.program,'js_u_uniform_colors'); 
		this.color = this.gl2.getUniformLocation(this.program,'js_u_color'); 
		this.projectionMatrixUniform = this.gl2.getUniformLocation(this.program,'js_u_projection');
		this.modelViewMatrixUniform = this.gl2.getUniformLocation(this.program,'js_u_modelView');

		//attributes
		this.coordsAttribute = this.gl2.getAttribLocation(this.program, 'js_coords');
		this.colorsAttribute = this.gl2.getAttribLocation(this.program, 'js_colors');
	}

	init(r, v, b, perspective, basicTransforms){
		this.initDisplay(r,v,b);
		this.initProjection(perspective);
		let gl = this.gl2;
		gl.useProgram(this.program);
		this.modelViewMatrix = mat4.create();

		if (basicTransforms) this.initModelView(basicTransforms);
		return this.gl2;
	}

	initModelView(){
		this.modelViewMatrix = mat4.create();
		this.gl2.uniformMatrix4fv(this.modelViewMatrixUniform, false, this.modelViewMatrix);
	}

	initDisplay(r,v,b){
		let gl = this.gl2;
     	gl.clearColor(r,v,b,1);
        gl.clear(gl.COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT);
        gl.enable(gl.DEPTH_TEST);
	}

	initProjection(perspective){ 
		let gl = this.gl2;
		gl.useProgram(this.program);
		gl.viewport(0,0,this.width,this.height);  // Reset the viewport
		this.projectionMatrix = mat4.create();
		if (perspective) {
			mat4.perspective(this.projectionMatrix, Math.PI/6, shader.aspectRatio, 1, 500);
		}else{
			let sceneWidth = this.width/2, sceneHeight = this.height / 2;
			mat4.ortho(this.projectionMatrix, -sceneWidth, sceneWidth,-sceneHeight, sceneHeight, 2*sceneWidth, -2*sceneWidth);
		}
		gl.uniformMatrix4fv(this.projectionMatrixUniform, false, this.projectionMatrix);
	}

	translate(x,y,z){
		mat4.translate(this.modelViewMatrix, this.modelViewMatrix, [x,y,z]);
		this.gl2.uniformMatrix4fv(this.modelViewMatrixUniform, false, this.modelViewMatrix);
	}

	rotate(axe, angle){
		switch (axe){
			case 1:
				mat4.rotateX(this.modelViewMatrix, this.modelViewMatrix, angle);
				break;
			case 2:
				mat4.rotateZ(this.modelViewMatrix, this.modelViewMatrix, angle);
				break;
			case 3:
				mat4.rotateY(this.modelViewMatrix, this.modelViewMatrix, angle);
				break;
		}
		this.gl2.uniformMatrix4fv(this.modelViewMatrixUniform, false, this.modelViewMatrix);
	}

	basicModelView(){
		if (basicTransforms){
			mat4.scale(this.modelViewMatrix, this.modelViewMatrix, [1.3,1.3,1.3]);
			mat4.translate(this.modelViewMatrix, this.modelViewMatrix, [-40,-this.height/4,0]);
			mat4.rotateY(this.modelViewMatrix, this.modelViewMatrix, -Math.PI / 10);
			mat4.rotateX(this.modelViewMatrix, this.modelViewMatrix, Math.PI / 6);
			mat4.rotateZ(this.modelViewMatrix, this.modelViewMatrix, -Math.PI / 10);
		}
		gl.uniformMatrix4fv(this.modelViewMatrixUniform, false, this.modelViewMatrix);
	}

	//  crée des vertex buffer objects (VBO) pour stocker les coordonnées et/ou les indices
	// 	- pour drawArray (sans indices) => 1 appel (indices = false) ou 
	//  - pour drawElement (avec indices) => appeler prepareAttrib à 2 reprises
	// 			(1 appel pour les sommets (indices false) 
	// 			et 1 appel pour les indices (indices = true))
	prepareCoords(coords, indices){
		if (!coords.data) console.log("warning prepareAttribs with null or undefined coords");
        //activer le shader
        this.gl2.useProgram(this.program);

       	//creer le vbo et le placer comme buffer courant
        let vbo = this.gl2.createBuffer();
        let cBuffer = indices ? this.gl2.ELEMENT_ARRAY_BUFFER: this.gl2.ARRAY_BUFFER;
        //console.log("prepare coords -> cbuffer? " + cBuffer);
        this.gl2.bindBuffer(cBuffer, vbo);        	

		//lier le buffer courant avec l'attribut du shader qui lui correspond
        //activation de l'attribut et configuration de la lecture des données
        if (!indices){
          	this.gl2.enableVertexAttribArray(this.coordsAttribute);       	
        	this.gl2.vertexAttribPointer(this.coordsAttribute, coords.dimension, coords.getType(this.gl2), false, 0, 0);
        }

        //écrire les données dans le buffer courant
        this.gl2.bufferData(cBuffer, coords.data, this.gl2.STATIC_DRAW);
	}

	// crée un vertex buffer objects (VBO) pour stocker des données simples
	// data must be encapsulated in BufferedData
	// attrib doit être déclaré (c'est une indirection sur les attribs du shader)
 	prepareVBO(data, attrib){       
        //console.log ("trying to create VBO for "+attrib+","+ data.dimension+","+ data.getType(this.gl2)) ;  	
        //activer le shader
        this.gl2.useProgram(this.program);

		let vbo = this.gl2.createBuffer();
    	let cBuffer = this.gl2.ARRAY_BUFFER;
    	this.gl2.bindBuffer(cBuffer, vbo);        	
        //console.log("prepare VBO -> cbuffer? " + cBuffer);

		//lier le buffer courant avec l'attribut du shader qui lui correspond
        //activation de l'attribut et configuration de la lecture des données
        this.gl2.enableVertexAttribArray(attrib);    
        this.gl2.vertexAttribPointer(attrib, data.dimension, data.getType(this.gl2), false, 0, 0);

    	//écrire les données dans le buffer courant
    	this.gl2.bufferData(cBuffer, data.data, this.gl2.STATIC_DRAW);
	}

 	prepareColors(coords){       
        //console.log ("trying to create VBO for "+coords+","+ this.colorsAttribute+","+ this.coordsAttribute) ;  	
		this.prepareVBO(coords, this.colorsAttribute);
		gl.useProgram(this.program);
 		this.gl2.uniform1i(this.coloredVertices, 1);
	}

	setUniformColor(color){
		console.log("setting colors "+ color[0]+","+color[1]+","+color[2]+","+color[3]);
       	this.gl2.uniform4f(this.color, color[0],color[1],color[2],color[3]);
 		this.gl2.uniform1i(this.uniformColors, 1);
	}

	unsetUniformColor(){
 		this.gl2.uniform1i(this.uniformColors, 0);
	}

	blend(enable){
		let gl = this.gl2;
		if (enable){
        	gl.enable(gl.BLEND);
    		gl.blendFuncSeparate(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA, gl.ZERO, gl.ONE);
    	}else{
    		gl.disable(gl.BLEND);
    	}
    	return this;
	}
}