var gl = null;
var programInfo = null;
var buffers = null;
var scene = {

	// Matrix
	projectionMatrix : mat4.create(),
	viewMatrix       : mat4.create(),
	modelMatrix      : mat4.create(),
	normalMatrix     : mat4.create(),

	// Camera
	viewPosition : vec3.clone([5, 2, 5]),
	viewUp       : vec3.clone([0, 1, 0]),
	viewCenter   : vec3.clone([0, 0, 0]),

	// Light (example)
	lightPosition : vec3.clone([0, 10, 5]),
	lightColor    : vec3.clone([1, 1, 1]),

	// Material (example)
	objectColor   : vec3.clone([1, 0, 0]),

	// Pbr parameters (todo)
	albedo        : vec3.clone([1, 0, 0]),
	roughness     : 0.5,
	metalness     : 0,
	ao            : 0,
};

main();

//
// Start here
//
function main() {
	const canvas = document.querySelector('#glcanvas');
	const gl2 = canvas.getContext('webgl2');
	gl = gl2;
	// If we don't have a GL context, give up now

	if (!gl) {
		alert('Unable to initialize WebGL. Your browser or machine may not support it.');
		return;
	}


	createMainShaderProgram(gl);
	initMatrix();
	initBuffers();
	createUI();

	var then = 0;
	// Draw the scene repeatedly
	function render(now) {
		now *= 0.001;  // convert to seconds
		const deltaTime = now - then;
		then = now;

		drawScene(deltaTime);
		const fps = 1 / deltaTime;
		document.getElementById('fps').innerHTML = fps.toFixed(1);
		requestAnimationFrame(render);
	}
	requestAnimationFrame(render);
}

//
// Draw the scene.
//
function drawScene(deltaTime) {
	gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Clear to black, fully opaque
	gl.clearDepth(1.0);                 // Clear everything
	gl.enable(gl.DEPTH_TEST);           // Enable depth testing
	gl.depthFunc(gl.LEQUAL);            // Near things obscure far things

	// Clear the canvas before we start drawing on it.
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

	// RECOMPUTE EACH FRAME MATRIX
	mat4.lookAt(scene.viewMatrix, scene.viewPosition, scene.viewCenter, scene.viewUp);
	// Tell WebGL how to pull out the positions from the position
	// buffer into the vertexPosition attribute
	if (programInfo.attribLocations.vertexPosition != -1) {
		const numComponents = 3;
		const type = gl.FLOAT;
		const normalize = false;
		const stride = 0;
		const offset = 0;
		gl.bindBuffer(gl.ARRAY_BUFFER, buffers.position);
		gl.vertexAttribPointer(programInfo.attribLocations.vertexPosition,
			numComponents, type, normalize, stride, offset);
		gl.enableVertexAttribArray(programInfo.attribLocations.vertexPosition);
	}

	// Tell WebGL how to pull out the texture coordinates from
	// the texture coordinate buffer into the textureCoord attribute.
	if (programInfo.attribLocations.vertexUV != -1) {
		const numComponents = 2;
		const type = gl.FLOAT;
		const normalize = false;
		const stride = 0;
		const offset = 0;
		gl.bindBuffer(gl.ARRAY_BUFFER, buffers.textureCoord);
		gl.vertexAttribPointer(
			programInfo.attribLocations.vertexUV,
			numComponents,
			type,
			normalize,
			stride,
			offset);
		gl.enableVertexAttribArray(programInfo.attribLocations.vertexUV);
	}

// Tell WebGL how to pull out the normals from
// the normal buffer into the vertexNormal attribute.
if (programInfo.attribLocations.vertexNormal != -1) {
const numComponents = 3;
const type = gl.FLOAT;
const normalize = false;
const stride = 0;
const offset = 0;
gl.bindBuffer(gl.ARRAY_BUFFER, buffers.normal);
gl.vertexAttribPointer(
programInfo.attribLocations.vertexNormal,
numComponents,
type,
normalize,
stride,
offset);
gl.enableVertexAttribArray(
programInfo.attribLocations.vertexNormal);
}

// Tell WebGL which indices to use to index the vertices
gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, buffers.indices);

// Tell WebGL to use our program when drawing
gl.useProgram(programInfo.program);

