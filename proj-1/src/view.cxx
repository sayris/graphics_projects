/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "view.hxx"
#include "wall.hxx"
#include "ball.hxx"

/* clip planes in world coordinates */
#define NEAR_Z		-1.1f
#define FAR_Z		1.1f
#define FOVY		65.0f	/* field-of-view in Y-dimension */

/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef DATA_DIR
#  define DATA_DIR "../data"
#endif

/* InitView:
 *
 * Program initialization.
 */
void View::Init (int argc, const char **argv)
{
    this->wid = 800;
    this->ht = 800;
    this->isVis = GL_TRUE;

  /* initialize the camera */
    this->camPos	= cs237::vec3f(0.0, 0.0, -3.0);
    this->camDir	= cs237::vec3f(0.0, 0.0, 1.0);
    this->camUp		= cs237::vec3f(0.0, 1.0, 0.0);
    
  /* initialize the animation clock */
    this->lastDraw	= 0.0;
    this->lastStep	= 0.0;
    this->lighting	= NO_LIGHT;

    this->shouldExit	= false;
    this->needsRedraw	= true;
}

/* InitProjMatrix:
 *
 * initialize the projection matrix based on the view state.
 */
void View::InitProjMatrix ()
{
    this->projectionMat = cs237::perspective (
	FOVY,
	(float)this->wid / (float)this->ht,
	NEAR_Z - this->camPos[2],
	FAR_Z - this->camPos[2]);

}

/* InitModelViewMatrix:
 *
 * initialize the model-view matrix based on the view state.
 */
void View::InitModelViewMatrix ()
{
    this->modelViewMat = cs237::lookAt (
	this->camPos,
	this->camPos + this->camDir,
	this->camUp);

}

/* InitShaders:
 */
void View::InitRenderers ()
{
    this->lighting = NO_LIGHT;

    this->noLightRender.Init (std::string(DATA_DIR "/no-light"));
    this->dirLightRender.Init (std::string(DATA_DIR "/dir-light"));

}

/* Render:
 *
 * render the current state of the animation
 */
void View::Render ()
{
    double now = glfwGetTime();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

  /* clear the screen */
    glClearColor (0.2f, 0.2f, 0.4f, 1.0f);	// clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Renderer *r;
    switch (this->lighting) {
      case NO_LIGHT:
	r = &this->noLightRender;
	break;
      case DIR_LIGHT:
	r = &this->dirLightRender;
	break;
    }
    r->Enable();
    cs237::setUniform (r->projLoc, this->projectionMat);
    cs237::setUniform (r->mvLoc, this->modelViewMat);

  /* Draw the box */
    for (int i = 0;  i < NUM_SIDES;  i++)
	DrawWall (this, r, static_cast<Side>(i));
 
    
  /* draw the current state of the balls */
    for (int i = 0;  i < Ball::NumBalls; i++) {
        DrawBall (this, r, now, Ball::Balls[i]);

    }

}

