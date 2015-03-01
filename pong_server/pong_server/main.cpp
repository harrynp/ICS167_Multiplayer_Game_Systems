#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <chrono>
//#include <time.h>
#include "websocket.h"
#include "pong.h"

using namespace std;

struct game_players{
	int p1clientid;
	int p2clientid;
	std::string p1gameid;
	std::string p2gameid;
};

struct game_message{
	unsigned int player;
	unsigned long long time;
	unsigned long long timeSent;
	unsigned int keyCode;
	unsigned int delay;
};

webSocket server;
Pong* game;
bool gameState = false;
game_players user;
std::queue<game_message> messageLatency;
bool artificial_latency = true;


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

/* called when a client connects */
void openHandler(int clientID){
    vector<int> clientIDs = server.getClientIDs();
	if (clientIDs.size() > 2){
		std::cout << "Connection from: " << clientID << " refused." << std::endl;
		server.wsSend(clientID, "2 people are already playing pong.  Connection refused.");
		server.wsClose(clientID);
	}
	else{
		std::cout << "Connection from: " << clientID << " opened." << std::endl;
		if (clientIDs.size() == 2){
			user.p1clientid = clientIDs[0];
			user.p2clientid = clientIDs[1];
		}
	}
}

/* called when a client disconnects */
void closeHandler(int clientID){
	std::cout << "Connection from: " << clientID << " closed." << std::endl;
	vector<int> clientIDs = server.getClientIDs();
	server.wsSend(clientIDs[0], "Connection closed because other player has disconnected.");
	server.wsClose(clientIDs[0]);
	if (clientIDs.size() < 2){
		gameState = false;
		delete game;
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){
	if (gameState){
		std::vector<string> split_msg = split(message, ',');
		//if (clientID == user.p1clientid){
		//	game->movePlayer(0, std::stoi(split_msg[1]));
		//}
		//else if (clientID == user.p2clientid){
		//	game->movePlayer(1, std::stoi(split_msg[1]));
		//}
		
		//Artificial Message Latency
		unsigned int player;
		if (clientID == user.p1clientid){
			player = 0;
		}
		else if (clientID == user.p2clientid){
			player = 1;
		}
		game_message move;
		move.player = player;
		move.timeSent = std::stoull(split_msg[0]);
		move.time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		move.keyCode = std::stoi(split_msg[1]);
		
		move.delay = artificial_latency ? (rand() % 3 + 1) * 100 : 0; //random 100 to 300 millisecond latency added
		messageLatency.push(move);
	}
	else{
		if (clientID == user.p1clientid){
			user.p1gameid = message;
		}
		else if (clientID == user.p2clientid){
			user.p2gameid = message;
		}
		std::ostringstream os;
		os << "Welcome " << message << "!";
		server.wsSend(clientID, os.str());
		vector<int> clientIDs = server.getClientIDs();
		if (clientIDs.size() == 2){
			game = new Pong(800, 600);
			gameState = true;
		}
	}
}

/* called once per select() loop */
void periodicHandler(){
	vector<int> clientIDs = server.getClientIDs();
	if (gameState && clientIDs.size() == 2){
		if (!messageLatency.empty()){
			auto time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			if (time >= messageLatency.front().time + messageLatency.front().delay){
				game->movePlayer(messageLatency.front().player, messageLatency.front().keyCode, time - messageLatency.front().timeSent);
				messageLatency.pop();
			}
		}

		game->update();
		std::ostringstream os = game->getData();
		os << "," << user.p1gameid << "," << user.p2gameid;
		server.wsSend(user.p1clientid, os.str());
		server.wsSend(user.p2clientid, os.str());
	}
}

int main(int argc, char *argv[]){
    int port;

    cout << "Please set server port: ";
    cin >> port;

    /* set event handler */
    server.setOpenHandler(openHandler);
    server.setCloseHandler(closeHandler);
    server.setMessageHandler(messageHandler);
    server.setPeriodicHandler(periodicHandler);

    /* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
    server.startServer(port);

    return 1;
}
