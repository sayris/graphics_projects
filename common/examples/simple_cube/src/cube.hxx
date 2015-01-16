 /*! \file cube.hxx
 *
 * \brief this file contains functions for creating and rendering a Cube
 *
 * \author Lamont Samuels 
 */
#ifndef _CUBE_HXX_
#define _CUBE_HXX_ 

#include "cs237.hxx" 
#include "vertex.hxx"

class Cube { 
	private: 
		GLuint		vaoId;		//!< vertex-array-object ID for the simple cube 
    	GLuint		vbufId;		//! vertex-buffer ID for the vertex data
    	
		Vertex_t vertices[36] = {  /* Front Side */ 
								  {cs237::vec3f( -1.0f,  -1.0f,   1.0f), RED_COLOR},  //0
								  {cs237::vec3f(  1.0f,   1.0f,   1.0f), RED_COLOR},  //2
								  {cs237::vec3f( -1.0f,   1.0f,   1.0f), RED_COLOR},  //1
	             				  {cs237::vec3f( -1.0f,  -1.0f,   1.0f), RED_COLOR},  //0
	             				  {cs237::vec3f(  1.0f,  -1.0f,   1.0f), RED_COLOR},  //3
	             				  {cs237::vec3f(  1.0f,   1.0f,   1.0f), RED_COLOR},  //2

	             				  /* Bottom Side */
	             				  {cs237::vec3f( -1.0f,  -1.0f,  -1.0f), GREEN_COLOR},  //4
	             				  {cs237::vec3f(  1.0f,  -1.0f,   1.0f), GREEN_COLOR},  //3
	             				  {cs237::vec3f( -1.0f,  -1.0f,   1.0f), GREEN_COLOR},  //0
	             				  {cs237::vec3f( -1.0f,  -1.0f,  -1.0f), GREEN_COLOR},  //4
	             				  {cs237::vec3f(  1.0f,  -1.0f,  -1.0f), GREEN_COLOR},  //7
	             				  {cs237::vec3f(  1.0f,  -1.0f,   1.0f), GREEN_COLOR},  //3
	             				  
	             				  /* Left Side */ 
	             				  {cs237::vec3f( -1.0f,  -1.0f,  -1.0f), YELLOW_COLOR},  //4
	             			      {cs237::vec3f( -1.0f,   1.0f,   1.0f), YELLOW_COLOR},  //1
	             				  {cs237::vec3f( -1.0f,   1.0f,  -1.0f), YELLOW_COLOR},  //5 
	             				  {cs237::vec3f( -1.0f,  -1.0f,  -1.0f), YELLOW_COLOR},  //4
								  {cs237::vec3f( -1.0f,  -1.0f,   1.0f), YELLOW_COLOR},  //0
		             			  {cs237::vec3f( -1.0f,   1.0f,   1.0f), YELLOW_COLOR},  //1

		             			  /* Right Side */ 
	             				  {cs237::vec3f(  1.0f,  -1.0f,   1.0f), BLUE_COLOR},  //3
	             				  {cs237::vec3f(  1.0f,   1.0f,  -1.0f), BLUE_COLOR},  //6	             				   
	             				  {cs237::vec3f(  1.0f,   1.0f,   1.0f), BLUE_COLOR},  //2
	             				  {cs237::vec3f(  1.0f,  -1.0f,   1.0f), BLUE_COLOR},  //3
	             				  {cs237::vec3f(  1.0f,  -1.0f,  -1.0f), BLUE_COLOR},  //7
	             				  {cs237::vec3f(  1.0f,   1.0f,  -1.0f), BLUE_COLOR},  //6	       				   

	             				  /* Top Side*/ 
	             			      {cs237::vec3f( -1.0f,   1.0f,   1.0f), SKY_BLUE_COLOR},  //1             				  
	             				  {cs237::vec3f(  1.0f,   1.0f,  -1.0f), SKY_BLUE_COLOR},  //6	
	             				  {cs237::vec3f( -1.0f,   1.0f,  -1.0f), SKY_BLUE_COLOR},  //5 
	             			      {cs237::vec3f( -1.0f,   1.0f,   1.0f), SKY_BLUE_COLOR},  //1
			             		  {cs237::vec3f(  1.0f,   1.0f,   1.0f), SKY_BLUE_COLOR},  //2    
	             				  {cs237::vec3f(  1.0f,   1.0f,  -1.0f), SKY_BLUE_COLOR},  //6	       			       

	             				  /* Back Side */ 
	             				  {cs237::vec3f(  1.0f,  -1.0f,  -1.0f), PINK_COLOR},  //7
	             				  {cs237::vec3f( -1.0f,   1.0f,  -1.0f), PINK_COLOR},  //5 	             				   
	             				  {cs237::vec3f(  1.0f,   1.0f,  -1.0f), PINK_COLOR},  //6	
	             				  {cs237::vec3f(  1.0f,  -1.0f,  -1.0f), PINK_COLOR},  //7  
	             				  {cs237::vec3f( -1.0f,  -1.0f,  -1.0f), PINK_COLOR},  //4
	             				  {cs237::vec3f( -1.0f,   1.0f,  -1.0f), PINK_COLOR},  //5 
		             			};	  
		void init(); 				  /*!< initializes the vertex buffer */ 
	public:
		GLint		posLoc;		//!< the vertex attribute location for the position 
    	GLint		colorLoc;	//!< the vertex attribute location for the color  	
		Cube(); 					  /*!< the default constructor*/ 
		Cube(GLfloat length); 		  /*!< a constructor for creating a cube with a given size*/ 
		void render(); 				  /*!< renders the cube to the screen */ 
}; 

#endif /* _CUBE_HXX_ */ 