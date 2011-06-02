#ifndef Camera_h
#define Camera_h

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define UP 4
#define DOWN 5
#define STOP 6

#define FPS 0
#define THIRD_PERSON 1
#include "Vec3.h"


/**
	Camera class for providing high-level camera
*/
class Camera
{
	public:
		/**
			Constructor.

			\param mode should be either FPS or THIRD_PERSON modes. Third person doesn't work
		*/
		Camera(int mode);

		/// Destructor
		virtual ~Camera();

		/**
			Move the camera without alterating the direction it faces.

			\param direction should be one of FORWARD, BACKWARD, LEFT, RIGHT, UP or DOWN.
		*/
		void move(int direction);

		/**
			Change the viewing angle, or the pitch of the camera

			\param direction should be either LEFT, RIGHT, UP or DOWN
		*/
		void look(int direction);

		/**
			Set the camera's position

			\param v x, y and z coordinates of the camera
		*/
		void setPosition(Vec3 v);

		/// Set up the modelview matrix in respect to the camera. This should be called each time in the render loop, before any drawing is done
		void load();

		/**
			Set the camera movement speed.

			\param speed the movement speed. 1 is a nice value.
		*/
		void setSpeed(float speed);

		/**
			Set the rotation angle, or the pitch of the camera.

			\param x_angle the angle to set the pitch on the x axis
			\param y_angle the angle to set the pitch on the y axis
			\param z_angle the angle to set the pitch on the z axis
		*/
		void setViewAngle(float x_angle, float y_angle, float z_angle);

		/**
			Handle mouse movement. Changes the pitch based on relative values

			\param x coordinate relative to previous x coordinate
			\param y coordinate relative to previous y coordinate
		*/
		void handleMouse(int x, int y);

	private:
		Vec3 position, rotation;
		float speed;
		int mode;
		bool first;
};
#endif
