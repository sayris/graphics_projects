/*! \file wall.c
 *
 * \brief Modelling and rendering of the wall.
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
#include "wall.hxx"
#include "view.hxx"

#define EPSILON		0.00001f

/*! \brief the representation of a wall */
struct Wall {
    cs237::color3ub	color;		//!< the wall's color
    cs237::vec3f	tan;		//!< the wall's tangent vector
    cs237::vec3f	bitan;		//!< the wall's bitangent vector
    cs237::vec3f	norm;		//!< the wall's normal vector
    cs237::vec3f	corner[4];	//!< the corners of the wall
    MeshInfo		*mesh;		//!< the mesh info for renderering the wall
};


/*! the five visible walls of the box */
Wall 	Wall[NUM_SIDES] = {
	{ /* left */
	    cs237::color3ub{ 172, 63, 172 },		// color
	    cs237::vec3f{  0.0f,  0.0f,  1.0f },	// tan
	    cs237::vec3f{  0.0f,  1.0f,  0.0f },	// bitan
	    cs237::vec3f{ -1.0f,  0.0f,  0.0f },	// norm
	    {						// corner
		cs237::vec3f{  1.0f, -1.0f, -1.0f },
		cs237::vec3f{  1.0f, -1.0f,  1.0f },
		cs237::vec3f{  1.0f,  1.0f,  1.0f },
		cs237::vec3f{  1.0f,  1.0f, -1.0f },
	    }
	},
	{ /* right */
	    cs237::color3ub{ 63, 172, 172 },		// color
	    cs237::vec3f{  0.0f,  0.0f, -1.0f },	// tan
	    cs237::vec3f{  0.0f,  1.0f,  0.0f },	// bitan
	    cs237::vec3f{  1.0f,  0.0f,  0.0f },	// norm
	    {						// corner
		cs237::vec3f{ -1.0f, -1.0f,  1.0f },
		cs237::vec3f{ -1.0f, -1.0f, -1.0f },
		cs237::vec3f{ -1.0f,  1.0f, -1.0f },
		cs237::vec3f{ -1.0f,  1.0f,  1.0f },
	    }
	},
	{ /* top */
	    cs237::color3ub{ 63, 63, 255 },		// color
	    cs237::vec3f{ -1.0f,  0.0f,  0.0f },	// tan
	    cs237::vec3f{  0.0f,  0.0f, -1.0f },	// bitan
	    cs237::vec3f{  0.0f, -1.0f,  0.0f },	// norm
	    {						// corner
		cs237::vec3f{  1.0f,  1.0f,  1.0f },
		cs237::vec3f{ -1.0f,  1.0f,  1.0f },
		cs237::vec3f{ -1.0f,  1.0f, -1.0f },
		cs237::vec3f{  1.0f,  1.0f, -1.0f },
	    }
	},
	{ /* bottom */
	    cs237::color3ub{ 255, 63, 63 },		// color
	    cs237::vec3f{ -1.0f,  0.0f,  0.0f },	// tan
	    cs237::vec3f{  0.0f,  0.0f,  1.0f },	// bitan
	    cs237::vec3f{  0.0f,  1.0f,  0.0f },	// norm
	    {						// corner
		cs237::vec3f{  1.0f, -1.0f, -1.0f },
		cs237::vec3f{ -1.0f, -1.0f, -1.0f },
		cs237::vec3f{ -1.0f, -1.0f,  1.0f },
		cs237::vec3f{  1.0f, -1.0f,  1.0f },
	    }
	},
	{ /* front */
	    cs237::color3ub{ 0, 0, 0 },			// color
	    cs237::vec3f{  1.0f,  0.0f,  0.0f },	// tan
	    cs237::vec3f{  0.0f,  1.0f,  0.0f },	// bitan
	    cs237::vec3f{  0.0f,  0.0f,  1.0f },	// norm
	    {						// corner
		cs237::vec3f{ -1.0f, -1.0f, -1.0f },
		cs237::vec3f{  1.0f, -1.0f, -1.0f },
		cs237::vec3f{  1.0f,  1.0f, -1.0f },
		cs237::vec3f{ -1.0f,  1.0f, -1.0f },
	    }
	},
	{ /* back */
	    cs237::color3ub{ 63, 255, 63 },		// color
	    cs237::vec3f{ -1.0f,  0.0f,  0.0f },	// tan
	    cs237::vec3f{  0.0f,  1.0f,  0.0f },	// bitan
	    cs237::vec3f{  0.0f,  0.0f, -1.0f },	// norm
	    {						// corner
		cs237::vec3f{  1.0f, -1.0f,  1.0f },
		cs237::vec3f{ -1.0f, -1.0f,  1.0f },
		cs237::vec3f{ -1.0f,  1.0f,  1.0f },
		cs237::vec3f{  1.0f,  1.0f,  1.0f },
	    }
	},
    };


