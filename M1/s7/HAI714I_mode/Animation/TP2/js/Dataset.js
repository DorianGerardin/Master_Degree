class Articulation {
	constructor(coords, label){
		let i = vec2.fromValues(0, 1)
		let j = vec2.fromValues(1, 0)
		this.x = coords.x
		this.y = coords.y
		this.coords = coords
		this.label = label
		this.transformMatrix
		this.base = {i: i, j:j};
		this.angle;
	}

	setBase(base) {
		this.base = base
	}

	setAngle(angle) {
		this.angle = angle
	}
}

class Os {
	constructor(art1, art2){
		this.from = art1
		this.to = art2 
		this.fromVec = vec2.fromValues(this.from.x, this.from.y)
		this.toVec = vec2.fromValues(this.to.x, this.to.y)
		this.vec = this.getVect()
		/*this.setBase()
		this.setAngle()*/
	}

	getVect() {
		let subVec = vec2.create()
		vec2.subtract(subVec, this.toVec, this.fromVec)
		return subVec
	}

	/*setBase() {
		let base = {i: null, j:null}
		let i, j
		vec2.normalize(i, this.vec)
		vec2.rotate(j, this.vec, this.toVec, -Math.PI/2)
		this.to.setBase({i: i, j:j})
	}

	setAngle() {
		let cosTheta = vec2.dot(this.vec, this.to.base.i)
		let theta = Math.acos(cosTheta)
		this.to.setAngle(theta)
	}*/

	/*rotate() {

	}*/


}

class EnsemblesOs {
	constructor(name, liste_os, liste_art){
		this.name = name;
		this.liste_os = liste_os;
		this.liste_art = liste_art;
		this.next = new Array();
		this.previous = new Array();
	}

	ajoutOs(os) {
		this.liste_os.push(os)
	}

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

	drawBridges(elt, displayer, colors){
		let from, to;
		let pointsArt = this.liste_art.map(art => {
			let point = new Point(art.x, art.y, art.label)
			return point
		})
		this.next.forEach(elt => {
			let points = elt.c.liste_art.map(art => {
				let point = new Point(art.x, art.y, art.label)
				return point
			})
			from = elt.b ? points[0]: points[points.length - 1];
			to = pointsArt[pointsArt.length -1];
			displayer.mDrawLine(from, to, "#F00");
//			console.log("drawBridges "+this.name);
		});
		this.previous.forEach(elt => {
			let points = elt.c.liste_art.map(art => {
				let point = new Point(art.x, art.y, art.label)
				return point
			})
			from = elt.b ? points[0]: points[points.length - 1];
			to = pointsArt[0];
			displayer.mDrawLine(from, to, "#F00");
//			console.log("drawBridges "+this.name);
		});
	}

	draw(displayer, colors, incrementalDrawing){
		let points = this.liste_art.map(art => {
			let point = new Point(art.x, art.y, art.label)
			return point
		})
		displayer.drawLineStrip(points, colors, incrementalDrawing);
		displayer.drawPoints(points, colors, true);
		this.drawBridges(this, displayer, colors);
		this.previous.forEach(function(elt, idx, array){
			elt.c.draw(displayer, colors, true);
		});
		this.next.forEach(function(elt, idx, array){
			elt.c.draw(displayer, colors, true);
		});
	}

}

class SqueletteArticule {
	constructor(){
		this.unitx = 75; this.unity = 75; this.margX = 10; this.margY = 10;
		
		this.head = this.initHead("H")
		this.trunk = this.initTrunk("T", 10)
		this.armL = this.initArm("AL", 20, true)
		this.armR = this.initArm("AR", 30, false)
		this.legL = this.initLeg("LL", 40, true)
		this.legR = this.initLeg("LR", 50, false)
		
		this.trunk.addBefore(this.legL);
		this.trunk.addBefore(this.legR);
		this.trunk.addAfter(this.armL, false);
		this.trunk.addAfter(this.armR, false);
		this.trunk.addAfter(this.head);
	}

	draw(displayer, colors){
		this.trunk.draw(displayer, colors, false);
	}

	initHead(label){
		let articulationArray = new Array();
		let liste_os = new Array();
		let x = 2, 
			y = 8.5;
		let coords = {x: x*this.unitx+this.margX, y: y*this.unity+this.margY};
		let head = new Articulation(coords, label)
		articulationArray.push(head)
		let os = new Os(head, head)
		liste_os.push(os)
		let ensemblesOs = new EnsemblesOs("head", liste_os, articulationArray)
		return ensemblesOs;
	}

	initTrunk(label, idx){
		let x = 2, 
			y = 5.7;
		let articulationArray = new Array();
		let liste_os = new Array();
		 for (let i = 0; i < 6; i++){
		 		let coords = {x: x*this.unitx+this.margX, y: (y+0.4*i)*this.unity+this.margY};
		 		let art = new Articulation(coords, label+idx);
		 		articulationArray.push(art)
		 		idx++
		 }
		 for (let i = 1; i < 6; i++) {
		 	let os = new Os(articulationArray[i-1], articulationArray[i])
		 	liste_os.push(os)
		 }
		 let ensemblesOs = new EnsemblesOs("trunk", liste_os, articulationArray)
		 console.log(ensemblesOs)
		return ensemblesOs;
	}

	initLeg(label, idx, left){
		let points = new Array();
		let x = left ? 3 : 1, 
			y = [1.5,3.2,5];
		let articulationArray = new Array();
		let liste_os = new Array();
		 for (let i = 0; i < y.length; i++){
		 		let coords = {x: x*this.unitx+this.margX, y: y[i]*this.unity+this.margY};
		 		let art = new Articulation(coords, label+idx);
		 		articulationArray.push(art)
		 		idx++
		 }
		 for (let i = 1; i < y.length; i++) {
		 	let os = new Os(articulationArray[i-1], articulationArray[i])
		 	liste_os.push(os)
		 }
		 let ensemblesOs = new EnsemblesOs("leg", liste_os, articulationArray)
		return ensemblesOs;
	}

	initArm(label, idx, left){
		let points = new Array();
		let x = left ? [3.5, 3.3, 3.2] : [.5, .7, .9], 
			y = [4.5,5.8,7.5];
		let articulationArray = new Array();
		let liste_os = new Array();
		 for (let i = 0; i < y.length; i++){
		 		let coords = {x: x[i]*this.unitx+this.margX, y: y[i]*this.unity+this.margY};
		 		let art = new Articulation(coords, label+idx);
		 		articulationArray.push(art)
		 		idx++
		 }
		 for (let i = 1; i < y.length; i++) {
		 	let os = new Os(articulationArray[i-1], articulationArray[i])
		 	liste_os.push(os)
		 }
		 let ensemblesOs = new EnsemblesOs("arm", liste_os, articulationArray)
		return ensemblesOs;
	}


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