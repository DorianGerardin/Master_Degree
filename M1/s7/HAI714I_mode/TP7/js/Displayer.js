class Point {
    // représentation d'un point "mathématique" et conversion vers les coordonnées écran du canvas
    constructor(x, y, label) {
        this.label = label ?? "p";
        this.x = x; //drawing coordinates - can be computed from chain
        this.y = y;
        this.ox = x; //original coordinates
        this.oy = y;
    }
    getLocalX(){
        if (this.previous!=null)
            return this.ox + this.previous.ro * Math.cos(this.previous.alpha);
        return this.ox;
    }
    getLocalY(){
        if (this.previous!=null)
            return this.oy + this.previous.ro * Math.sin(this.previous.alpha);
         return this.oy;
    }

    setNext(id, ro, alpha){
        this.next = {id:id,ro:ro,iro:ro, alpha:alpha}
    }

    changeNextAlpha(id, alpha){
        this.next.forEach(elt => {if (elt.id == id) {elt.alpha = alpha}});
    }

    changePreviousAlpha(id, alpha){
        this.previous.forEach(elt => {if (elt.id == id) {elt.alpha = alpha}});
    }

    setPrevious(id, ro, alpha){
        this.previous = {id:id,ro: ro, iro:ro,alpha:alpha}
    }

    printCoords() {
        console.log(this.x.toPrecision(2) + "," + this.y.toPrecision(2));
    }

    dist(p){
        let dx = p.ox - this.ox, dy = p.oy - this.oy;
        dx *= dx; dy *= dy;
        return Math.sqrt(dx + dy);
    }
}

const draw = {points:0, lines:1, lineLoop:2};

class Displayer {
    constructor(canvas, minX, maxX, minY, maxY, colors) {
    	this.canvas = canvas;
        if (canvas.getContext) {
            this.g2d = canvas.getContext('2d');
        } else {
            canvas.write("Votre navigateur ne peut visualiser cette page correctement");
        }

        this.sf = {
            x: 1,
            y: 1
        }; // scale factor en x et y 
        this.incr = 0.1; //grain de modification du scale factor
        this.lineWidth = 1;

        this.setColors(colors);

        //dimensions du display
        this.cWidth = this.canvas.width;
        this.cHeight = this.canvas.height;
        this.cCenterX = this.canvas.width / 2;
        this.cCenterY = this.canvas.height / 2;

        //dimensions des données à afficher
        this.minX = minX ?? 0;
        this.maxX = maxX ?? this.canvas.width;
        this.minY = minX ?? 0;
        this.maxY = maxX ?? this.canvas.height;
        let delta = this.maxX - this.minX;
        this.init();
    }

    setColors(colors){
    	this.colors = {fg:"#FEE",txt:"#FFF",bg:"#999"};
    	if (colors) {
        	this.colors.fg = colors[0] ?? this.colors.fg;
        	this.colors.txt = colors[1] ?? this.colors.txt;
        	this.colors.bg = colors[2] ?? this.colors.bg;
        }
    }

    init() {
        this.g2d.clearRect(0, 0, this.cWidth, this.cHeight);
        this.g2d.fillStyle = this.colors.bg;
        this.g2d.fillRect(0, 0, this.cWidth, this.cHeight);
     }

    xm2p(x) {
        return this.cWidth * (x - this.minX) / (this.maxX - this.minX);
    }

    ym2p(y) {
        return this.cHeight * (y - this.maxY) / (this.minY - this.maxY);
    }

    //dessin point avec coordonnées "mathématique" transformées en coordonnées canvas
    mDrawPoint(p) {
        this.g2d.beginPath();
        let x = this.xm2p(p.x),
            y = this.ym2p(p.y);
        
        this.g2d.arc(x, y, 12, 0, Math.PI * 2, true);
        //this.g2d.fillrect(x, y, 15, 0, Math.PI * 2, true);
 
        this.g2d.fillStyle = this.colors.bg;
        this.g2d.fill();
        this.g2d.strokeStyle = this.colors.fg;
        this.g2d.stroke();

        this.g2d.fillStyle = this.colors.txt;
        let coords = "(" + p.x.toPrecision(2) + "," + p.y.toPrecision(2) + ")";
        let dx = p.label.length * 3,
            dy = 2;   
        this.g2d.fillText(p.label, x - dx, y + dy);
        dy = 23;
        dx = coords.length * 3;    
        //this.g2d.fillText(coords, x - dx, y + dy);
    }

    //dessin segment entre points en coordonnées cartésiennes
    mDrawLine(p1, p2, color) {
        this.g2d.strokeStyle = color ?? this.colors.fg;
        this.g2d.beginPath();
        this.g2d.moveTo(this.xm2p(p1.x), this.ym2p(p1.y));
        this.g2d.lineWidth = this.lineWidth;
        this.g2d.lineJoin = "round";
        this.g2d.lineTo(this.xm2p(p2.x), this.ym2p(p2.y));
        this.g2d.stroke();
        this.g2d.strokeStyle = this.colors.fg;
    }

    //dessin segment entre points en coordonnées "canvas" 
    pDrawLine(x1, y1, x2, y2) {
        this.g2d.strokeStyle = this.colors.fg;
        this.g2d.beginPath();
        this.g2d.moveTo(x1, y1);
        this.g2d.lineTo(x2, y2);
        this.g2d.stroke();
    }

    setOptions(points, colors, incrementalDrawing) {
    	this.points = points ?? this.points;
     	this.incrementalDrawing = incrementalDrawing ?? false;
        this.setColors(colors);
        if(!incrementalDrawing) this.init();
    }

    drawPoints(points, colors, incrementalDrawing) {
     	this.setOptions(points, colors, incrementalDrawing);
       	let n = points.length;
        for (let i = 0; i < n; i++) {
        	this.mDrawPoint(points[i], colors);
        }
    }

    drawLineStrip(points, colors, incrementalDrawing) {
        this.setOptions(points, colors, incrementalDrawing);
        let n = points.length;
        for (let i = 0; i < n - 1; i++) {
            this.mDrawLine(points[i], points[(i + 1)], colors);
         }
    }

    drawLineLoop(points, colors, incrementalDrawing) {
     	this.setOptions(points, colors, incrementalDrawing);
        let n = points.length;
        for (let i = 0; i < n; i++) {
            this.mDrawLine(points[i], points[(i + 1) % n], colors);
        }
    }

    drawLines(points, colors, incrementalDrawing) {
     	this.setOptions(points, colors, incrementalDrawing);
        let n = points.length;
        for (let i = 0; i < n - 1; i += 2) {
            this.mDrawLine(points[i], points[(i + 1) % n], colors);
            this.mDrawPoint(points[i], colors);
            this.mDrawPoint(points[i + 1], colors);
        }
    }
};