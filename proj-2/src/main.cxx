/*! \file main.c
 *
 * \brief this file contains the main function and the GLFW callbacks.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 2 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"

#include "event.hxx"
#include "view.hxx"

#include <unistd.h>

#define SIMULATION_TIMESTEP	0.010	// 100Hz
#define RENDER_TIMESTEP		0.016	// ~60Hz

/* the viewer state */
View	*View = nullptr;

/*! \brief Set the viewport to the current size of the framebufffer.
 *         We use the framebuffer size instead of the window size, because it
 *         is different on Apple's retina displays.
 */
static void SetViewport (GLFWwindow *win)
{
    int fbWid, fbHt;
    glfwGetFramebufferSize (win, &fbWid, &fbHt);
    glViewport(0, 0 , fbWid, fbHt);

}

/***** Callback functions *****/

static void Error (int err, const char *msg)
{
    std::cerr << "[GLFW ERROR " << err << "] " << msg << "\n" << std::endl;
}

/*! \brief Run the simulation and then redraw the animation.
 */
void Display (GLFWwindow *win)
{
  /* avoid drawing when we are invisible */
    if (! View->isVis)
	return;

    View->Render ();
    glfwSwapBuffers (win);

    View->needsRedraw = false;

} /* end of Display */

/*! \brief Window resize callback.
 *  \param wid the new width of the window.
 *  \param ht the new height of the window. 
 */
void Reshape (GLFWwindow *win, int wid, int ht)
{
    View->wid = wid;
    View->ht = ht;

  /* set the viewport */
    SetViewport (win);

  /* recompute the projection matrix */
    View->InitProjMatrix ();

} /* end of Reshape */

/*! \brief Keyboard-event callback.
 *  \param key The Unicode code point of the character.
 */
void Key (GLFWwindow *win, unsigned int key)
{
    switch (key) {
      case 'q':
	glfwSetWindowShouldClose (win, true);
	break;
      case 'l':  // cycle lighting mode
	switch (View->lighting) {
	  case NO_LIGHT: View->lighting = DIR_LIGHT; break;
	  case DIR_LIGHT: View->lighting = POINT_LIGHT; break;
	  case POINT_LIGHT: View->lighting = NO_LIGHT; break;
	}
	View->needsRedraw = true;
	break;
      case 'b':  // toggle bump mapping
	View->bumpMapping = !View->bumpMapping;
	View->needsRedraw = true;
	break;
      case '+':
	PredictEvent (NewBall (glfwGetTime()));
	View->needsRedraw = true;
	break;
      case '-':
	KillBall ();
	View->needsRedraw = true;
	break;
      default: // ignore all other keys
	return;
    }

} /* end of Key */

/*! \brief Visibility-change callback: sets the visibility state of the view.
 *  \param state the current state of the window; GL_TRUE for iconified, GL_FALSE otherwise.
 */
void Visible (GLFWwindow *win, int state)
{
    View->isVis = (state == GL_FALSE);

} /* end of Visible */

/*! \brief the main function for the program
 *  \param argc the count of arguments in \a argv
 *  \param argv the array of command-line arguments.
 */
int main(int argc, const char **argv)
{
    glfwSetErrorCallback (Error);

    glfwInit ();

  // Check the GLFW version
    {
	int major;
	glfwGetVersion (&major, NULL, NULL);
	if (major < 3) {
	    std::cerr << "CS237 requires GLFW 3.0 or later\n" << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

    View = new struct View();
    View->Init (argc, argv);

    glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(View->wid, View->ht, "CMSC 23700: Project 2", NULL, NULL);
    if (window == nullptr)
	exit (EXIT_FAILURE);

    glfwMakeContextCurrent (window);

    SetViewport (window);

  // Check the OpenGL version
    {
	GLint major, minor;
	glGetIntegerv (GL_MAJOR_VERSION, &major);
	glGetIntegerv (GL_MINOR_VERSION, &minor);
	if ((major < 3) || ((major == 3) && (minor < 2))) {
	    std::cerr << "CS237 requires OpenGL 3.2 or later; found " << major << "." << minor << "\n" << std::endl;
	    exit (EXIT_FAILURE);
	}
    }

  // NOTE: the shaders must be initialized after a GL context is in place.
    View->InitRenderer ();

    View->InitProjMatrix ();
    View->InitModelViewMatrix ();

  // set up callbacks
    glfwSetWindowRefreshCallback (window, Display);
    glfwSetWindowSizeCallback (window, Reshape);
    glfwSetWindowIconifyCallback (window, Visible);
    glfwSetCharCallback (window, Key);

    while (! glfwWindowShouldClose(window)) {
	double now = glfwGetTime();

	if (now - View->lastStep >= SIMULATION_TIMESTEP) {
	  /* run the simulation */
	    Simulate (now);
	}

	if (View->needsRedraw || (now - View->lastDraw >= RENDER_TIMESTEP)) {
	    View->lastDraw = now;
	    Display (window);
	}

	usleep (1000);	// sleep for 1mS

	glfwPollEvents ();
    }

    glfwTerminate ();

    return EXIT_SUCCESS;
    
} /* end of main */
