#pragma once

// http://www.libsdl.org/
#include "SDL.h"
// http://www.libsdl.org/projects/SDL_ttf/
#include "SDL_ttf.h"
// http://www.libsdl.org/projects/SDL_image/
#include "SDL_image.h"
// http://glew.sourceforge.net/
#include "glew.h"

#include "Primitives.h"
#include "Camera.h"

// Include your headers here
// #include "Primitives.h"
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

    /** Images */
    SDL_Surface *terrainHeightMap;

    /** Objects */
    Object terrain;

    /** List of Objects */ // Optional but helpful
    std::vector<Object*> objects;

	unsigned int shaderProgram, vShader, fShader;

	Camera camera;

	void createVBOs();

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
    void createTerrain(int xDiv, int zDiv, Object* _terrain, std::vector<float> heights);

    /** Copies the data from the height map into memory. */
    void fillTerrainHeights(int xDiv, int zDiv);

    Vec4 getpixel24(SDL_Surface *surface, int x, int y);
};
