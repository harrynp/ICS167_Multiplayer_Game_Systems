var WIDTH = 800;
var HEIGHT = 600;

var canvas;
var context;

var ping = {
	p1ping: 0,
	p2ping: 0,
	update: function(p1p, p2p){
		this.p1ping = p1p;
		this.p2ping = p2p;
	},
	draw: function(){
		context.font = "24px ariel";
		context.textAlign = 'center';
		context.fillText("Ping: " + this.p1ping + " ms        " + "Ping: " + this.p2ping + " ms", WIDTH / 2, 100);
	}
};

var score = {
	p1gameid: null,
	p2gameid: null,
	p1: 0,
	p2: 0,
	update: function(p1id, p2id, p1score, p2score){
		this.p1gameid = p1id;
		this.p2gameid = p2id;
		this.p1 = p1score;
		this.p2 = p2score;
	},
	draw: function(){
		context.font = "24px ariel";
		context.textAlign = 'center';
		context.fillText(this.p1gameid + ": " + this.p1 + "        " + this.p2gameid + ": " + this.p2, WIDTH / 2, 50);
	}
};

var player1 = {
	x: null,
	y: null,
	width: 20,
	height: 100,
	update: function(x, y, offset){
		this.x = x;
		this.y = y;
		this.y += offset;
		this.y = Math.max(Math.min(this.y, 600 - this.height), 0);
	},

	draw: function(){
		context.fillRect(this.x, this.y, this.width, this.height);
	}
};

var player2 = {
	x: null,
	y: null,
	width: 20,
	height: 100,
	update: function(x, y, offset){
		this.x = x;
		this.y = y;
		this.y += offset;
		this.y = Math.max(Math.min(this.y, 600 - this.height), 0);
	},

	draw: function(){
		context.fillRect(this.x, this.y, this.width, this.height);
	}
};

var ball = {
	x: null,
	y: null,
	radius: 20,
	update:function(x, y){
		this.x = x;
		this.y = y;
	},
	draw: function(){
		context.beginPath();
		context.arc(this.x, this.y, this.radius, 0, 2 * Math.PI);
		context.fillStyle = "white";
		context.fill();
		context.stroke();
	}
	
};

function main(){
	canvas = document.createElement("canvas");
	canvas.width = WIDTH;
	canvas.height = HEIGHT;
	context = canvas.getContext("2d");
	$('#game_canvas').append(canvas);
	player1.x = player1.width;
	player1.y = (HEIGHT - player1.height) / 2;
	player2.x = WIDTH - player2.width * 2;
	player2.y = (HEIGHT - player2.height) / 2;
	ball.x = WIDTH / 2;
	ball.y = HEIGHT / 2;
	
	var loop = function(){
		draw();
		window.requestAnimationFrame(loop, canvas);
	};
	window.requestAnimationFrame(loop, canvas);
}


function draw(){
	context.fillRect(0, 0, WIDTH, HEIGHT);
	
	context.save();
	context.fillStyle = "white";
	ball.draw();
	player1.draw();
	player2.draw();
	score.draw();
	ping.draw();
	var w = 4;
	var x = (WIDTH - w) / 2;
	var y = 0;
	
	// For 2 players later
	context.fillRect(x, y, w, HEIGHT);
	
	context.restore();
}
main();