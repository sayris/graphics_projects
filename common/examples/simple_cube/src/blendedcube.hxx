 /*! \file blendedcube.hxx
 *
 * \brief this file contains functions for creating and rendering a cube using GLDrawElements 
 *
 * \author Lamont Samuels 
 */
#ifndef _BLENDEDCUBE_HXX_
#define _BLENDEDCUBE_HXX_ 

#include "vertex.hxx"
#include "cs237.hxx" 

class BlendedCube { 
	private: 
		GLuint		vaoId;		//!< vertex-array-object ID for the simple cube 
    	GLuint		vbufId;		//! vertex-buffer ID for the vertex data
    	GLuint		ibufId;		//! buffer ID for the indices data

    	/* The vertices for the triangle */ 
		Vertex_t vertices[8]= {{cs237::vec3f(-1.0f,  -1.0f,  1.0f),RED_COLOR}, //0
		             		   {cs237::vec3f (-1.0f,  1.0f,  1.0f),GREEN_COLOR}, //1
		             		   {cs237::vec3f ( 1.0f,  1.0f,  1.0f),BLUE_COLOR}, //2
		             		   {cs237::vec3f( 1.0f,  -1.0f,  1.0f),SKY_BLUE_COLOR}, //3
		             		   {cs237::vec3f (-1.0f,  -1.0f, -1.0f),PINK_COLOR}, //4
		             		   {cs237::vec3f (-1.0f,  1.0f, -1.0f),YELLOW_COLOR}, //5
		             		   {cs237::vec3f ( 1.0f,  1.0f, -1.0f),RED_COLOR}, //6
		             		   {cs237::vec3f ( 1.0f,  -1.0f, -1.0f),BLUE_COLOR}}; //7 

		/* the indices that allow us to create the triangles. */ 
		const GLuint indices[36] = {
		    0,2,1,  0,3,2,
		    4,3,0,  4,7,3,
		    4,1,5,  4,0,1,
		    3,6,2,  3,7,6,
		    1,6,5,  1,2,6,
		    7,5,6,  7,4,5
	    };  
	    void init(); 
	public: 	
	    GLint		posLoc;		//!< the vertex attribute location for the position 
    	GLint		colorLoc;	//!< the vertex attribute location for the color 

		BlendedCube(); 					 /*!< the default constructor*/ 
		BlendedCube(GLfloat length); 	 /*!< a constructor for creating a cube with a given size*/ 
		void render(); 					 /*!< renders the cube to the screen */ 
}; 

#endif /* _BLENDEDCUBE_HXX_ */ 