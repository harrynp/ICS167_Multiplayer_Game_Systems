var WIDTH = 800;
var HEIGHT = 600;

var canvas;
var context;

var score = {
	current: 0,
	high: 0,
	update: function(current, high){
		this.current = current;
		this.high = high;
	},
	draw: function(){
		context.font = "24px ariel";
		context.textAlign = 'center';
		context.fillText("Current Score: " + this.current + " High Score: " + this.high, WIDTH / 2, 50);
	}
};

var player = {
	x: null,
	y: null,
	width: 20,
	height: 100,
	update: function(x, y){
		this.x = x;
		this.y = y;
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
	player.x = player.width;
	player.y = (HEIGHT - player.height) / 2;
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
	player.draw();
	score.draw();
	var w = 4;
	var x = (WIDTH - w) / 2;
	var y = 0;
	
	//For 2 players later
	// context.fillRect(x, y, w, HEIGHT);
	
	context.restore();
}
main();