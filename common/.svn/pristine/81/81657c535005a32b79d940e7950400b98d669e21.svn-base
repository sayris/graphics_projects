/*! \file view.hxx
 *
 * Handles functions for window and rendering objects 
 * \author Lamont Samuels 
 */
#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "cube.hxx"

class View 
{
	private: 
		cs237::ShaderProgram  * shaderProg;	/*!< the shader program for cube(s)*/
		
		cs237::vec3f camPos;   					/*!< camera position in world space */
		cs237::vec3f camTarget;   				/*!< camera target in world space */
		cs237::vec3f camUp;    					/*!< camera up vector in world space */
		
		cs237::mat4f modelViewMat; 				/*!< the model view matrix */ 
		cs237::mat4f projectionMat; 			/*!< the projection matrix */ 
					
		/*! \brief a cube  to render to the screen */ 
		Cube *c1;

		/*! \brief performs initializations needed before starting up the view*/ 
		void init(); 
		
		/*! \brief compiles the shaders */ 
		void initShaders(); 

  		/*! \brief initialize the model-view matrix based on the view state. */
    	void initModelViewMatrix(); 

	public: 
		View(); 								/*!< the default constructor */ 
		View(int w, int h); 					/*!< a constructor for creating a view of a certain size */ 
		int width, height; 						/*!< the width and height of the window */

		GLint modelViewLoc; 					/*!< the uniform location for model view matrix in the shaders*/ 
		GLint projectionLoc; 					/*!< the uniform location for model view matrix in the shaders*/ 
		GLint texSamplerLoc; 
		GLint colorLoc;  
		GLint posLoc; 
		GLint normLoc; 
		GLint texLoc;  

		/*! \brief performs any initialization before using the view */  
		void startup(); 
		
		/*! \brief performs any cleanup before we destroy the view */ 
		void shutdown(); 
		
		/*! \brief renders the cube(s) to the screen */
		void render(); 

		/*! \brief initialize the projection matrix based on the view state. */
   		void initProjMatrix();


}; 
#endif /* !_VIEW_HXX_*/ 