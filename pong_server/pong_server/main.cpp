#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include "pong.h"

using namespace std;

webSocket server;
Pong* game;
bool gameState = false;
int currentID;


/* called when a client connects */
void openHandler(int clientID){
    vector<int> clientIDs = server.getClientIDs();
	if (clientIDs.size() > 1){
		std::cout << "Connection from: " << clientID << " refused." << std::endl;
		server.wsSend(clientID, "Someone is already playing pong.  Connection refused.");
		server.wsClose(clientID);
	}
	else{
		std::cout << "Connection from: " << clientID << " opened." << std::endl;
		currentID = clientID;
	}
}

/* called when a client disconnects */
void closeHandler(int clientID){
	std::cout << "Connection from: " << clientID << " closed." << std::endl;
	vector<int> clientIDs = server.getClientIDs();
	if (clientIDs.empty()){
		gameState = false;
		delete game;
	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){
	if (gameState && clientID == currentID){
		game->movePlayer(std::stoi(message));
	}
	else{
		std::ostringstream os;
		os << "Welcome " << message << "!";
		server.wsSend(clientID, os.str());
		game = new Pong(800, 600);
		gameState = true;
	}
}

/* called once per select() loop */
void periodicHandler(){
	if (gameState){
		game->update();
		vector<int> clientIDs = server.getClientIDs();
		if (!clientIDs.empty()){
			server.wsSend(clientIDs[0], game->getData());
		}
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
