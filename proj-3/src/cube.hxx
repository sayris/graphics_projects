/*! \file cube.hxx
 *
 * \author Lamont Samuels
 *
 *  \brief The representation of a cube
 */
/*
 * COPYRIGHT (c) 2014 Lamont Samuels (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */
#ifndef _CUBE_HXX_
#define _CUBE_HXX_

#include "plane.hxx"

//! the information needed to render a cube
struct Cube { 

    static const int NumSides = 6;

    Plane sides[NumSides];
    
  //! \brief Create a cube with sides that have the given length 
    Cube (float length);

  //! \brief Render a cube to the view 
    void Draw (View *view, Renderer &render); 
};

#endif /* !_CUBE_HXX_ */

