#ifndef Camera_h
#define Camera_h

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define UP 4
#define DOWN 5

#define FPS 0
#define THIRD_PERSON 1
#include "Vec3.h"


class Camera
{
	public:
		Camera(int mode);
		virtual ~Camera();
		void move(int direction);
		void look(int direction);
		void setPosition(Vec3 v);
		void load();
		void setSpeed(float speed);
		void setViewAngle(float x_angle, float y_angle, float z_angle);
		void handleMouse(int x, int y);

	private:
		Vec3 position, rotation;
		float speed;
		int mode;
		bool first;
};
#endif
