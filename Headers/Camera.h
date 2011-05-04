#ifndef Camera_h
#define Camera_h
#define TILT_UP 0
#define TILT_DOWN 1
#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define UP 4
#define DOWN 5
#include "Transformation.h"


class Camera
{
	public:
		Camera();
		virtual ~Camera();
		void move(int direction);
		void look(int direction);
		void setPosition(Vec4 v);
		void setTarget(Vec4 v);
		void setUp(Vec4 v);
		void load();
		void setSpeed(float speed);

	private:
		Vec4 target, position, up, right;
		float speed;
};
#endif
