/*! \file ball.c
 *
 * \brief The simulation and rendering code for balls.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 2 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "ball.hxx"
#include "event.hxx"
#include "view.hxx"

#define EPSILON		0.00001f

#define NUM_COLORS	8

static cs237::color3ub	colors[NUM_COLORS] = {
	{ 204, 255, 102 },		// honeydew
	{ 255,   0, 128 },		// strawberry
	{ 102, 255, 102 },		// flora
	{  0,  255, 255 },		// turqoise
	{ 255, 102, 102 },		// salmon
	{ 255, 111, 207 },		// carnation
	{   0, 128, 255 },		// aqua
	{ 102, 255, 204 },		// spindrift
    };

int  Ball::NumBalls = 0;		//!< \brief the current number of balls being simulated
Ball *Ball::Balls[MAX_NBALLS];		//!< \brief the array of balls being simulated

static MeshInfo *SphereMesh = nullptr;

//! \brief generate a random float between 0.0 and 1.0.
static float ranf ()
{
    return (float)drand48();
}

//! \brief generate a random unit vector.
//! \param v this output parameter will hold the random vector. 
static cs237::vec3f RandVec ()
{
    cs237::vec3f v;
    float lenSq;

    do {
	v = cs237::vec3f(2.0 * ranf() - 1.0, 2.0 * ranf() - 1.0, 2.0 * ranf() - 1.0);
	lenSq = dot(v, v);
    } while (lenSq < EPSILON);

    return (v / sqrtf(lenSq));	// sqrt should work here, since it is overloaded in cmath, but it doesn't

}

//! \brief generate a random number between 0 and n-1.
//! \param n defines the range of random numbers
//! \return a random number between 0 and n-1.
static int randInRange (int n)
{
    return (int)lrand48() % n;
}

/* Ball constructor:
 */
Ball *NewBall (double now)
{
    if (Ball::NumBalls >= MAX_NBALLS)
	return nullptr;

    Ball *ball = new Ball;
    ball->initT = now;
    ball->pos.origin = cs237::vec3f((1.8 * ranf()) - 0.9, (1.8 * ranf()) - 0.9, (1.8 * ranf()) - 0.9);
    ball->pos.dir = RandVec();
    ball->color = cs237::color3f(colors[randInRange(NUM_COLORS)]);

    Ball::Balls[Ball::NumBalls++] = ball;

    return ball;

} /* end of NewBall */

/* KillBall:
 */
void KillBall ()
{
    if (Ball::NumBalls == 0)
	return;

    if (Ball::NumBalls == 1) {
	CancelEvent (Ball::Balls[0]);
	Ball::NumBalls = 0;
	return;
    }

    int i = randInRange (Ball::NumBalls);
    int j = --Ball::NumBalls;

  // remove Ball[i] from the event queue
    CancelEvent (Ball::Balls[i]);

  // copy Ball[j]'s state into Ball[i]
    Ball::Balls[i] = Ball::Balls[j];

} /* end of KillBall */


/***** Simulation code *****/

/* Collide:
 */
bool Collide (Ball *b1, Ball *b2, double &hitT)
{
    cs237::vec3f p1, p2;
    double t0;

  // adjust the ball with the older initT to give a common starting point
    if (b1->initT < b2->initT) {
	t0 = b2->initT;
	p1 = b1->pos.toPoint((float)(t0 - b1->initT));
	p2 = b2->pos.origin;
    }
    else {
	t0 = b1->initT;
	p1 = b1->pos.origin;
	p2 = b2->pos.toPoint((float)(t0 - b2->initT));
    }

  // the difference in initial positions
    cs237::vec3f u = p1 - p2;

  // the difference in velocity vectors
    cs237::vec3f v = b1->pos.dir - b2->pos.dir;

  // the roots of at^2 + bt + c determine the collision points
    float a = dot(v, v);
    float b = 2.0 * dot(u, v);
    float c = dot(u, u) - (4.0 * BALL_RADIUS * BALL_RADIUS);
    float d2 = (b*b) - 4.0*a*c;

    if (d2 <= EPSILON)
	return false;

  // here there is (or was) a collision
    float d = sqrt(d2);
    float t1 = (-b - d) / (2.0 * a);
    if (t1 < 0.0) {
	return false;
    }

    hitT = t0+t1;
    return true;

}

/* BallBounce:
 */
void BallBounce (Ball *b1, Ball *b2)
{
  /* compute the balls's positions at the time of impact */
    cs237::vec3f p1 = b1->pos.toPoint ((float)(b1->hitT - b1->initT));
    cs237::vec3f p2 = b2->pos.toPoint ((float)(b2->hitT - b2->initT));

  // a vector from p1 to p2
    cs237::vec3f v = p2 - p1;

  // normalize v.  Thus v is the plane normal for the contact plane.
    v.normalize();

  // compute the new direction vector for b1; if b1 is heading toward the contact
  // plane, then the following dot product will be positive.
    float d = dot (b1->pos.dir, v);
    if (d > 0.0f) {
      // as per homework 1, the reflection vector is given by projecting
      // the negative direction vector onto the normal vector, scaling by 2,
      // and then adding in the original direction vector.
	b1->pos.dir += (-2.0f * d) * v;
    }
    /* else: b1 is moving away from the plane, so no change */

  // compute the new direction vector for b2
    d = dot (b2->pos.dir, v);
    if (d < 0.0) {
        b2->pos.dir += (-2.0f * d) * v;
    }
    /* else: b2 is moving away from the plane, so no change */

  // move the balls apart a bit so that they do not have an immediate collision
    b1->pos.origin = p1 - EPSILON * v;
    b2->pos.origin = p2 + EPSILON * v;

  // reset the ball's timers
    b1->initT = b1->hitT;
    b2->initT = b2->hitT;

}


/********** Rendering code **********/


/* DrawBall:
 */
void DrawBall (View *view, double now, const Ball *ball)
{
    if (SphereMesh == nullptr) {
      // initialize the sphere mesh
	cs237::Sphere sp (BALL_RADIUS);
	SphereMesh = new MeshInfo (GL_TRIANGLES);
	SphereMesh->LoadVertexData (sp.numVerts, sp.verts, sp.norms);
	SphereMesh->LoadIndices (3*sp.numTris, sp.tris);
    }

    cs237::mat4x4f mvMat = cs237::translate(view->modelViewMat, ball->pos.toPoint(now - ball->initT));
    cs237::setUniform (view->render.mvLoc, mvMat);
    cs237::setUniform (view->render.colorLoc, ball->color);

    /* render the mesh */
    view->render.Render (SphereMesh);

} /* end of DrawBall */


/********** Debugging code **********/

std::ostream& operator<< (std::ostream& s, const Ball &b)
{
    return (
	s << static_cast<const void *>(&b) << ":[@" << b.initT << ", " << b.pos.origin << ", " << b.pos.dir << "]");
}
