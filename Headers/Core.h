/**
 * 3407ICT Graphics Programming
 * Tutorial Graphics Rendering Framework
 * @author Xavier Ho (contact@xavierho.com)
 */
#pragma once

#include "Primitives.h"

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

	 /** The mesh for a cube */
	 Mesh cube;

	 /** rotation angle */
	 float angle;

public:
    /** Constructor. */
    Core(int width=512, int height=512, bool fullscreen=false);

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

    /** Handles user events. */
    void handleEvents();

	 void drawCube(Mesh & mesh, int i);
	 //void drawCube(Mesh & mesh, int i, int b);

};
