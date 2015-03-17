var server;
var gameState = false;;
var upArrow = 38;
var downArrow = 40;
var debug = false;
var offset = 0;

function log( text ) {
	$log = $('#log');
	//Add text to log
	$('#log').append(($log.val()?"\n":'')+text);
	//Autoscroll
	$('#log')[0].scrollTop = $('#log')[0].scrollHeight - $('#log')[0].clientHeight;
}

function send( text ) {
	server.send( 'message', text);
}

function connect(){
	log('Connecting...');
	server = new FancyWebSocket('ws://' + document.getElementById('ip').value + ':' + document.getElementById('port').value);
	$(document).keydown(function(event) {
		if ( event.which == upArrow || event.which == downArrow ) {
			var d = new Date();
			send( d.getTime() + "," + event.which );
			offset += event.which == upArrow ? -15 : 15;
		}
	});

	//Let the user know we're connected
	server.bind('open', function() {
		document.getElementById("cntBtn").disabled = true;
		log( "Connected." );
		send(document.getElementById('gameid').value);
	});

	//OH NOES! Disconnection occurred.
	server.bind('close', function( data ) {
		document.getElementById("cntBtn").disabled = false;
		log( "Disconnected." );
	});

	//Log any messages sent from server
	server.bind('message', function( payload ) {
		if(payload == "Connection closed because other player has disconnected."){
			log(payload);
		}
		else if(payload.indexOf("Move confirmed") > -1){
			var res = payload.split(",");
			offset += res[1] == upArrow ? 15 : -15;
		}
		else if(payload.indexOf("New Game") > -1){
			offset = 0;
		}
		else if(gameState){
			if(debug){
				log(payload);
			}
			var res = payload.split(",");
			
			//var d = new Date();
			//var ct = Math.floor(d.getTime() / 1000);
			//var ping = ct - res[10];

			ball.update(res[4],res[5]);
			if(res[12] == 0){
				player1.update(parseInt(res[0]),parseInt(res[1]), offset);
				player2.update(parseInt(res[2]),parseInt(res[3]), 0);
			}
			else{
				player1.update(parseInt(res[0]),parseInt(res[1]), 0);
				player2.update(parseInt(res[2]),parseInt(res[3]), offset);
			}
			// player1.update(res[0],res[1]);
			// player2.update(res[2],res[3]);
			score.update(res[10], res[11], res[6],res[7]);
			ping.update(res[8], res[9]);
			
		}
		else{
			log(payload);
			gameState = true;
		}
	});

	server.connect();
}