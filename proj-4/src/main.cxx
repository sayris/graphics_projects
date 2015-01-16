/*! \file main.cxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "map.hxx"
#include "map-cell.hxx"
#include <unistd.h>
#include "view.hxx"
#include <cmath>

#define RENDER_TIMESTEP   0.016 // ~60Hz

#define INERTIA_THRESHOLD 1.0f
#define INERTIA_FACTOR    0.5f
#define SCALE_INCREMENT   0.01f
#define MIN_SCALE         0.25f
#define MAX_SCALE         0.75f
#define SCREEN_ERROR      100
#define MOUSE_SENSE       30000


/* the viewer state */
View *View = nullptr;
/*! \brief Set the viewport to the current size of the framebufffer.
 *         We use the framebuffer size instead of the window size, because it
 *         is different on Apple's retina displays.
 */
static void SetViewport (GLFWwindow *win)
{
    int fbWid, fbHt;
    glfwGetFramebufferSize (win, &fbWid, &fbHt);
    glViewport(0, 0, fbWid, fbHt);
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
    if (!View->isVis)
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
 *  \param win the window receiving the event
 *  \param key The keyboard code of the key
 *  \param scancode The system-specific scancode of the key.
 *  \param action GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT.
 *  \param mods the state of the keyboard modifier keys.
 */
void Key (GLFWwindow *win, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;
    
    switch (key) {
        case GLFW_KEY_ESCAPE:
        case GLFW_KEY_Q:
            if (mods == 0)
                glfwSetWindowShouldClose (win, true);
            break;
        case GLFW_KEY_K:
            if(mods == 0){
                View->renderMode = View->renderMode == WIREFRAME ? TEXTURE : WIREFRAME;
            }
            break;
        case GLFW_KEY_L:        //lighting
            if (mods == 0) {
                //switching from REGULAR to WIREFRAME... might have to alter this at some point
                View->lighting=(View->lighting==NO_LIGHT)? DIR_LIGHT : NO_LIGHT;
            }
            break;
        case GLFW_KEY_F:        //fog
            if (mods == 0) {
                View->fogMode = View->fogMode == NOFOG? FOG : NOFOG;
            }
            break;
        case GLFW_KEY_N:        //fog
            if (mods == 0) {
                View->skyMode = View->skyMode == NOSKY? SKY : NOSKY;
            }
            break;
        case GLFW_KEY_EQUAL:
            if (mods == GLFW_MOD_SHIFT) {
                // shift+'=' is '+'
                /* Need to make this increase screen-space error tolerance by root 2*/
                View->screen_space_error *= sqrt(2);
                std::cout<<"screen space error="<<View->screen_space_error<<"\n";
                
            }
            break;
        case GLFW_KEY_KP_ADD:  // keypad '+'
            if (mods == 0) {
                /* Need to make this increase screen-space error tolerance by root 2*/
                View->screen_space_error *= sqrt(2);
                std::cout<<"screen space error="<<View->screen_space_error<<"\n";
            }
            break;
        case GLFW_KEY_MINUS:
            if (mods == 0) {
                /* Need to make this decrease screen-space error tolerance by root 2*/
                View->screen_space_error /= sqrt(2);
                std::cout<<"screen space error="<<View->screen_space_error<<"\n";
                
            }
            break;
        case GLFW_KEY_KP_SUBTRACT:  // keypad '-'
            if (mods == 0) {
                /* Need to make this increase screen-space error tolerance by root 2*/
                View->screen_space_error /= sqrt(2);
                std::cout<<"screen space error="<<View->screen_space_error<<"\n";
            }
            break;
        case GLFW_KEY_W:
            if (mods == 0)
                View->FPS(GLFW_KEY_W, SCREEN_ERROR);
            break;
        case GLFW_KEY_A:
            if (mods == 0){
                View->FPS(GLFW_KEY_A, SCREEN_ERROR);
            }
            break;
        case GLFW_KEY_S:
            if (mods == 0)
                View->FPS(GLFW_KEY_S, SCREEN_ERROR);
            break;
        case GLFW_KEY_D:
            if (mods == 0)
                View->FPS(GLFW_KEY_D, SCREEN_ERROR);
            break;
        case GLFW_KEY_UP:
            if (mods == 0)
                View->FPS(GLFW_KEY_UP, SCREEN_ERROR);
            break;
        case GLFW_KEY_DOWN:
            if (mods == 0)
                View->FPS(GLFW_KEY_DOWN, SCREEN_ERROR);
            break;
        case GLFW_KEY_SPACE:
            if (mods == 0)
                View->FPS(GLFW_KEY_SPACE, SCREEN_ERROR);
            break;
        default: // ignore all other keys
            return;
    }
    View->needsRedraw = true;
    
} /* Key */

/*! \brief The mouse button callback function
 */
void MouseButton (GLFWwindow *win, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_1) {
        View->mouseTracking = (action == GLFW_PRESS);
        if (View->mouseTracking) {
            // initialize inertial parameters
            View->fInertia = cs237::vec2f(0.0, 0.0);
            double xPos, yPos;
            glfwGetCursorPos (win, &xPos, &yPos);
            View->xLast = static_cast<int>(xPos);
            View->yLast = static_cast<int>(yPos);
        }
        else {
            if (View->xLastIncr > INERTIA_THRESHOLD)
                View->fInertia.x = (View->xLastIncr - INERTIA_THRESHOLD)*INERTIA_FACTOR;
            else if (-View->xLastIncr > INERTIA_THRESHOLD)
                View->fInertia.x = (View->xLastIncr + INERTIA_THRESHOLD)*INERTIA_FACTOR;
            if (View->yLastIncr > INERTIA_THRESHOLD)
                View->fInertia.y = (View->yLastIncr - INERTIA_THRESHOLD)*INERTIA_FACTOR;
            else if (-View->yLastIncr > INERTIA_THRESHOLD)
                View->fInertia.y = (View->yLastIncr + INERTIA_THRESHOLD)*INERTIA_FACTOR;
        }
        View->xLastIncr = 0;
        View->yLastIncr = 0;
    }
}

