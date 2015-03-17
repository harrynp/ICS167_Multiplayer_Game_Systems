#ifndef PONG_H
#define PONG_H
#include <string>

class Pong{
public:
	Pong(unsigned int width, unsigned int height);
	~Pong();
	void init();
	void movePlayer(unsigned int user, unsigned int keyCode, unsigned int latency);
	bool update();
	std::ostringstream getData();

private:
	bool AABBIntersect(double ax, double ay, double aw, double ah, double bx, double by, double bw, double bh);

	unsigned int width;
	unsigned int height;

	struct player{
		double x;
		double y;
		unsigned int width = 20;
		unsigned int height = 100;
		unsigned int latency = 0;
	};

	struct ball{
		double x;
		double y;
		struct velocity{
			double x;
			double y;
		};
		velocity v;
		double speed = 2;
		double radius = 20;
	};

	struct score{
		unsigned int p1 = 0;
		unsigned int p2 = 0;
	};

	player p1;
	player p2;
	ball b;
	score s;
};



#endif