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
	s.current = 0;
	b.x = width / 2;
	b.y = height / 2;
	b.v.x = b.speed;
	b.v.y = 0;
}

void Pong::movePlayer(int keyCode){
	if (keyCode == 38){
		p1.y -= 25;
	}
	else if (keyCode == 40){
		p1.y += 25;
	}
	p1.y = fmax(fmin(p1.y, height - p1.height), 0);
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
		//Checks if ball hits paddle
		if (b.v.x < 0){
			//only increments the score once
			s.current += 1;
			if (s.high < s.current){
				//updates high score if current score is higher
				s.high = s.current;
			}
		}
		double n = (b.y + b.radius - p1.y) / (p1.height + b.radius);
		double theta = M_PI / 4 * (2 * n - 1);
		double smash = abs(theta) > .1 * M_PI ? 1.5 : 1;
		b.v.x = smash * b.speed * cos(theta);
		b.v.y = smash * b.speed * sin(theta);
	}
	if (b.x + b.radius < 0){
		//restarts game if the ball hits the left border
		init();
	}
}

std::string Pong::getData(){
	std::ostringstream os;
	//os << "{ \"player\" : {\"x\" : " << p1.x << ", \"y\" : " << p1.y << "}, \"ball\" : {\"x\" : " << b.x << ", \"y\" : " << b.y << "}" << ", \"score\" : {\"current\" : " << s.current << ", \"high\" : " << s.high << "}}";
	os << p1.x << "," << p1.y << ", " << b.x << ", " << b.y << ", " << s.current << ", " << s.high;
	return os.str();
}