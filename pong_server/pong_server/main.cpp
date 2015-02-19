#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include "pong.h"

using namespace std;

struct game_players{
	int p1clientid;
	int p2clientid;
	std::string p1gameid;
	std::string p2gameid;
};

webSocket server;
Pong* game;
bool gameState = false;
game_players user;

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
		if (clientID == user.p1clientid){
			game->movePlayer(0, std::stoi(message));
		}
		else if (clientID == user.p2clientid){
			game->movePlayer(1, std::stoi(message));
		}
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