/*****************************************************************************
 * MouseMotion: The mouse callback function
 *****************************************************************************/
void MouseMotion (GLFWwindow *win, double xPos, double yPos)
{
    int centerx = View->wid/2;
    int centery = View->ht/2;
    View->x_angle += (centerx - xPos)/MOUSE_SENSE;
    View->y_angle += (centery - yPos)/MOUSE_SENSE;
}

/*! \brief Visibility-change callback: sets the visibility state of the view.
 *  \param state the current state of the window; GL_TRUE for iconified, GL_FALSE otherwise.
 */
void Visible (GLFWwindow *win, int state)
{
    View->isVis = (state == GL_FALSE);
    
}
/* end of Visible */

/*! \brief the main function for the program
 *  \param argc the count of arguments in \a argv
 *  \param argv the array of command-line arguments.
 */
int main(int argc, const char **argv)
{
    // get the mapfile
    if (argc < 2) {
        std::cerr << "usage: proj-4 <map-dir>" << std::endl;
        return 1;
    }
    
    //need to sent this cell to a renderer
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
    
    GLFWwindow *window = glfwCreateWindow(View->wid, View->ht, "CMSC 23700: Project 4", NULL, NULL);
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
    View->window = window;
    View->InitRenderer ();
    View->InitProjMatrix ();
    View->InitModelViewMatrix ();
    
    // set up callbacks
    glfwSetWindowRefreshCallback (window, Display);
    glfwSetWindowSizeCallback (window, Reshape);
    glfwSetWindowIconifyCallback (window, Visible);
    glfwSetKeyCallback (window, Key);
    glfwSetMouseButtonCallback (window, MouseButton);
    glfwSetCursorPosCallback (window, MouseMotion);
    
    while (! glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        
        if (View->needsRedraw || (now - View->lastDraw >= RENDER_TIMESTEP)) {
            View->InitModelViewMatrix();
            View->lastDraw = now;
            Display (window);
        }
        
        usleep (1000);  // sleep for 1mS
        
        glfwPollEvents ();
    }
    glfwTerminate ();
    
    return EXIT_SUCCESS;
    
} /* end of main */


