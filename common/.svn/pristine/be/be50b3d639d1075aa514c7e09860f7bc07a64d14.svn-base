 /*! \file cube.hxx
 *
 * \brief this file contains functions for creating and rendering a cube using GLDrawElements 
 *
 * \author Lamont Samuels 
 */
#ifndef _WALL_HXX_
#define _WALL_HXX_ 

#define RED_COLOR  cs237::color4f(1.0f, 0.0f, 0.0f, 1.0f)  	/*!< Red color */ 
#define GREEN_COLOR cs237::color4f(0.0f, 1.0f, 0.0f, 1.0f)		/*!< Green color */
#define YELLOW_COLOR	cs237::color4f(1.0f, 1.0f, 0.0f, 1.0f)		/*!< Yellow color */
#define BLUE_COLOR cs237::color4f(0.0f, 0.0f, 1.0f, 1.0f)		/*!< Blue color */
#define SKY_BLUE_COLOR cs237::color4f(0.0f, 1.0f, 1.0f, 1.0f)		/*!< Sky Blue color */
#define PINK_COLOR cs237::color4f(1.0f, 0.0f, 0.5f, 1.0f)		/*!< Pink */ 

#define NUM_VERTICES 4

struct Wall {
	cs237::color4f color; 
	cs237::vec3f norm;
	cs237::vec3f corners[NUM_VERTICES]; 
	cs237::vec2f texCoords[NUM_VERTICES]; 
	GLuint		vaoId;		//!< vertex-array-object ID for the simple cube 
    GLuint		vbufId;		//! vertex-buffer ID for the vertex data
    GLuint		ibufId;		//! buffer ID for the indices data
    GLuint 		nbufId; 	
    GLuint		tbufId; 
    GLenum 		prim; 


	Wall (cs237::color4f c, cs237::vec3f nv,
	cs237::vec3f v0, cs237::vec3f v1, cs237::vec3f v2, cs237::vec3f v3, 
	cs237::vec2f t0, cs237::vec2f t1, cs237::vec2f t2, cs237::vec2f t3)
	: color(c), norm(nv), corners{v0, v1, v2, v3}, texCoords{t0, t1, t2, t3},  vaoId(-1),vbufId(-1),ibufId(-1), nbufId(-1), tbufId(-1), prim(0)
    { }

}; 

#endif /* _WALL_HXX_ */ 