// Set the shader uniforms
gl.uniformMatrix4fv(programInfo.uniformLocations.projectionMatrix, false,
scene.projectionMatrix);
gl.uniformMatrix4fv(programInfo.uniformLocations.viewMatrix,
false, scene.viewMatrix);
gl.uniformMatrix4fv(
	programInfo.uniformLocations.modelMatrix,
	false,scene.modelMatrix);
	gl.uniformMatrix4fv(
		programInfo.uniformLocations.normalMatrix,
		false,
		scene.normalMatrix);

		// Specify the texture to map onto the faces.

		// Tell WebGL we want to affect texture unit 0
		// gl.activeTexture(gl.TEXTURE0);

		// Bind the texture to texture unit 0
		// gl.bindTexture(gl.TEXTURE_2D, texture);

		// Tell the shader we bound the texture to texture unit 0
		// gl.uniform1i(programInfo.uniformLocations.uSampler, 0);

		gl.uniform3fv(programInfo.uniformLocations.viewPosition, scene.viewPosition);

		gl.uniform3fv(programInfo.uniformLocations.lightPosition, scene.lightPosition);

		gl.uniform3fv(programInfo.uniformLocations.lightColor, scene.lightColor);

		gl.uniform3fv(programInfo.uniformLocations.objectColor, scene.objectColor);

		if(programInfo.uniformLocations.albedo) {
			gl.uniform3fv(programInfo.uniformLocations.albedo, scene.albedo);
		}

		if(programInfo.uniformLocations.metalness) {
			gl.uniform1f(programInfo.uniformLocations.metalness, scene.metalness);
		}

		if(programInfo.uniformLocations.roughness) {
			gl.uniform1f(programInfo.uniformLocations.roughness, scene.roughness);
		}




		// DRAW MESH
		{
			const vertexCount = 36;
			const type = gl.UNSIGNED_SHORT;
			const offset = 0;
			gl.drawElements(gl.TRIANGLES, vertexCount, type, offset);
		}
	}



































// BUFFERS



