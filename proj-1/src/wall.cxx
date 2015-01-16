/*! \file wall.c
 *
 * \brief Modelling and rendering of the wall.
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
#include "wall.hxx"
#include "view.hxx"

#define EPSILON		0.00001f

/*! \brief the representation of a wall */
struct Wall {
    cs237::color4f	color;		//!< the wall's color
    cs237::vec3f	norm;		//!< the wall's normal vector
    cs237::vec3f	corner[4];	//!< the corners of the wall
    MeshInfo		*mesh;		//!< the mesh info for renderering the wall

    Wall (
	cs237::color4ub c,
	cs237::vec3f nv,
	cs237::vec3f v0, cs237::vec3f v1, cs237::vec3f v2, cs237::vec3f v3)
	: color(cs237::color4f(c)), norm(nv), corner{v0, v1, v2, v3}, mesh(nullptr)
    { }
};



/*! the five visible walls of the box */
Wall 	Wall[NUM_SIDES] = {
	{ /* left */
	    cs237::color4ub{ 63, 63, 63, 255 },		// color
	    cs237::vec3f{  1.0f,  0.0f,  0.0f },	// norm
	    cs237::vec3f{ -1.0f,  1.0f, -1.0f },	// corner[0]
	    cs237::vec3f{ -1.0f,  1.0f,  1.0f },
	    cs237::vec3f{ -1.0f, -1.0f,  1.0f },
	    cs237::vec3f{ -1.0f, -1.0f, -1.0f }
	},
	{ /* right */
	    cs237::color4ub{ 63, 63, 63, 255 },
	    cs237::vec3f{ -1.0f,  0.0f,  0.0f },
	    cs237::vec3f{  1.0f,  1.0f,  1.0f },
	    cs237::vec3f{  1.0f,  1.0f, -1.0f },
	    cs237::vec3f{  1.0f, -1.0f, -1.0f },
	    cs237::vec3f{  1.0f, -1.0f,  1.0f }
	},
	{ /* top */
	    cs237::color4ub{ 63, 63, 255, 255 },
	    cs237::vec3f{  0.0f, -1.0f,  0.0f },
	    cs237::vec3f{ -1.0f,  1.0f, -1.0f },
	    cs237::vec3f{  1.0f,  1.0f, -1.0f },
	    cs237::vec3f{  1.0f,  1.0f,  1.0f },
	    cs237::vec3f{ -1.0f,  1.0f,  1.0f }
	},
	{ /* bottom */
	    cs237::color4ub{ 255, 63, 63, 255 },
	    cs237::vec3f{  0.0f,  1.0f,  0.0f },
	    cs237::vec3f{ -1.0f, -1.0f,  1.0f },
	    cs237::vec3f{  1.0f, -1.0f,  1.0f },
	    cs237::vec3f{  1.0f, -1.0f, -1.0f },
	    cs237::vec3f{ -1.0f, -1.0f, -1.0f }
	},
	{ /* front */
	    cs237::color4ub{ 0, 0, 0, 0 },
	    cs237::vec3f{  0.0f,  0.0f,  1.0f },
	    cs237::vec3f{  1.0f,  1.0f, -1.0f },
	    cs237::vec3f{ -1.0f,  1.0f, -1.0f },
	    cs237::vec3f{ -1.0f, -1.0f, -1.0f },
	    cs237::vec3f{  1.0f, -1.0f, -1.0f }
	},
	{ /* back */
	    cs237::color4ub{ 63, 255, 63, 255 },
	    cs237::vec3f{  0.0f,  0.0f, -1.0f },
	    cs237::vec3f{ -1.0f,  1.0f,  1.0f },
	    cs237::vec3f{  1.0f,  1.0f,  1.0f },
	    cs237::vec3f{  1.0f, -1.0f,  1.0f },
	    cs237::vec3f{ -1.0f, -1.0f,  1.0f }
	},
    
    
    };

const unsigned short indices[4] = {
    0,1,2,3,
};



/********** Simulation code **********/

/* HitTest:
 */
void HitTest (Ray const &r, Side &w, double &t)
{
    float vx=r.dir[0];
    float vy=r.dir[1];
    float vz=r.dir[2];
    float qx=r.origin[0];
    float qy=r.origin[1];
    float qz=r.origin[2];

    
    
    double tx,ty,tz,t_accum;
    
    
    if (vx>EPSILON){
        tx=(1-BALL_RADIUS-qx)/vx;       // RIGHT wall
    } else {
        tx=(-1+BALL_RADIUS-qx)/vx;      // LEFT wall
    }
    
    if(vy>EPSILON) {
        ty=(1-BALL_RADIUS-qy)/vy;       // TOP wall
    } else {
        ty=(-1+BALL_RADIUS-qy)/vy;      // BOTTOM wall
    }
    
    if(vz>EPSILON){
        tz=(1-BALL_RADIUS-qz)/vz;       // REAR/back side
    } else {
        tz=(-1+BALL_RADIUS-qz)/vz;       // FRONT side
    }
    
    t_accum=std::min(tx,tz); // find minimum of the three times
    t=std::min(t_accum,ty); //time of collision
    
    if(t==tx){                  //set the correct wall
        x= tx>EPSILON ? RIGHT : LEFT;
    } else if(t==ty){
        w= ty>EPSILON ? TOP : BOTTOM;
    } else if(t==tz){
        tw= z>EPSILON ? REAR : FRONT;
    }

    

} /* end of HitTest */

/* WallBounce:
 */
void WallBounce (Ball *b, Side w)
{

    double now=glfwGetTime(); //Lamont: You need to use the next collision field. -1 

    b->pos.origin = b->pos.toPoint(now-(b->initT));     // new origin
    b->initT=now;           // new initial time
    
    switch(w){                          // based on which wall ball collides with, changes velocity vector
        case FRONT:
        case REAR:
            b->pos.dir.z=-1*(b->pos.dir.z);
            break;
        case LEFT:
        case RIGHT:
            b->pos.dir.x=-1*(b->pos.dir.x);
            break;
        case TOP:
        case BOTTOM:
            b->pos.dir.y=-1*(b->pos.dir.y);
            break;
        default:
            break;
    }


} /* end of WallBounce */


/********** Rendering code **********/

static bool isFirst = true;

// initWalls:
//
// Initialize the mesh info for the walls.
//
static void InitWalls ()
{
	int i;
    
    for(i=0;i<6;i++){
        Wall[i].mesh=new MeshInfo(GL_TRIANGLE_FAN);
        Wall[i].mesh->LoadVertices(4,Wall[i].corner);
        Wall[i].mesh->LoadIndices(4,indices);
        Wall[i].mesh->nIndices=4;
        Wall[i].mesh->LoadNormals(1,&Wall[i].norm);     }
}



/* DrawWall:
 *
 * Draw the wall w.
 */
void DrawWall (View *view, Renderer *render, Side w)
{
    if (w == FRONT)
	return;

    if (isFirst) {
	InitWalls ();
	isFirst = false;
    }

  // set the color
    cs237::setUniform (render->colorLoc, Wall[w].color);

    render->Render (Wall[w].mesh);

}













