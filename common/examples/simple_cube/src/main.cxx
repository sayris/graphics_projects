/*! \file main.c
 *
 * \brief this file contains the main function and the GLFW callbacks.
 *
 * \author John Reppy
 * \author Lamont Samuels 
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels  (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */

#include "cs237.hxx"

#include "view.hxx"

#include <unistd.h>

/* the viewer state */
View  *view = nullptr;

/*! \brief Set the viewport to the current size of the framebufffer.
 *         We use the framebuffer size instead of the window size, because they
 *         are different on Apple's retina displays.
 */
static void SetViewport (GLFWwindow *window)
{
    int fbWid, fbHt;
    glfwGetFramebufferSize (window, &fbWid, &fbHt);
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
    view->render ();
    glfwSwapBuffers (win);
} /* end of Display */

/*! \brief Window resize callback.
 *  \param wid the new width of the window.
 *  \param ht the new height of the window. 
 */
void Reshape (GLFWwindow *win, int wid, int ht)
{
    view->width = wid;
    view->height = ht;

  /* set the viewport */
    SetViewport (win);

  /* recompute the projection matrix */
    view->initProjMatrix ();

} /* end of Reshape */

/*! \brief Keyboard-event callback.
 *  \param key The Unicode code point of the character.
 */
void Key (GLFWwindow *win, unsigned int key)
{
    switch(key){
      case 's': 
        view->showBlendedCube = !view->showBlendedCube;
      break; 
    }

} /* end of Key */

/*! \brief Visibility-change callback: sets the visibility state of the view.
 *  \param state the current state of the window; GL_TRUE for iconified, GL_FALSE otherwise.
 */
void Visible (GLFWwindow *win, int state)
{

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

    view = new View(800,800);

    glfwWindowHint (GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(view->width, view->height, "Simple Cube", NULL, NULL);
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
    view->startup();

  // set up callbacks
    glfwSetWindowRefreshCallback (window, Display);
    glfwSetWindowSizeCallback (window, Reshape);
    glfwSetWindowIconifyCallback (window, Visible);
    glfwSetCharCallback (window, Key);

    while (! glfwWindowShouldClose(window)) {
      Display (window);
      glfwPollEvents ();
    }

    glfwTerminate ();

    return EXIT_SUCCESS;
    
} /* end of main */