//
// initBuffers
//
// Initialize the buffers we'll need. For this demo, we just
// have one object -- a simple three-dimensional cube.
//
function initBuffers() {

	// Create a buffer for the cube's vertex positions.

	const positionBuffer = gl.createBuffer();

	// Select the positionBuffer as the one to apply buffer
	// operations to from here out.

	gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

	// Now create an array of positions for the cube.

	const positions = [
	// Front face
	-1.0, -1.0,  1.0,
	1.0, -1.0,  1.0,
	1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,

	// Back face
	-1.0, -1.0, -1.0,
	-1.0,  1.0, -1.0,
	1.0,  1.0, -1.0,
	1.0, -1.0, -1.0,

	// Top face
	-1.0,  1.0, -1.0,
	-1.0,  1.0,  1.0,
	1.0,  1.0,  1.0,
	1.0,  1.0, -1.0,

	// Bottom face
	-1.0, -1.0, -1.0,
	1.0, -1.0, -1.0,
	1.0, -1.0,  1.0,
	-1.0, -1.0,  1.0,

	// Right face
	1.0, -1.0, -1.0,
	1.0,  1.0, -1.0,
	1.0,  1.0,  1.0,
	1.0, -1.0,  1.0,

	// Left face
	-1.0, -1.0, -1.0,
	-1.0, -1.0,  1.0,
	-1.0,  1.0,  1.0,
	-1.0,  1.0, -1.0,
	];

	// Now pass the list of positions into WebGL to build the
	// shape. We do this by creating a Float32Array from the
	// JavaScript array, then use it to fill the current buffer.

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);

	// Set up the normals for the vertices, so that we can compute lighting.

	const normalBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, normalBuffer);

	const vertexNormals = [
	// Front
	0.0,  0.0,  1.0,
	0.0,  0.0,  1.0,
	0.0,  0.0,  1.0,
	0.0,  0.0,  1.0,

	// Back
	0.0,  0.0, -1.0,
	0.0,  0.0, -1.0,
	0.0,  0.0, -1.0,
	0.0,  0.0, -1.0,

	// Top
	0.0,  1.0,  0.0,
	0.0,  1.0,  0.0,
	0.0,  1.0,  0.0,
	0.0,  1.0,  0.0,

	// Bottom
	0.0, -1.0,  0.0,
	0.0, -1.0,  0.0,
	0.0, -1.0,  0.0,
	0.0, -1.0,  0.0,

	// Right
	1.0,  0.0,  0.0,
	1.0,  0.0,  0.0,
	1.0,  0.0,  0.0,
	1.0,  0.0,  0.0,

	// Left
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0,
	-1.0,  0.0,  0.0
	];

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vertexNormals),
	gl.STATIC_DRAW);

	// Now set up the texture coordinates for the faces.

	const textureCoordBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ARRAY_BUFFER, textureCoordBuffer);

	const textureCoordinates = [
	// Front
	0.0,  0.0,
	1.0,  0.0,
	1.0,  1.0,
	0.0,  1.0,
	// Back
	0.0,  0.0,
	1.0,  0.0,
	1.0,  1.0,
	0.0,  1.0,
	// Top
	0.0,  0.0,
	1.0,  0.0,
	1.0,  1.0,
	0.0,  1.0,
	// Bottom
	0.0,  0.0,
	1.0,  0.0,
	1.0,  1.0,
	0.0,  1.0,
	// Right
	0.0,  0.0,
	1.0,  0.0,
	1.0,  1.0,
	0.0,  1.0,
	// Left
	0.0,  0.0,
	1.0,  0.0,
	1.0,  1.0,
	0.0,  1.0,
	];

	gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(textureCoordinates),
	gl.STATIC_DRAW);

	// Build the element array buffer; this specifies the indices
	// into the vertex arrays for each face's vertices.

	const indexBuffer = gl.createBuffer();
	gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, indexBuffer);

	// This array defines each face as two triangles, using the
	// indices into the vertex array to specify each triangle's
	// position.

	const indices = [
	0,  1,  2,      0,  2,  3,    // front
	4,  5,  6,      4,  6,  7,    // back
	8,  9,  10,     8,  10, 11,   // top
	12, 13, 14,     12, 14, 15,   // bottom
	16, 17, 18,     16, 18, 19,   // right
	20, 21, 22,     20, 22, 23,   // left
	];

	// Now send the element array to GL

	gl.bufferData(gl.ELEMENT_ARRAY_BUFFER,
	new Uint16Array(indices), gl.STATIC_DRAW);

	buffers = {
		position     : positionBuffer,
		normal       : normalBuffer,
		textureCoord : textureCoordBuffer,
		indices      : indexBuffer,
	};
}

// LOAD TEXTURE

//
// Initialize a texture and load an image.
// When the image finished loading copy it into the texture.
//
function loadTexture(gl, url) {
	const texture = gl.createTexture();
	gl.bindTexture(gl.TEXTURE_2D, texture);

	// Because images have to be download over the internet
	// they might take a moment until they are ready.
	// Until then put a single pixel in the texture so we can
	// use it immediately. When the image has finished downloading
	// we'll update the texture with the contents of the image.
	const level = 0;
	const internalFormat = gl.RGBA;
	const width = 1;
	const height = 1;
	const border = 0;
	const srcFormat = gl.RGBA;
	const srcType = gl.UNSIGNED_BYTE;
	const pixel = new Uint8Array([0, 0, 255, 255]);  // opaque blue
	gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
	width, height, border, srcFormat, srcType,
	pixel);

	const image = new Image();
	image.onload = function() {
	gl.bindTexture(gl.TEXTURE_2D, texture);
	gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
		srcFormat, srcType, image);

		// WebGL1 has different requirements for power of 2 images
		// vs non power of 2 images so check if the image is a
		// power of 2 in both dimensions.
		if (isPowerOf2(image.width) && isPowerOf2(image.height)) {
			// Yes, it's a power of 2. Generate mips.
			gl.generateMipmap(gl.TEXTURE_2D);
		} else {
			// No, it's not a power of 2. Turn of mips and set
			// wrapping to clamp to edge
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
		}
	};
	image.src = url;

	return texture;
}

function isPowerOf2(value) {
	return (value & (value - 1)) == 0;
}

























// MATRICES


