const algoEnvConv = {demiPlan: 0, jarvis:1, graham:2};

class EnveloppeConvexe{
	constructor(lesPoints, a){
		this.algo = a ?? algoEnvConv.jarvis;
		this.points = lesPoints;
		this.setAlgo(this.algo);
	}

	getElements (){
		let ec = new Array();
		for (let i=0; i < this.envconv.length; i++){
			ec.push(this.points[this.envconv[i]]);
		}
		return ec;
	}

	setAlgo(idAlgo){
		this.algo = idAlgo;
		switch (this.algo){
			case algoEnvConv.demiPlan:
				this.envconv = this.algoDemiPlan(this.points);
				break;
			case algoEnvConv.jarvis:
				this.envconv = this.algoJarvis(this.points);
				break;
			case algoEnvConv.graham:
				this.envconv = this.algoGraham(this.points);
				break;	
			default:
				console.log("algo non défini => algo jarvis utilisé")
				this.envconv = this.algoDemiPlan(this.points);
				break;
		}
	}

	findMinIdx(points){
		let imin = 0;

		for(let i = 1; i < points.length; i++){
			if (points[i].y < points[imin].y)
				imin = i;
		}

		return imin;
	}

	findFirst(i, j, points) /* i < j */ {
		if(i == 0 && j == 1) return points[2]
		else if (i == 0 && j == 2) return points[1]
		else return points[0] 
	}

	determinant(v1,v2){
		return v1.x * v2.y - v1.y * v2.x;
	}

	detSign(v1,v2){
		let d = this.determinant(v1,v2);
		if (d > 0) return 1;
		if (d == 0) return 0;
		return -1;
	}

	vect(a,b){
		return{x:b.x-a.x,y:b.y - a.y}
	}

	// return 0 si les points sont alignés, -1 pour un tour horaire (droit=horaire), +1 pour un tour gauche (=direct=trigo=antihoraire)
	tour(a, b, c){

		let ab = this.vect(a,b)
		let bc = this.vect(b,c)

		return this.detSign(ab, bc)

	}


	algoDemiPlan(points){
		console.log("algo d'intersection des demi-plans")
		let envconv = new Array();
		let n = points.length;
		let courant, previous, i, j, k;

		for(i = 0; i < n; i++) {
			for(j = i; j < n; j++) {
				if(i < j) {
					let mc = true;
					k = 0
					do {
						if(k == 0) {
							let first = this.findFirst(i, j, points)
							previous = this.tour(points[i], points[j], first)
						}
						if (k != i && k != j) {
							courant = this.tour(points[i], points[j], points[k])
							if (courant == 0) {// exception points alignés
								console.log("alignement", points[i], points[j], points[k])
							} else if (courant != previous) mc = false
						}
						k++
					} while(k < n && mc)
					if (k == n && previous == courant) {
						if (courant > 0) envconv.push(i, j)
						if (courant < 0) envconv.push(j, i)
						if (courant == 0) console.log() ("alignement", points[i], points[j], points[k])
					}
				} 
			}
		}

		return envconv;
	}

	 algoJarvis(points){
	 	let envconv
		console.log("algo Jarvis")

		//todo

		return envconv;
	}

	algoGraham(points){
		let imin = this.findMinIdx(points);
		let ec = new Array();
		ec[0] = imin;

		//todo

		return ec;
	}
}