/********** Simulation code **********/

/* HitTest:
 */
void HitTest (Ray const &r, Side &w, double &t)
{
    int		side = -1, i;
    double	time = INFINITY, d, tmp;

  /* the Wall[] array is organized in pairs, with the first side of a pair being the
   * side at -1 and the second being the side at +1.
   */
    for (i = 0;  i < 3;  i++) {
	d = r.dir[i];
	if (d < -EPSILON) {
	    tmp = (-1.0 + BALL_RADIUS - r.origin[i]) / d;
	    if (tmp < time) {
		time = tmp;
		side = 2*i;
	    }
	}
	else if (d > EPSILON) {
	    tmp = (1.0 - BALL_RADIUS - r.origin[i]) / d;
	    if (tmp < time) {
		time = tmp;
		side = 2*i+1;
	    }
	}
    }

    assert (side >= 0);

    w = static_cast<Side>(side);
    t = time;

} /* end of HitTest */

/* WallBounce:
 */
void WallBounce (Ball *b, Side w)
{
  /* compute the ball's position at the time of impact */
    cs237::vec3f pos = b->pos.toPoint((float)(b->hitT - b->initT));

  /* compute the new direction vector */
    cs237::vec3f nd = b->pos.dir - (2.0f * dot(b->pos.dir, Wall[w].norm)) * Wall[w].norm;

    b->pos.origin = pos;
    b->pos.dir = nd;
    b->initT = b->hitT;

} /* end of WallBounce */


/********** Rendering code **********/

static cs237::texture2D **textures;

// initWalls:
//
// Initialize the mesh info for the walls.
//
static void InitWalls ()
{
  // initialize textures
    //cs237::image2d colorImg(DATA_DIR "/uchic-rgb.png");
    //cs237::image2d bumpImg(DATA_DIR "/uchic-norm08.png");
    //cs237::image2d *images[NUM_TEXTURES] = { &colorImg, &bumpImg };
    /*textures = new cs237::texture2D *[NUM_TEXTURES];
    for (int i = 0;  i < NUM_TEXTURES;  i++) {
	textures[i] = new cs237::texture2D (GL_TEXTURE_2D, images[i]);
	textures[i]->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	textures[i]->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }*/

    for (Side i = LEFT;  i < NUM_SIDES;  i++) {
	if (i == FRONT)
	  // no vertex data for the front side
	    continue;

	Wall[i].mesh = new MeshInfo (GL_TRIANGLE_FAN);

      // initialize the vertex data
	cs237::vec3f norms[4] = { Wall[i].norm, Wall[i].norm, Wall[i].norm, Wall[i].norm };
	Wall[i].mesh->LoadVertexData (4, Wall[i].corner, norms);

      // initialize the tangent coordinates
	cs237::vec4f tanw(Wall[i].tan, 0.0);
	tanw.w = (dot(cross(Wall[i].norm, Wall[i].tan), Wall[i].bitan) < 0.0f) ? -1.0f : 1.0f;
	cs237::vec4f tangent[4] = { tanw, tanw, tanw, tanw };
	Wall[i].mesh->LoadTangents (4, tangent);

      // initialize the texture coordinates
	cs237::vec2f texCoord[4] = {
		cs237::vec2f{ 0.0f, 0.0f },
		cs237::vec2f{ 1.0f, 0.0f },
		cs237::vec2f{ 1.0f, 1.0f },
		cs237::vec2f{ 0.0f, 1.0f }
	    };
	Wall[i].mesh->LoadTexCoords (4, texCoord);

      // initialize the indices
	unsigned short indices[4] = {0, 1, 2, 3};
	Wall[i].mesh->LoadIndices (4, indices);

      // initialize the textures
	//Wall[i].mesh->LoadTextures (textures);
    }

}

/* DrawWall:
 *
 * Draw the wall w.
 */
void DrawWall (View *view, Side w)
{
    if (w == FRONT)
	return;

    if (textures == nullptr) {
	InitWalls ();
    }

  // set the textures
    for (int i = 0;  i < NUM_TEXTURES;  i++) {
	glActiveTexture (GL_TEXTURE0 + i);
	Wall[i].mesh->texs[i]->Bind();
    }

    cs237::setUniform (view->render.colorLoc, Wall[w].color);

    view->render.Render (Wall[w].mesh);

}
