#include "pong.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>

Pong::Pong(unsigned int width, unsigned int height){
	this->width = width;
	this->height = height;
	init();
}

Pong::~Pong(){}

void Pong::init(){
	p1.x = p1.width;
	p1.y = (height - p1.height) / 2;
	p2.x = width - p2.width * 2;
	p2.y = (height - p2.height) / 2;
	b.x = width / 2;
	b.y = height / 2;
	b.v.x = b.speed;
	b.v.y = 0;
}

void Pong::movePlayer(unsigned int user, unsigned int keyCode){
	player *p = nullptr;
	if (user == 0){
		p = &p1;
	}
	else if (user == 1){
		p = &p2;
	}
	if (p != nullptr){
		if (keyCode == 38){
			p->y -= 25;
		}
		else if (keyCode == 40){
			p->y += 25;
		}
		p->y = fmax(fmin(p->y, height - p->height), 0);
	}
}

bool Pong::AABBIntersect(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh){
	return ax < bx + bw && ay < by + bh && bx < ax + aw && by < ay + ah;
}

void Pong::update(){
	b.x += b.v.x;
	b.y += b.v.y;

	if (b.radius > b.y || b.y + b.radius > height){
		b.v.y *= -1;
	}

	if (b.x + b.radius > width){
		b.v.x *= -1;
	}

	if (AABBIntersect(p1.x, p1.y, p1.width, p1.height, b.x - b.radius, b.y - b.radius, b.radius * 2, b.radius * 2)){
		double n = (b.y + b.radius - p1.y) / (p1.height + b.radius);
		double theta = M_PI / 4 * (2 * n - 1);
		double smash = abs(theta) > .1 * M_PI ? 1.5 : 1;
		b.v.x = smash * b.speed * cos(theta);
		b.v.y = smash * b.speed * sin(theta);
	}

	if (AABBIntersect(p2.x, p2.y, p2.width, p2.height, b.x - b.radius, b.y - b.radius, b.radius * 2, b.radius * 2)){
		double n = (b.y + b.radius - p2.y) / (p2.height + b.radius);
		double theta = M_PI / 4 * (2 * n - 1);
		double smash = abs(theta) > .1 * M_PI ? 1.5 : 1;
		b.v.x = -1 * smash * b.speed * cos(theta);
		b.v.y = -1 * smash * b.speed * sin(theta);
	}
	if (b.x + b.radius < 0){
		//restarts game and increments score of player 2 if the ball hits the left border
		++s.p2;
		init();
	}
	if (b.x  + b.radius > width){
		//restarts game and increments score of player 1 if the ball hits the right border
		++s.p1;
		init();
	}
}

std::ostringstream Pong::getData(){
	std::ostringstream os;
	//os << "{ \"player\" : {\"x\" : " << p1.x << ", \"y\" : " << p1.y << "}, \"ball\" : {\"x\" : " << b.x << ", \"y\" : " << b.y << "}" << ", \"score\" : {\"current\" : " << s.current << ", \"high\" : " << s.high << "}}";
	os << p1.x << "," << p1.y << "," << p2.x << "," << p2.y << "," << b.x << "," << b.y << "," << s.p1 << "," << s.p2;
	return os;
}