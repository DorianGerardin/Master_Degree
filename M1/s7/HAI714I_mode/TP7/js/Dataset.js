class Articulation {
	constructor(coords, previous){
		this.x = coords.x
		this.y = coords.y
		this.previous = previous
		this.transformMatrix
		this.base;
	}

	setBase(vecArr) {
		this.base = vecArr
	}
}

class Os {
	constructor(art1, art2){
		this.from = art1
		this.to = art2 
		this.fromVec = vec2.fromValues(this.from.x, this.from.y)
		this.toVec = vec2.fromValues(this.to.x, this.to.y)
		this.vec = this.getVect()
	}

	getVect() {
		let subVec = vec2.create()
		vec2.subtract(subVec, this.toVec, this.fromVec)
		return subVec
	}

	getBase() {
		let norm = vec2.create()
		vec2.cross(norm, this.fromVec, this.toVec)
	}


}

class EnsemblesOs {
	constructor(name, liste_os){
		this.name = name;
		this.liste_os = liste_os;
		this.next = new Array();
		this.previous = new Array();
	}

	ajoutOs(os) {
		this.liste_os.push(os)
	}

}

class SqueletteArticule {

}






class Chain{
	constructor(name, points){
		this.name = name;
		this.points = points;
		this.next = new Array();
		this.previous = new Array();
	}


// ajout une autre chaine en l'attachant au dernier élément de cette chaine
// lorsque begin == true le chemin va du dernier élément de cette chaine au premier élément de l'autre chaine.
// autrement il va vers le dernier élément de l'autre chaine
	addAfter(anotherChain, begin){
		let b = begin ?? true;
		this.next.push({c:anotherChain, b:b});
	}

// ajout une autre chaine en l'attachant au premier élément de cette chaine
// lorsque begin == true le chemin va du dernier élément de cette chaine au premier élément de l'autre chaine.
// autrement il va vers le dernier élément de l'autre chaine	
	addBefore(anotherChain, begin){
		let b = begin ?? false;
		this.previous.push({c:anotherChain, b:b});
	}

//liens entre les "sous-chaines"
	drawBridges(elt, displayer, colors){
		let from, to;
		this.next.forEach(elt => {
			from = elt.b ? elt.c.points[0]: elt.c.points[elt.c.points.length - 1];
			to = this.points[this.points.length -1];
			displayer.mDrawLine(from, to, "#F00");
//			console.log("drawBridges "+this.name);
		});
		this.previous.forEach(elt => {
			from = elt.b ? elt.c.points[0]: elt.c.points[elt.c.points.length - 1];
			to = this.points[0];
			displayer.mDrawLine(from, to, "#F00");
//			console.log("drawBridges "+this.name);
		});
	}

	draw(displayer, colors, incrementalDrawing){
		displayer.drawLineStrip(this.points, colors, incrementalDrawing);
		displayer.drawPoints(this.points, colors, true);
		this.drawBridges(this, displayer, colors);
		this.previous.forEach(function(elt, idx, array){
			elt.c.draw(displayer, colors, true);
		});
		this.next.forEach(function(elt, idx, array){
			elt.c.draw(displayer, colors, true);
		});
	}

}

class Squelette{

	constructor(){
	this.unitx = 75; this.unity = 75; this.margX = 10; this.margY = 10;
		this.leg1 = new Chain("left leg", this.initLeg(" ", 20, true));
		this.leg2 = new Chain("right leg", this.initLeg(" ", 10, false));
		this.arm1 = new Chain("left arm", this.initArm(" ", 30, true));
		this.arm2 = new Chain("right arm", this.initArm(" ",50, false));
		this.tronc = new Chain("tronc", this.initTrunk(" ", 0));
		this.head = new Chain("head", this.initHead(" ", 40));
		this.tronc.addBefore(this.leg1);
		this.tronc.addBefore(this.leg2);
		this.tronc.addAfter(this.arm1, false);
		this.tronc.addAfter(this.arm2, false);
		this.tronc.addAfter(this.head);
	}

	draw(displayer, colors){
		this.tronc.draw(displayer, colors, false);
	}

	initHead(label, idx){
		let points = new Array();
		let x = 2, 
			y = [8.5];
		 for (let i = 0; i < y.length; i++){
		 		points.push(new Point(x*this.unitx+this.margX, y[i]*this.unity+this.margY, label+idx));
		 		//console.log("init tronc " + points[i].x + " "+ points[i].y);
		 		idx++;
		 }
		return points;
	}

	initTrunk(label, idx){
		let points = new Array();
		let x = 2, 
			y = 5.7;
		 for (let i = 0; i < 6; i++){
		 		points.push(new Point(x*this.unitx+this.margX, (y+0.4*i)*this.unity+this.margY, label+idx));
		 		//console.log("init tronc " + points[i].x + " "+ points[i].y);
		 		idx++;
		 }
		return points;
	}

	//left=true pour le côté gauche du modèle (côté droit de la fenêtre à l'écran quand le modèle fait face)
	initLeg(label, idx, left){
		let points = new Array();
		let x = left ? 3 : 1, 
			y = [1.5,3.2,5];
		 for (let i = 0; i < y.length; i++){
		 		points.push(new Point(x*this.unitx+this.margX, y[i]*this.unity+this.margY, label+idx));
		 		//console.log(left + " " + points[i].x + " "+ points[i].y);
		 		idx++;
		 }
		return points;
	}
	//left=true pour le côté gauche du modèle (côté droit de la fenêtre à l'écran quand le modèle fait face)
	initArm(label, idx, left){
		let points = new Array();
		let x = left ? [3.5, 3.3, 3.2] : [.5, .7, .9], 
			y = [4.5,5.8,7.5];
		 for (let i = 0; i < y.length; i++){
		 		points.push(new Point(x[i]*this.unitx+this.margX, y[i]*this.unity+this.margY, label+idx));
		 		//console.log(left + " " + points[i].x + " "+ points[i].y);
		 		idx++;
		 }
		return points;
	}


}