function initMatrix() {
	// Create a perspective matrix, a special matrix that is
	// used to simulate the distortion of perspective in a camera.
	// Our field of view is 45 degrees, with a width/height
	// ratio that matches the display size of the canvas
	// and we only want to see objects between 0.1 units
	// and 100 units away from the camera.
	const fieldOfView = 45 * Math.PI / 180;   // in radians
	const aspect = gl.canvas.clientWidth / gl.canvas.clientHeight;
	const zNear = 0.1;
	const zFar = 100.0;


	// note: glmatrix.js always has the first argument
	// as the destination to receive the result.
	mat4.perspective(scene.projectionMatrix,
		fieldOfView,
		aspect,
		zNear,
		zFar);

	// Set the drawing position to the "identity" point, which is
	// the center of the scene.
	mat4.identity(scene.modelMatrix);
	mat4.lookAt(scene.viewMatrix, scene.viewPosition, scene.viewCenter, scene.viewUp);
	mat4.invert(scene.normalMatrix, scene.modelMatrix);
	mat4.transpose(scene.normalMatrix, scene.normalMatrix);
}


























// SHADERS



function createMainShaderProgram(gl) {
	// Vertex shader program
	const vsSource = document.getElementById("vs").textContent;
	// Fragment shader program
	const fsSource = document.getElementById("fs").textContent;
	// Shader sources
	const shaderSources = {
		pbr: {
			vsSource : vsSource,
			fsSource : fsSource
		}
	};

	// Initialize a shader program; this is where all the lighting
	// for the vertices and so forth is established.
	const shaderProgram = initShaderProgram(gl, shaderSources.pbr.vsSource, shaderSources.pbr.fsSource);

	// Collect all the info needed to use the shader program.
	// Look up which attributes our shader program is using
	// for aVertexPosition, aVertexNormal, aTextureCoord,
	// and look up uniform locations.
	programInfo = {
		program : shaderProgram,
		attribLocations : {
			vertexPosition : gl.getAttribLocation(shaderProgram, 'aVertexPosition'),
			vertexNormal   : gl.getAttribLocation(shaderProgram, 'aVertexNormal'),
			vertexUV       : gl.getAttribLocation(shaderProgram, 'aVertexUV'),
		},
		uniformLocations : {
			projectionMatrix : gl.getUniformLocation(shaderProgram, 'uProjectionMatrix'),
			// modelViewMatrix  : gl.getUniformLocation(shaderProgram, 'uModelViewMatrix'),
			modelMatrix      : gl.getUniformLocation(shaderProgram, 'uModelMatrix'),
			normalMatrix     : gl.getUniformLocation(shaderProgram, 'uNormalMatrix'),
			viewMatrix       : gl.getUniformLocation(shaderProgram, 'uViewMatrix'),
			// uSampler         : gl.getUniformLocation(shaderProgram, 'uSampler'),
			viewPosition     : gl.getUniformLocation(shaderProgram, 'uViewPos'),
			lightPosition    : gl.getUniformLocation(shaderProgram, 'uLightPos'),
			lightColor       : gl.getUniformLocation(shaderProgram, 'uLightColor'),
			objectColor      : gl.getUniformLocation(shaderProgram, 'uObjectColor'),
			albedo 			 : gl.getUniformLocation(shaderProgram, 'uAlbedo'),
			metalness		 : gl.getUniformLocation(shaderProgram, 'uMetalness'),
			roughness     	 : gl.getUniformLocation(shaderProgram, 'uRoughness'),
			ao     			 : gl.getUniformLocation(shaderProgram, 'uAo'),
		}
	};
}

