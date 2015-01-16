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
#include "plane.hxx"
#include "character.hxx"
#include "guard.hxx"
#include <sstream>

/* default motion parameters */
#define DEFAULT_X_ROT	0.0f
#define DEFAULT_Y_ROT	0.0f
#define DEFAULT_Z_ROT	0.0f
#define DEFAULT_SCALE	0.35f

/* clip planes in world coordinates */
#define NEAR_Z		0.2f
#define FAR_Z		100.0f
#define FOVY		65.0f	/* field-of-view in Y-dimension */

/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef DATA_DIR
#  define DATA_DIR "../data"
#endif

//! the directional light's direction in world coordinates
static cs237::vec4f DirLight(1.0, 1.0, 0.5, 0.0);

// build Light's View Matrix
static cs237::mat4f light_view=cs237::lookAt (cs237::vec3f(DirLight),
                                              cs237::vec3f(0.0,0.0,0.0),
                                              cs237::vec3f(0.0,1.0,0.0));

//build light's Projection Matrix
static cs237::mat4f light_proj=cs237::ortho (-41.0f,41.0f,
                                             -41.0f,41.0f,
                                             -35.0f,45.0f);
//Bias Matrix... scaling and translating
static cs237::mat4f BiasMat=cs237::mat4f(0.5f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 0.5f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 0.5f, 0.0f,
                                         0.5f, 0.5f, 0.49f, 1.0f);


/* InitView:
 *
 * Program initialization.
 */
void View::Init (int argc, const char **argv)
{
    this->wid = 800;
    this->ht = 800;
    this->isVis = GL_TRUE;
	
	/* initialize the motion parameters */
    this->xRot = DEFAULT_X_ROT;
    this->yRot = DEFAULT_Y_ROT;
    this->zRot = DEFAULT_Z_ROT;
    this->xLastIncr = 0;
    this->yLastIncr = 0;
    this->fInertia = cs237::vec2f(-0.5f, 0.0f);
    this->fScale = DEFAULT_SCALE;
    this->xLast = -1;
    this->yLast = -1;
    this->rotate = true;
    this->mouseTracking = false;
	
	/* initialize the camera */
    this->camPos	= cs237::vec3f(0.0, 15.0, 40.0);
    this->camTarget	= cs237::vec3f(0.0, 15.0, 0.0);
    this->camUp		= cs237::vec3f(0.0, 1.0, 0.0);
	
	/* initialize the rendering mode */
    this->renderMode = SKELETON;
	
	/* initialize the animation clock */
    this->lastDraw	= glfwGetTime();;
    this->lastFrame = 0;
    this->lastT = 0.0;
	
    this->shouldExit	= false;
    this->needsRedraw	= true;
	
    this->guard = nullptr;  // cannot initialize until after GL context is created!
    this->floor = new Plane {
	    cs237::color3ub{ 128, 204, 128 },		// color
	    cs237::vec3f{  0.0f,  1.0f,  0.0f },	// norm
	    cs237::vec3f{  40.0f, 0.0f, -40.0f },	// corners
	    cs237::vec3f{ -40.0f, 0.0f, -40.0f },
	    cs237::vec3f{ -40.0f, 0.0f,  40.0f },
	    cs237::vec3f{  40.0f, 0.0f,  40.0f }
	};
	
}

/* Reset:
 *
 * Reset view parameters
 */
void View::Reset ()
{
    this->xRot    	= DEFAULT_X_ROT;
    this->yRot    	= DEFAULT_Y_ROT;
    this->zRot    	= DEFAULT_Z_ROT;
    this->xLastIncr	= 0;
    this->yLastIncr	= 0;
    this->fInertia	= cs237::vec2f(-0.5f, 0.0f);
    this->fScale	= DEFAULT_SCALE;
}

/* BindFrameBuffer:
 *
 * initialize the framebuffer back to the screen.
 */
void View::BindFrameBuffer()
{
    int fwid, fht;
    CS237_CHECK (glBindFramebuffer (GL_FRAMEBUFFER, 0) );
    CS237_CHECK (glfwGetFramebufferSize (this->window, &fwid, &fht) );
    CS237_CHECK (glViewport (0, 0, fwid, fht) );
	
}

//! \brief initialze the guard data; this must be done after the OpenGL context has been created
void View::InitGuard ()
{
    if (this->guard == nullptr)
		this->guard = new Guard();
	
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
											  NEAR_Z ,
											  FAR_Z);
	
}

/* InitModelViewMatrix:
 *
 * initialize the model-view matrix based on the view state.
 */
void View::InitModelViewMatrix ()
{
	
    this->viewMat = cs237::lookAt (
								   this->camPos,
								   this->camTarget,
								   this->camUp);
	
    this->modelViewMat = this->viewMat
	* cs237::rotateX(this->xRot)
	* cs237::rotateY(this->yRot)
	* cs237::rotateZ(this->zRot)
	* cs237::scale(this->fScale);
	
}

/* InitRenderer:
 */
