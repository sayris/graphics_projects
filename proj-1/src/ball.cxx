/*! \file ball.c
 *
 * \brief The simulation and rendering code for balls.
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
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

static cs237::color4ub	colors[NUM_COLORS] = {
	{ 204, 255, 102, 255 },		// honeydew
	{ 255,   0, 128, 255 },		// strawberry
	{ 102, 255, 102, 255 },		// flora
	{  0,  255, 255, 255 },		// turqoise
	{ 255, 102, 102, 255 },		// salmon
	{ 255, 111, 207, 255 },		// carnation
	{   0, 128, 255, 255 },		// aqua
	{ 102, 255, 204, 255 },		// spindrift
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
//  \param v this output parameter will hold the random vector.
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
//  \param n defines the range of random numbers
//  \return a random number between 0 and n-1.
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
    ball->color = cs237::color4f(colors[randInRange(NUM_COLORS)]);
    
    Ball::Balls[Ball::NumBalls++] = ball;
    
    return ball;
    
} /* end of NewBall */

/* KillBall:
 */
void KillBall ()
{
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
    cs237::vec3f q=b1->pos.origin-b2->pos.origin;
    cs237::vec3f v=b1->pos.dir-b2->pos.dir;
    
    //Lamont: You need to adjust the ball with the older initT to give a common starting point -2 

    double a=dot(v,v);
    double b=2*dot(q,v);
    double c=dot(q,q)-(4*BALL_RADIUS*BALL_RADIUS);
    double x;
    double root=(b*b)-(4*a*c);      // calculate determinent
    double t1,t2,t;
    
    
    if (root>EPSILON){          // if determinent is positive, proceed with calculation
        t1=(-b+sqrtf(root))/(2*a);
        t2=(-b-sqrtf(root))/(2*a);
        
        if(t1>EPSILON && t2>EPSILON){
            t=fmin(t1,t2);                          // value for t
            x=dot(q,q)+2*dot(q,v)*t+dot(v,v)*t*t;
            x=sqrtf(x);                             // calculate distanct
            
            if(x<=2*BALL_RADIUS){                   // if true
                hitT=t;
                return true;
            } else {                                // all other cases are false
                return false;
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
    
}

/* BallBounce:
 */
void BallBounce (Ball *b1, Ball *b2)
{
    
    cs237::vec3f v1=b1->pos.dir;                // velocity vector of ball 1
    cs237::vec3f v2=b2->pos.dir;                // velocity vector of ball 2
    cs237::vec3f tan1,tan2,new_dir1,new_dir2;
    float x,y;
    double now=glfwGetTime();
    
    

    b1->pos.origin = b1->pos.toPoint(now-(b1->initT));      // new origin for ball 1
    b2->pos.origin = b2->pos.toPoint(now-(b2->initT));      // new origin for ball 2
    
    tan1=normalize(b1->pos.origin-b2->pos.origin);      // finding "tangent plane"
    tan2=normalize(b2->pos.origin-b1->pos.origin);  // finding "tangent plane"
    
    
    b1->initT=now;              // new initial time for ball 1
    b2->initT=now;              // new initial time for ball 1
    
    x=dot(v1,tan1);
    y=dot(v2,tan2);
    
    //Lamont: You should use the dot product of the position and normal to determine if b1 is heading toward the contact
    // plane, then the following dot product will be positive else b1 is moving away from the plane, so no change. Hence
    // the direction shouldn't be changing always. -3
    new_dir1=v1-(2*x*tan1);         // using solution to HW1 problem 4, calculate new velocity vector
    b1->pos.dir=new_dir1;
    
    new_dir2=v2-(2*y*tan2);
    b2->pos.dir=new_dir2;
    
    
}


/********** Rendering code **********/


/* DrawBall:
 */
void DrawBall (View *view, Renderer *render, double now, const Ball *ball)
{
    /* render the mesh */
    if (SphereMesh == nullptr) {
        cs237::Sphere *sphere=new cs237::Sphere(BALL_RADIUS);
        SphereMesh = new MeshInfo(GL_TRIANGLES);
        
        SphereMesh->nIndices=(sphere->numTris)*3;
        SphereMesh->LoadVertices(sphere->numVerts,sphere->verts);
        SphereMesh->LoadIndices((sphere->numTris)*3,sphere->tris);
        SphereMesh->LoadNormals(sphere->numVerts,sphere->norms);
        
    }
    cs237::vec3f position=ball->pos.toPoint(now-(ball->initT));
    
    cs237::setUniform(render->mvLoc,cs237::translate(view->modelViewMat,position));
    cs237::setUniform (render->colorLoc, ball->color);
    render->Render(SphereMesh);
}

/********** Debugging code **********/

std::ostream& operator<< (std::ostream& s, const Ball &b)
{
    return (
            s << static_cast<const void *>(&b) << ":[@" << b.initT << ", " << b.pos.origin << ", " << b.pos.dir << "]");
}