//
// Initialize a shader program, so WebGL knows how to draw our data
//
function initShaderProgram(gl, vsSource, fsSource) {
	const vertexShader = loadShader(gl, gl.VERTEX_SHADER, vsSource);
	const fragmentShader = loadShader(gl, gl.FRAGMENT_SHADER, fsSource);

	// Create the shader program
	const shaderProgram = gl.createProgram();
	gl.attachShader(shaderProgram, vertexShader);
	gl.attachShader(shaderProgram, fragmentShader);
	gl.linkProgram(shaderProgram);

	// If creating the shader program failed, alert
	if (!gl.getProgramParameter(shaderProgram, gl.LINK_STATUS)) {
		alert('Unable to initialize the shader program: ' + gl.getProgramInfoLog(shaderProgram));
		return null;
	}

	return shaderProgram;
}
//
// creates a shader of the given type, uploads the source and
// compiles it.
//
function loadShader(gl, type, source) {
	const shader = gl.createShader(type);

	// Send the source to the shader object
	gl.shaderSource(shader, source);

	// Compile the shader program
	gl.compileShader(shader);

	// See if it compiled successfully
	if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
		const error = gl.getShaderInfoLog(shader);
		console.error('glsl:' + error);
		gl.deleteShader(shader);
		return null;
	}

	return shader;
}
































// UI
/**
* Use this to create a controlable vector 3 in UI
*/
function addVec3Parameter(entity, field, step=0.1) {
	var wrapper = document.createElement("div");
	var firstLabel = document.createElement("span");
	firstLabel.innerHTML = field;
	wrapper.appendChild(firstLabel);
	for (var i = 0; i < 3; i++) {
		var label = document.createElement("span");
		label.innerHTML = "[" + i + "]";
		wrapper.appendChild(label);
		var input = document.createElement("input");
		input.type = "number";
		input.step = step; // step
		input.value = scene[field][i];
		input._target = i;
		input.addEventListener("input", function(event, value) {
			scene[field][event.target._target] = event.target.valueAsNumber;
		});
		input.addEventListener("change", function(event, value) {
			scene[field][event.target._target] = event.target.valueAsNumber;
		});
		input.addEventListener("valuechange", function(event, value) {
			scene[field][event.target._target] = event.target.valueAsNumber;
		});
		wrapper.appendChild(input);
	}
	entity.appendChild(wrapper);
}

/**
* Use this to create a controlable float in UI
*/
function addFloatParameter(entity, field) {
	var wrapper = document.createElement("div");
	var firstLabel = document.createElement("span");
	firstLabel.innerHTML = field;
	wrapper.appendChild(firstLabel);
	var input = document.createElement("input");
	input.type = "number";
	input.value = scene[field];
	input.addEventListener("change", function(event, value) {
		scene[field] = event.target.valueAsNumber;
	});
	input.addEventListener("valuechange", function(event, value) {
		scene[field] = event.target.valueAsNumber;
	});
	wrapper.appendChild(input);
	entity.appendChild(wrapper);
}

/**
* Use this to create a controlable texture in UI (TODO)
*/
function addTextureParameter(entity, field) {
	var wrapper = document.createElement("div");
	var firstLabel = document.createElement("span");
	firstLabel.innerHTML = field;
	wrapper.appendChild(firstLabel);
	var input = document.createElement("input");
	input.type = "file";
	input.accept = ".jpeg,.png";
	input.addEventListener("change", function(event) {
		console.log(event.target.value);
		//loadTexture(even)
	});
	input.addEventListener("valuechange", function(event) {
		//console.log(event);
	});
	wrapper.appendChild(input);
	entity.appendChild(wrapper);
}

/** main function to create UI**/
function createUI() {
	var parametersDiv = document.getElementById("parameters");

	addVec3Parameter(parametersDiv, "viewPosition");
	addVec3Parameter(parametersDiv, "lightPosition");
	addVec3Parameter(parametersDiv, "lightColor");
	addVec3Parameter(parametersDiv, "objectColor");

	var title = document.createElement("h2");
	title.innerHTML = "PBR parameters (TO DO)";
	parametersDiv.appendChild(title);
	addVec3Parameter(parametersDiv, "albedo");
	addFloatParameter(parametersDiv, "roughness");
	addFloatParameter(parametersDiv, "metalness");
	addFloatParameter(parametersDiv, "ao");

	title = document.createElement("h2");
	title.innerHTML = "PBR advanced parameters (TO DO)";
	parametersDiv.appendChild(title);
	addTextureParameter(parametersDiv, "albedo_map");
	addTextureParameter(parametersDiv, "normal_map");
	addTextureParameter(parametersDiv, "metalness_map");
	addTextureParameter(parametersDiv, "roughness_map");
	addTextureParameter(parametersDiv, "ao_map");
}
