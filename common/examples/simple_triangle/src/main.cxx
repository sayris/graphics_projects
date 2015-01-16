 /*! \file main.cxx
 *
 * \brief this file contains the main function and the GLFW callbacks for drawing a triangle to the screen. 
 *
 * \author Lamont Samuels 
 */

#include "cs237.hxx"
#include "view.hxx" 

static View * view = 0; 

/*! \brief Window resize callback.
 *  \param wid the new width of the window.
 *  \param ht the new height of the window. 
 */
void reshape (GLFWwindow *win, int wid, int ht)
{
   /* update your views width and height */
  view->width = wid; 
  view->height = ht; 
 
   glViewport (0, 0, (GLsizei)wid, (GLsizei)ht);

   /* recompute the projection matrix (not necessary for this sample project) */

} /* end of reshape */


/*! \brief Keyboard-event callback.
 *  \param key The Unicode code point of the character.
 */
void keyCallback (GLFWwindow *win, unsigned int key)
{
    switch (key) {
      case 'q':
	    break;
      case 'l':
	    break;
      case '+':
	    break;
      case '-':
	    break;
      default: // ignore all other keys
	    return;
    }

} /* end of keyCallback */

/*! \brief Visibility-change callback: sets the visibility state of the view.
 *  \param state the current state of the window; GL_TRUE for iconified, GL_FALSE otherwise.
 */
void visible (GLFWwindow *win, int state)
{
    

} /* end of visible */


int main(int argc, char * argv[])
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit()) {
    	std::cerr << "GLFW Library could not be initialized " << std::endl; 
        exit (EXIT_FAILURE);
    }
    
	// Check the GLFW version
    {
		int major;
		glfwGetVersion (&major, NULL, NULL);
		if (major < 3) {
			std::cerr << "CS237 requires GLFW 3.0 or later\n" << std::endl; 
	    	exit (EXIT_FAILURE);
		}
    }
    
	  glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /*Create view for application */ 
    view = new View(800,800);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(view->width, view->height, "Triangle", NULL, NULL);

    if (!window) {
        glfwTerminate();
        std::cerr << "GLFW could not create a window\n" << std::endl; 
       	exit (EXIT_FAILURE);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
     //Initialize the shaders 
     view->initShaders();  
    
    // Check the OpenGL version
   {
		GLint major, minor;
		glGetIntegerv (GL_MAJOR_VERSION, &major);
		glGetIntegerv (GL_MINOR_VERSION, &minor);
		if ((major < 3) || ((major == 3) && (minor < 2))) {
	    	std::cerr << "CS237 requires OpenGL 3.2 or later\n" << std::endl;
	    	exit (EXIT_FAILURE);
		}
    }
    
    glfwSetWindowSizeCallback (window, reshape);
    glfwSetWindowIconifyCallback (window, visible);
    glfwSetCharCallback (window, keyCallback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      view->render(); 
      glfwSwapBuffers (window);

      /* Poll for and process events */
      glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}