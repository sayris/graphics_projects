 /*! \file cube.hxx
 *
 * \brief this file contains functions for creating and rendering a cube using GLDrawElements 
 *
 * \author Lamont Samuels 
 */
#ifndef _CUBE_HXX_
#define _CUBE_HXX_ 

#include "cs237.hxx" 
#include "wall.hxx"

#define NUM_SIDES 6

class View; 

class Cube { 
	private: 
    	/* The vertices for the triangle */ 
		/*! the five visible walls of the box */	
		Wall 	walls[NUM_SIDES] = {
		{ /* left */
		    RED_COLOR,		// color
		    cs237::vec3f{  1.0f,  0.0f,  0.0f },	// norm
		    cs237::vec3f{ -1.0f,  1.0f, -1.0f },	// corner[0]
		    cs237::vec3f{ -1.0f,  1.0f,  1.0f },
		    cs237::vec3f{ -1.0f, -1.0f,  1.0f },
		    cs237::vec3f{ -1.0f, -1.0f, -1.0f },
	    	cs237::vec2f{  0.333f,  0.666f},  // texture coordinates 
	    	cs237::vec2f{  0.666f,  0.666f},
	    	cs237::vec2f{  0.666f,  0.333f},
	    	cs237::vec2f{  0.334f,  0.333f},

		},
		{ /* right */
		    GREEN_COLOR,
		    cs237::vec3f{ -1.0f,  0.0f,  0.0f },
		    cs237::vec3f{  1.0f,  1.0f,  1.0f },
		    cs237::vec3f{  1.0f,  1.0f, -1.0f },
		    cs237::vec3f{  1.0f, -1.0f, -1.0f },
		    cs237::vec3f{  1.0f, -1.0f,  1.0f }, 
	    	cs237::vec2f{  0.666f,  0.666f}, //texture coordinates 
	    	cs237::vec2f{  1.0f,  0.666f},
	    	cs237::vec2f{  1.0f,  0.333f},
	    	cs237::vec2f{  0.666f,  0.333f}

		},
		{ /* top */
		    YELLOW_COLOR,
		    cs237::vec3f{  0.0f, -1.0f,  0.0f },
		    cs237::vec3f{ -1.0f,  1.0f, -1.0f },
		    cs237::vec3f{  1.0f,  1.0f, -1.0f },
		    cs237::vec3f{  1.0f,  1.0f,  1.0f },
		    cs237::vec3f{ -1.0f,  1.0f,  1.0f }, 
	    	cs237::vec2f{  0.333f,  0.333f}, 
	    	cs237::vec2f{  0.666f,  0.333f},
	    	cs237::vec2f{  0.666f,  0.0f},
	        cs237::vec2f{  0.333f,  0.0f},



		},
		{ /* bottom */
		    BLUE_COLOR,
		    cs237::vec3f{  0.0f,  1.0f,  0.0f },
		    cs237::vec3f{ -1.0f, -1.0f,  1.0f },
		    cs237::vec3f{  1.0f, -1.0f,  1.0f },
		    cs237::vec3f{  1.0f, -1.0f, -1.0f },
		    cs237::vec3f{ -1.0f, -1.0f, -1.0f }, 
	    	cs237::vec2f{  0.0f,  0.666f}, //texture coordinates 
	    	cs237::vec2f{  0.333f,  0.666f},
	    	cs237::vec2f{  0.333f,  0.333f}, 
	    	cs237::vec2f{  0.0f,  0.333f},


		},
		{ /* front */
		    SKY_BLUE_COLOR, 
		    cs237::vec3f{  0.0f,  0.0f,  1.0f },
		    cs237::vec3f{  1.0f,  1.0f, -1.0f },
		    cs237::vec3f{ -1.0f,  1.0f, -1.0f },
		    cs237::vec3f{ -1.0f, -1.0f, -1.0f },
		    cs237::vec3f{  1.0f, -1.0f, -1.0f },  
	    	cs237::vec2f{  0.0f,  0.333f}, // texture coordinates 
	    	cs237::vec2f{  0.333f,  0.333f},
	    	cs237::vec2f{  0.333f,  0.0f},
	    	cs237::vec2f{  0.0f,  0.0f}

		},
		{ /* back */
		    PINK_COLOR,
		    cs237::vec3f{  0.0f,  0.0f, -1.0f },
		    cs237::vec3f{ -1.0f,  1.0f,  1.0f },
		    cs237::vec3f{  1.0f,  1.0f,  1.0f },
		    cs237::vec3f{  1.0f, -1.0f,  1.0f },
		    cs237::vec3f{ -1.0f, -1.0f,  1.0f }, 
      		cs237::vec2f{  0.666f,  0.333f},
      	    cs237::vec2f{  1.0f,  0.333f},
      	    cs237::vec2f{  1.0f,  0.0f}, 
      	    cs237::vec2f{  0.666f,  0.0f}

		},
	    };

	    const unsigned short indices[NUM_VERTICES] = {0,1,2,3}; 

 		View * view; 
	    cs237::image2d 	*  cubeFlatImage; 
	    cs237::texture2D * cubeFlatTexture; 

	    void init(); 
	    void initTextures(); 
	    void loadVertices(int i); 
	    void loadNorms(int i); 
	    void loadIndices(int i); 
	    void loadTexCoords(int i); 

	public: 	
		Cube(View * view); 					 /*!< the default constructor*/ 
		Cube(View * view, GLfloat length); 	 /*!< a constructor for creating a cube with a given size*/ 
		void render(); 					 /*!< renders the cube to the screen */ 
}; 

#endif /* _CUBE_HXX_ */ 