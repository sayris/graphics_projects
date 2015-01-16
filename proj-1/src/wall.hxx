 /*! \file wall.hxx
 *
 * \brief The representation of walls in the simulation.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _WALL_HXX_
#define _WALL_HXX_

#include "cs237.hxx"
#include "ball.hxx"

struct Wall;

/*! \brief the sides of the cube. */
enum Side {
    LEFT = 0,		/*!< left side:   x == -1 */
    RIGHT,		/*!< right side:  x == +1 */
    BOTTOM,		/*!< bottom side: y == -1 */
    TOP,		/*!< top side:    y == +1 */
    FRONT,		/*!< front side:  z == -1 */
    REAR,		/*!< rear side:   z == +1 */
    NUM_SIDES		/*!< the number of sides */
};

//! \brief test to see which wall the ray hits and when.
//! \param r the ray to test
//! \param w output paramater for returning the wall
//! \param t output parameter for returning the time until the collision
//
void HitTest (Ray const &r, Side &w, double &t);

//! \brief bounce a ball off the wall
//! \param b the ball
//! \param w specifies which wall the ball is bouncing off of
void WallBounce (Ball *b, Side w);

//! \brief draw a wall
//! \param w the side to draw.
extern void DrawWall (struct View *view, struct Renderer *render, Side w);

#endif /* !_WALL_HXX_ */
