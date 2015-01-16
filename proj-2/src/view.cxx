/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 2 sample code (Winter 2014)
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
    this->bumpMapping	= false;

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

/* InitRenderer:
 */
void View::InitRenderer ()
{
    this->render.Init (std::string(DATA_DIR "/shader"));

}

//! the directional light's direction in world coordinates
static cs237::vec4f DirLight(1.0f, 1.0f, -0.2f, 0.0f);

//! the point light's position in world coordinates
static cs237::vec4f PointLight(-0.85f, 0.85f, 0.0f, 1.0f);

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

    this->render.Enable();
    cs237::setUniform (this->render.projLoc, this->projectionMat);
    cs237::setUniform (this->render.mvLoc, this->modelViewMat);
    cs237::setUniform (this->render.lightingModeLoc, static_cast<int>(this->lighting));
    cs237::setUniform (this->render.colorTxLoc, 0);  // texture unit 0
    cs237::setUniform (this->render.bumpTxLoc, 1);  // texture unit 1

  // set lighting information
    if (this->lighting == DIR_LIGHT) {
	cs237::setUniform (this->render.lightLoc,
	    normalize(cs237::vec3f(this->modelViewMat * DirLight)));
    }
    else if (this->lighting == POINT_LIGHT) {
      // transform the world coordinates of the light to eye coordinates.
	cs237::setUniform (this->render.lightLoc,
	    cs237::vec3f(this->modelViewMat * PointLight));
    }

  /* Draw the box */
    if ((! this->bumpMapping) || (this->lighting == NO_LIGHT))
	cs237::setUniform (this->render.texturingModeLoc, TEXTURE);
    else
	cs237::setUniform (this->render.texturingModeLoc, BUMP_MAPPING);
    for (int i = 0;  i < NUM_SIDES;  i++)
	DrawWall (this, static_cast<Side>(i));

  /* draw the current state of the balls */
    cs237::setUniform (this->render.texturingModeLoc, NO_TEXTURE);
    for (int i = 0;  i < Ball::NumBalls; i++) {
	DrawBall (this, now, Ball::Balls[i]);
    }

}

