var server;
var gameState = false;;
var upArrow = 38;
var downArrow = 40;


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
			send( event.which );
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
		if(gameState){
			var res = payload.split(",");
			ball.update(res[2],res[3]);
			player.update(res[0],res[1]);
			score.update(res[4],res[5]);		
		}
		else{
			log(payload);
			gameState = true;
		}
	});

	server.connect();
}