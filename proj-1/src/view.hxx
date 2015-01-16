/*! \file view.hxx
 *
 * \brief type definitions for tracking the view state.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "render.hxx"

/*! \brief The current state of the view */
struct View {
    bool		shouldExit;
    bool		needsRedraw;
  // Camera state
    cs237::vec3f	camPos;		//!< camera position in world space
    cs237::vec3f	camDir;		//!< camera direction in world space
    cs237::vec3f	camUp;		//!< camera up vector in world space
  // animation state
    double		lastStep;	//!< time of last simulation step
    double		lastDraw;	//!< time of last redraw
  // view info 
    cs237::mat4f	modelViewMat;	//!< the current model-view matrix
    cs237::mat4f	projectionMat;	//!< the current projection matrix
    int			wid, ht;	//!< window dimensions
    bool		isVis;		//!< true, when the window is visible
  // rendering state
    LightMode		lighting;	//!< the current lighting mode
    Renderer		noLightRender;	//!< the renderer for no-lighting mode
    Renderer		dirLightRender;	//!< the renderer for directional-lighting mode

  // Initialize
    void Init (int argc, const char **argv);

  /*! \brief initialize the renderers by loading and compiling their shaders.
   */
    void InitRenderers ();

  /*! \brief initialize the projection matrix based on the view state. */
    void InitProjMatrix ();

  /*! \brief initialize the model-view matrix based on the view state. */
    void InitModelViewMatrix ();

  /*! \brief render the state of the animation at the current time
   */
    void Render ();

};

#endif /* !_VIEW_HXX_ */
