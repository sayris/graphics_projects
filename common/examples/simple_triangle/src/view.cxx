/*! \file view.cxx
 *
 * \author Lamont Samuels 
 */

#include "view.hxx" 
#include <iostream> 

View::View() : triangleShader(nullptr), width(800), height(800){}  //default constructor for view class 

View::View(int w, int h) : triangleShader(nullptr), width(w), height(h){} //constructor for providing the width and height of the view. 

//! \brief initializes and compiles the shader program 
void View::initShaders() { 	

	//Check to see if we created the shader program 
    if (this->triangleShader == nullptr) {
    	
    	// Compiles the vertex shader 
		cs237::VertexShader vsh("../shaders/shader.vert");
		
		// Compiles the fragment shader 
		cs237::FragmentShader fsh("../shaders/shader.frag");
		
		//Creates and Links the vertex and fragment shaders to the shader program 
		this->triangleShader = new cs237::ShaderProgram (vsh, fsh);
		
		// Verify that shader program was created correctly
		if (this->triangleShader == nullptr)
	    	exit (1);
	    
	    // Create a Vertex Array Object for rendering geometries to the screen 	
	    GLuint vaoName;
	    
    	// Generates a name for the Vertex Array Object 
    	glGenVertexArrays(1, &vaoName);

    	// Binds the Vertex Array Object to the OpenGL context 
    	glBindVertexArray(vaoName);
	}
	
}
//! \brief renders the triangle to the screen
void View::render() { 

	// declare a color (red color) for the background of the screen.   
	const GLfloat color[] = {1.0f, 0.0f,0.0f, 1.0f}; 
							
	// Clears the color buffer (i.e. the background) and assigns red to it.  
	glClearBufferfv(GL_COLOR,0,color); 

	// Tells the shader program we are ready to use it for rendering. 
	triangleShader->Use(); 	

	// Renders the triangle to the screeen. 
	glDrawArrays(GL_TRIANGLES,0,3); 
}