#pragma once

// http://www.libsdl.org/
#include "SDL.h"
// http://www.libsdl.org/projects/SDL_ttf/
#include "SDL_ttf.h"
// http://www.libsdl.org/projects/SDL_image/
#include "SDL_image.h"
// http://glew.sourceforge.net/
#include "glew.h"
#include "Vec3.h"
#include "Primitives.h"
#include "Camera.h"
#include "Pacman.h"

// Include your headers here
// #include "Primitives.h"
#include <list>
#include <vector>

/**
 * Top-tier class, handles mainloop, events, and other classes.
 * This class has been designed with a minimal style in mind.
 */
class Core
{
    /** Width and height of the rendering window. */
    int width, height;

    /** fullscreen flag */
    bool fullscreen;

    /** The amount of time passed after each frame */
    double elapsedTime;

    /** Mainloop control toggle */
    bool running;

    /** Height map array */
    std::vector<float> terrainHeights;

    /** List of GameEntitys */ // Optional but helpful
    std::vector<GameEntity*> objects;

	 /**
	  Get a height from the heightmap using nice x and y coordinates

	  \param heights the heightmap
	  \param x x coordinate of desired heightmap value.
	  \param y y coordinate of desired heightmap value (this should be your z value really)
	  \param size the width or height of the height map (same size)
	  \return heightmap value at x and y
	 */

	 Camera camera;

	 Terrain terrain;

	 std::vector<Vec3> colors;

	 bool keys[256];

	 Pacman * player;

public:
    /** Constructor. */
    Core(int width=800, int height=600, bool fullscreen=false);

    /** Destructor */
    virtual ~Core();

    /** Starts the main loop. */
    void start();

protected:
    /** Sets up rendering context. */
    void initialise();

    /** Prepares objects for rendering. */
    void preprocess();

    /** Draws to screen. */
    void render();

    /** Free any OpenGL resources here */
    void cleanup();

    /** Handles user events. */
    void handleEvents();

    /** Generates a square ground plane, 1x1, with even tessellation into a VBO.
        xDiv and yDiv determines the number of polygon divisions on the plane.
        heights is the height map array.
        Result is stored in obj. */ //                       v- You may also use std::vector<float>
    void createTerrain(int xDiv, int zDiv, GameEntity* _terrain, std::vector<float> heights);

    /** Copies the data from the height map into memory. */
    void fillTerrainHeights(int xDiv, int zDiv);

    Vec3 getpixel24(SDL_Surface *surface, int x, int y);
};
