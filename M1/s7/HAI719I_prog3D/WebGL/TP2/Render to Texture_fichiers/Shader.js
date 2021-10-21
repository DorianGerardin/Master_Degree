class Shader {

	data : tableau défini
	// Ici on stocke les données dont nous aurons besoin comme des coordonnées par exemple
	
	shader : nom_attr
	//
	
	v = getAttribLocation(?, nom_attr);
	//
	
	buffer = createBuffer();
	//
	
	gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
	//
	
	gl.bufferData(gl.ARRAY_BUFFER, data);
	//
	
	gl.vertexAttribPointer();
	//
	
	gl.enable(attrib);
	//

}