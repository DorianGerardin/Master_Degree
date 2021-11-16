class Shape{
    //[0,0],0.5,[0.5,0,0]
	constructor(center, radius, fgColor, bgColor, numberOfVertices){
        this.dimension = 2;
		this.radius = radius;
		this.center = center; 
        this.fgColor = fgColor;
        this.bgColor = bgColor;
        this.numberOfVertices = numberOfVertices;
        this.updateCoords();
    }
    niceColors(){
        let orange = [0.6386477637529299,0.06707652714664614,0.1926528301818181,1];
    }
    setBgColor(r,v,b){
        this.bgColor[0] = r;
        this.bgColor[1] = v;
        this.bgColor[2] = b;
        this.bgColor[3] = 1;
        return this;
    }

    updateCoords(){
        let angle = 0;
        let coords = new Float32Array(this.numberOfVertices * this.dimension);
        for (let i = 0; i < this.numberOfVertices*this.dimension; i++){
            angle = i * (2 * Math.PI / this.numberOfVertices);
            coords[2*i] = this.center[0] + this.radius * Math.cos(angle);
            coords[2*i+1] = this.center[1] + this.radius * Math.sin(angle);
        }
        this.coords = new BufferedData(coords, 0, this.dimension); 
        this.numberOfVertices = this.coords.size();
        return this;       
    }   

    setPosition(position){
        this.center[0] = position[0];
        this.center[1] = position[1];
        return this;
    }

    setSize(size){
        this.radius = size/2;
        return this;
    }

    drawWith(shader){
        shader.prepareCoords(this.coords);
       //console.log(this.numberOfVertices);
        shader.setUniformColor(this.bgColor);
        shader.gl2.drawArrays(shader.gl2.TRIANGLE_FAN, 0, this.numberOfVertices);
        shader.setUniformColor(this.fgColor);
        shader.gl2.drawArrays(shader.gl2.LINE_LOOP, 0, this.numberOfVertices);
	}
}