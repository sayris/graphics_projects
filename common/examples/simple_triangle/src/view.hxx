/*! \file view.hxx
 *
 * \author Lamont Samuels 
 */
#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"

class View 
{
	private: 
		cs237::ShaderProgram  * triangleShader;	 /*!< the shader program for triangle*/
	public: 
		View(); 								/*!< the default constructor */ 
		View(int w, int h); 					/*!< constructor for declaring the width and height of the view */ 
		int width, height; 						/*!< the width and height of the window */ 					
		void initShaders(); 				    /*!< initializes and compiles the shader program */  
		void render(); 							/*!< renders the triangle to the screen */ 
}; 
#endif /* !_VIEW_HXX_*/ 