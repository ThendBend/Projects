var s;
var scl = 20;
var food;
var score;
var prevScore;


function setup() {
	score = createDiv("Score: 0")
	score.position(10,10)
	score.style("color","white")
  	createCanvas(600, 600)
    s = new Snake();
    frameRate(10);
    pickLocation();
}

function pickLocation()	{
	var cols = floor(width/scl);
	var rows = floor(height/scl);
	food = createVector(floor(random(cols)), floor(random(rows)));
	food.mult(scl);
}

function draw() {
	background(51);
	score.html("Score: "+s.total)
	s.lose();
	s.update();
	s.show();

	if(s.eat(food))	{
		pickLocation();
	}

	fill(255, 0, 100);
	rect(food.x, food.y, scl, scl);
}

function keyPressed() {
	if(keyCode === UP_ARROW && s.yspeed != 1) {
		s.dir(0,-1);
	}else if (keyCode === DOWN_ARROW && s.yspeed != -1) {
		s.dir(0,1);
	}else if (keyCode === RIGHT_ARROW && s.xspeed != -1) {
		s.dir(1,0);
	}else if (keyCode === LEFT_ARROW && s.xspeed != 1) {
		s.dir(-1,0);
	}
}