void View::InitRenderer ()
{
	/***** This is placeholder code for initializing the renderers; you will need to update it *****/

    
    this->skeletonRender.Init (std::string(DATA_DIR "/default"));
    this->wireRender.Init (std::string(DATA_DIR "/default"));
    this->guardRender.Init (std::string(DATA_DIR "/default"));



}

/* Render:
 *
 * render the current state of the animation
 */
void View::Render ()
{
    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);
    glEnable (GL_DEPTH_TEST);
    glDepthMask (GL_TRUE);
	
	/* clear the screen */
    glClearColor (0.2f, 0.2f, 0.4f, 1.0f);	// clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    double now = glfwGetTime();
	
	/* compute the interpolation factor and frame indices */
    double t = this->lastT + (now - this->lastDraw) / GuardData->duration;  // scaled to frame duration
	
    int frame = this->lastFrame;
    while (t >= 1.0) {
		t -= 1.0;
		if (++frame >= GuardData->nFrames) frame = 0;
    }
    int nextFrame = frame+1;
	
    if (nextFrame >= GuardData->nFrames) nextFrame = 0;
    this->lastFrame = frame;
    this->lastDraw = now;
    this->lastT = t;
    assert ((0.0 <= t) && (t <= 1.0));
	
	/* Create a skeleton that interpolates between Guard1->frame[frame] and
	 * Guard1->frame[nextFrame] using t as the scale.
	 *
	 * You can do the interploation here or create a class/struct to contains a function that
	 * returns the interpolated skeleton for you.
	 */
    /***** YOUR CODE HERE *****/
    
    //allocated array of Joints for interpolated skeleton caluclation
    Skeleton_t inter_skel= new Joint[GuardData->nJoints];
    int n;
	
    
    //assigning each value of Joint in Joint array interpolated value based on [frame] and [nextFrame]
    for(n=0;n<GuardData->nJoints;n++){
        inter_skel[n].parent=GuardData->frame[frame][n].parent;
		
        //linear interpolation for position
        inter_skel[n].pos=mix(GuardData->frame[frame][n].pos,(float)t,GuardData->frame[nextFrame][n].pos);
		
        //spherical interpolation for orientations
        inter_skel[n].orient=slerp(GuardData->frame[frame][n].orient,(float)t,GuardData->frame[nextFrame][n].orient);
    };
    
	
    InitModelViewMatrix ();
	
	/* Render the animation */
    /***** YOUR CODE HERE *****/


    // handling of the different modes of rendering
    switch(this->renderMode){
        
        case SKELETON:
            this->guard->RenderSkeleton(this,inter_skel,SKELETON);
            this->RenderFloor();
            break;
        case WIREFRAME:
            this->guard->RenderWire(this,inter_skel,WIREFRAME);
            this->RenderFloor();
            break;
        case MODEL:
            this->guard->RenderGuard(this,inter_skel,MODEL);
            this->RenderFloor();
            break;
        case SHADOWS:
        
            //initialize depth buffer
            this->depthBuffer.Init(1024,1024);
            
            //first pass
            // bind data to depth buffer, send to "light" shader where 
            this->shadowRender.Init (DATA_DIR "/light");
            this->guard->BufferShadow(this,inter_skel,SHADOWS,light_view,light_proj);
        
            //second pass
           this->bRender.Init (DATA_DIR "/default");
           this->guard->RenderShadow(this,inter_skel,SHADOWS,light_view,light_proj);

            break;
    }
}

/* RenderFloor:
 */
void View::RenderFloor ()
{
    this->floorRender.Init (std::string(DATA_DIR "/default"));
    this->floorRender.Enable();
    
    
    //setting Uniforms
    cs237::setUniform (this->floorRender.projLoc, this->projectionMat);
    cs237::setUniform (this->floorRender.mvLoc, this->modelViewMat);
    cs237::setUniform(this->floorRender.lightPOV,light_proj*light_view);
    cs237::setUniform(this->floorRender.BiasMat,BiasMat);
    
    cs237::setUniform(this->floorRender.ShadowLoc,1);
    cs237::setUniform(this->floorRender.TextureLoc,0);

    cs237::setUniform(this->floorRender.isFloor,true);
    cs237::setUniform(this->floorRender.renderMode,this->renderMode);
    
    //drawing floor
    this->floor->Draw (this, this->floorRender);
}

//to be used when rendering shadows on the first pass
// shadowRender is used, so don't need to initialize/enable
void View::ShadowRenderFloor ()
{
    //setting Uniforms
    cs237::setUniform (this->shadowRender.projLoc, this->projectionMat);
    cs237::setUniform (this->shadowRender.mvLoc, this->modelViewMat);
    cs237::setUniform(this->shadowRender.lightPOV,light_proj*light_view);
    cs237::setUniform(this->shadowRender.BiasMat,BiasMat);
    
    cs237::setUniform(this->shadowRender.ShadowLoc,1);
    cs237::setUniform(this->shadowRender.TextureLoc,0);

    cs237::setUniform(this->shadowRender.renderMode,this->renderMode);
    cs237::setUniform(this->shadowRender.isFloor,true);

    //drawing floor
    this->floor->Draw (this,this->shadowRender);
}

