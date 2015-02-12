#ifndef PONG_H
#define PONG_H
#include <string>

class Pong{
public:
	Pong(unsigned int width, unsigned int height);
	~Pong();
	void init();
	void movePlayer(int keyCode);
	void update();
	std::string getData();

private:
	bool AABBIntersect(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh);

	unsigned int width;
	unsigned int height;

	struct player{
		double x;
		double y;
		unsigned int width = 20;
		unsigned int height = 100;
	};

	struct ball{
		double x;
		double y;
		struct velocity{
			double x;
			double y;
		};
		velocity v;
		double speed = 10;
		double radius = 20;
	};

	struct score{
		unsigned int current;
		unsigned int high = 0;
	};

	player p1;
	ball b;
	score s;

};



#endif