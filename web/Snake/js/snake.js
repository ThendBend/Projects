function Snake() {
	this.x = 300;
	this.y = 300;
	this.xspeed = 0;
	this.yspeed = 0;
	this.total = 0; 
	this.tail = [];

	this.dir = function(x,y) {
		this.xspeed = x;
		this.yspeed = y;
	}

	this.lose = function() {
		for (var i = 0; i < this.tail.length; i++) {
			var pos = this.tail[i];
			var d = dist(this.x, this.y, pos.x, pos.y);
			if(d < 1) {
				this.x = 300;
				this.y = 300;
				this.xspeed = 0;
				this.yspeed = 0;
				prevScore = this.total;
				this.total = 0;
				this.tail = [];
				noLoop();
				score.html("GAME OVER! Your score: " + prevScore + " Please refresh the page to play again!")
			}
		}
		if (this.x === width-(scl-2) || this.y === height-(scl-2) || this.x === -scl+(scl-2) || this.y === -scl+(scl-2))
		{
			this.x = 300;
			this.y = 300;
			this.xspeed = 0;
			this.yspeed = 0;
			prevScore = this.total;
			noLoop();
			score.html("GAME OVER! Your score: " + prevScore + " Please refresh the page to play again!")
		}
	}

	this.eat = function(pos) {
		var d = dist(this.x, this.y, pos.x, pos.y);
		if(d < 1) {
			this.total++;
			return true;
		} else {
			return false;
		}
	}

	this.update = function() {
		if (this.total === this.tail.length) {
			for (var i = 0; i < this.tail.length-1; i++) {
				this.tail[i] = this.tail[i+1];
			}
        }
        this.tail[this.total-1] = createVector(this.x, this.y);

		this.x = this.x + this.xspeed*scl;
		this.y = this.y + this.yspeed*scl;
		this.x = constrain(this.x, -scl+(scl-2), width-(scl-2));
		this.y = constrain(this.y, -scl+(scl-2), height-(scl-2));
	}

	this.show = function() {
		fill(255);
		for (var i = 0; i < this.total; i++)	{
			rect(this.tail[i].x, this.tail[i].y, scl , scl);
		}

		rect(this.x, this.y, scl, scl);
	}
}