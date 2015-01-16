/*! \file ball.hxx
 *
 * \brief the representation of balls in the simulation and their operations.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 2 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _BALL_HXX_
#define _BALL_HXX_

#include "cs237.hxx"

struct Renderer;

#define BALL_RADIUS		0.05	//!< radius of balls
#define MAX_NBALLS 		100	//!< maximum number of balls at any time

struct Ray {
    cs237::vec3f	origin;
    cs237::vec3f	dir;

    cs237::vec3f toPoint (float t) const	{ return this->origin + t * this->dir; }
};

/* \brief the representation of a ball in the simulation.
 *
 * A ball's position at time t is pos.orig + (t - initT)*pos.dir
 */
struct Ball {
    double		initT;		//!< time of position/direction info
    double		hitT;		//!< time of next collision
    Ray			pos;		//!< ray that defines the ball's position.
    cs237::color3f	color;		//!< the ball's color

    static int		NumBalls;		//!< \brief the current number of balls being simulated
    static Ball		*Balls[MAX_NBALLS];	//!< \brief the array of balls being simulated
};

//! \brief draw a ball
//  \param now the time now, which is used to determine the position of the ball
//  \param ball the ball to draw
void DrawBall (struct View *view, double now, const Ball *ball);

//! \brief create a new ball
//! \param now the time at which the ball is created
//! \return the new ball, or 0 if MAX_NBALLS has been exceeded.
Ball *NewBall (double now);

//! \brief randomly remove a ball from the simulation
void KillBall ();

//! \brief test to see if (and when) two balls collide.
//! \param b1 points to one of the balls
//! \param b2 points to the other ball
//! \param holds the time of the collision (if there is one)
//! \return true if there is a collision, false otherwise.
bool Collide (Ball *b1, Ball *b2, double &hitT);

//! \brief compute the new states for a ball-ball collision
//! \param b1 points to one of the balls
//! \param b2 points to the other ball
void BallBounce (Ball *b1, Ball *b2);

//! \brief output the state of a ball (for debugging)
//! \param s the stream to print to
//! \param b the ball to print
std::ostream& operator<< (std::ostream& s, Ball const &b);

#endif /* ! _BALL_HXX_ */
