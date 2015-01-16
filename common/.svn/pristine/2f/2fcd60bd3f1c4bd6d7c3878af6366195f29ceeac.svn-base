/*! \file main.cxx
 *
 * \author John Reppy
 *
 * Driver for procedural texture demo.
 */

/*
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"

/* clip planes in world coordinates */
#define NEAR_Z		-1.1f
#define FAR_Z		1.1f
#define FOVY		65.0f	/* field-of-view in Y-dimension */

cs237::ShaderProgram *shader = nullptr;

GLuint VAOs[1];
GLuint Buffers[2];

float Aspect = 1.0f;

/***** Callback functions *****/

static void Error (int err, const char *msg)
{
    std::cerr << "[GLFW ERROR " << err << "] " << msg << "\n" << std::endl;
}

/*! \brief Draw a rectangle with the texture
 */
void Display (GLFWwindow *win)
{
  /* clear the screen */
    glClearColor (0.2f, 0.2f, 0.4f, 1.0f);	// clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* initialize the camera */
    cs237::vec3f camPos	= cs237::vec3f(0.0, 0.0, -3.0);
    cs237::vec3f camDir	= cs237::vec3f(0.0, 0.0, 1.0);
    cs237::vec3f camUp = cs237::vec3f(0.0, 1.0, 0.0);

    cs237::mat4x4f projectionMat = cs237::perspective (
	FOVY,
	Aspect,
	NEAR_Z - camPos[2],
	FAR_Z - camPos[2]);

    cs237::mat4x4f modelViewMat = cs237::lookAt (
	camPos,
	camPos + camDir,
	camUp);

    cs237::setUniform (shader->UniformLocation ("projection"), projectionMat);
    cs237::setUniform (shader->UniformLocation ("normalMatrix"), cs237::mat3x3f(modelViewMat));
    cs237::setUniform (shader->UniformLocation ("modelView"), modelViewMat);
    cs237::setUniform (shader->UniformLocation ("lightPosition"),
	cs237::vec3f(modelViewMat * cs237::vec4f(1.0, 1.0, 1.5, 1.0)));

    CS237_CHECK( glBindVertexArray (VAOs[0]) );
    CS237_CHECK( glDrawArrays (GL_TRIANGLE_FAN, 0, 4) );

    glfwSwapBuffers (win);

} /* end of Display */

/*! \brief Keyboard-event callback.
 *  \param key The Unicode code point of the character.
 */
void Key (GLFWwindow *win, unsigned int key)
{
    switch (key) {
      case 'q':
	glfwSetWindowShouldClose (win, true);
	break;
      default: // ignore all other keys
	return;
    }

} /* end of Key */

/* initialize the shader */
void InitShader ()
{
    cs237::VertexShader vsh(DATA_DIR "/bricks.vsh");
    cs237::FragmentShader fsh(DATA_DIR "/bricks.fsh");

    shader = new cs237::ShaderProgram (vsh, fsh);
    if (shader == nullptr) {
	std::cerr << "Cannot build shader\n" << std::endl;
	exit (1);
    }

    shader->Use();
}

/* initialize the geometry
 *
 * We draw a single rectangle in the XY plane at Z=0
 */
void InitGeometry ()
{
    CS237_CHECK( glGenVertexArrays (1, VAOs) );
    CS237_CHECK( glBindVertexArray (VAOs[0]) );

    CS237_CHECK( glGenBuffers (2, Buffers) );

  // set up vertex data buffer
    cs237::vec3f verts[4] = {
	    cs237::vec3f(-1.90, -1.50, 0.0),
	    cs237::vec3f( 1.90, -1.50, 0.75),
	    cs237::vec3f( 1.90,  1.50, 0.75),
	    cs237::vec3f(-1.90,  1.50, 0.0)
	};

    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, Buffers[0]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW) );
    int loc = shader->AttributeLocation("coord");
    CS237_CHECK( glVertexAttribPointer (loc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (loc) );

  // set up normal-vector data buffer
    cs237::vec3f norms[4] = {
	    cs237::vec3f(0.0, 0.0, 1.0),
	    cs237::vec3f(0.0, 0.0, 1.0),
	    cs237::vec3f(0.0, 0.0, 1.0),
	    cs237::vec3f(0.0, 0.0, 1.0)
	};

    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, Buffers[1]) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, sizeof(norms), norms, GL_STATIC_DRAW) );
    loc = shader->AttributeLocation("norm");
    CS237_CHECK( glVertexAttribPointer (loc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (loc) );

}

int main (int argc, const char **argv)
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

    glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(1024, 768, "Bricks", NULL, NULL);
    if (window == nullptr)
	exit (EXIT_FAILURE);

    glfwMakeContextCurrent (window);

    {
	int fbWid, fbHt;
	glfwGetFramebufferSize (window, &fbWid, &fbHt);
	glViewport(0, 0 , fbWid, fbHt);
	Aspect = static_cast<float>(fbWid) / static_cast<float>(fbHt);
    }

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

  // set up callbacks
    glfwSetWindowRefreshCallback (window, Display);
    glfwSetCharCallback (window, Key);

  // init the render state
    InitShader();
    InitGeometry();

    while (! glfwWindowShouldClose(window)) {
	Display (window);
	glfwWaitEvents ();
    }

    glfwTerminate ();

    return EXIT_SUCCESS;
    
}
