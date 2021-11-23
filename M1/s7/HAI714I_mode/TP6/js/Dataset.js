class Dataset{
constructor(){
	
}
initRandomPoints(n, w, h, label){
	let points = new Array();
	for (let i = 0; i < n; i++){
			points.push(new Point(12 + (w - 40) * Math.random(), 20 + (h - 40) * Math.random(), label+i));
	}
	return points;
}	
initFixedPoints(label){
	let points = new Array();
	let x = [120, 30, 220, 330, 60], y = [340, 450, 100, 450, 200];
	for (let i = 0; i < 5; i++){
			points.push(new Point(x[i], y[i], label+i));
	}
	return points;
}	
}