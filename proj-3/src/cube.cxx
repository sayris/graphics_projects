/*! \file cube.cxx
 *
 * \author Lamont Samuels
 *
 * \brief Modeling and implementation of a cube 
 */

/*
 * COPYRIGHT (c) 2014 Lamont Samuels (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */

#include "cube.hxx"

#define NUM_VERTICES 4 

// Data for 2x2x2 cube at the origin
static Plane Sides[Cube::NumSides] = {
	{ /* left */
	    cs237::color3ub{ 172, 63, 172 },		// color
	    cs237::vec3f{ -1.0f,  0.0f,  0.0f },	// norm
		cs237::vec3f{  1.0f, -1.0f, -1.0f },	//corner 
		cs237::vec3f{  1.0f, -1.0f,  1.0f },
		cs237::vec3f{  1.0f,  1.0f,  1.0f },
		cs237::vec3f{  1.0f,  1.0f, -1.0f },
	},
	{ /* right */
	    cs237::color3ub{ 172, 63, 172 },		// color
	    cs237::vec3f{  1.0f,  0.0f,  0.0f },	// norm					
		cs237::vec3f{ -1.0f, -1.0f,  1.0f },	// corner 
		cs237::vec3f{ -1.0f, -1.0f, -1.0f },	// corner
		cs237::vec3f{ -1.0f,  1.0f, -1.0f },
		cs237::vec3f{ -1.0f,  1.0f,  1.0f },
	},
	{ /* top */
	    cs237::color3ub{ 172, 63, 172 },		// color
	    cs237::vec3f{  0.0f, -1.0f,  0.0f },	// norm
		cs237::vec3f{  1.0f,  1.0f,  1.0f }, 	// corner 
		cs237::vec3f{ -1.0f,  1.0f,  1.0f },
		cs237::vec3f{ -1.0f,  1.0f, -1.0f },
		cs237::vec3f{  1.0f,  1.0f, -1.0f },
	},
	{ /* bottom */
	    cs237::color3ub{ 172, 63, 172 },		// color
	    cs237::vec3f{  0.0f,  1.0f,  0.0f },	// norm
		cs237::vec3f{  1.0f, -1.0f, -1.0f },	//corner 
		cs237::vec3f{ -1.0f, -1.0f, -1.0f },
		cs237::vec3f{ -1.0f, -1.0f,  1.0f },
		cs237::vec3f{  1.0f, -1.0f,  1.0f },
	},
	{ /* front */
	    cs237::color3ub{ 172, 63, 172 },		// color
	    cs237::vec3f{  0.0f,  0.0f,  1.0f },	// norm
		cs237::vec3f{ -1.0f, -1.0f, -1.0f },	//corner 
		cs237::vec3f{  1.0f, -1.0f, -1.0f },
		cs237::vec3f{  1.0f,  1.0f, -1.0f },
		cs237::vec3f{ -1.0f,  1.0f, -1.0f },
	},
	{ /* back */
	    cs237::color3ub{ 172, 63, 172 },		// color
	    cs237::vec3f{  0.0f,  0.0f, -1.0f },	// norm
		cs237::vec3f{  1.0f, -1.0f,  1.0f }, 	//corner 
		cs237::vec3f{ -1.0f, -1.0f,  1.0f },
		cs237::vec3f{ -1.0f,  1.0f,  1.0f },
		cs237::vec3f{  1.0f,  1.0f,  1.0f },
	},
    };

Cube::Cube (float length)
{
  // Initialize to have sides with the given length
    length *= 0.5; 
    for (int s = 0; s < Cube::NumSides; s++) {
	this->sides[s].color = ::Sides[s].color;
	this->sides[s].norm = ::Sides[s].norm;
	for (int v = 0; v < NUM_VERTICES; v++) {
	    this->sides[s].corner[v] = length * ::Sides[s].corner[v];
	}
    }
}

// Draw:
// 
// Render a cube to the view 
//
void Cube::Draw (View *view, Renderer &render)
{
    for (int s = 0; s < Cube::NumSides; s++) {
	this->sides[s].Draw (view, render); 
    }
}
