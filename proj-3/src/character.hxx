/*! \file character.h
 *
 * \author John Reppy & Lamont Samuels
 *
 * Data representation for character animation.
 */
/* CMSC23700 Sample code
 *
 * COPYRIGHT (c) 2010 John Reppy (http://cs.uchicago.edu/~jhr) & 
 *                    Lamont Samuels (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */

#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <stdint.h>
#include "cs237.hxx"

struct Joint;
struct Skin;
struct Vertex;
struct Weight;

typedef Joint *Skeleton_t;


//! Representation of a character animation
struct Character {
    int		nFrames;	//!< number of frames of animation
    int		nSkins;		//!< number of skins of skeleton
    double	duration;	//!< time between frames
    Skin	*skin;		//!< array of skins for the character
    int		nJoints;	//!< the number of joints in the skeletons
    Skeleton_t	*frame;		//!< array of precomputed skeletons
};

//! Representation of a joint in a skeleton
struct Joint {
    int		parent;		//!< index of parent joint (-1 for root)
    cs237::vec3f pos;		//!< the joint's position
    cs237::quatf orient;	//!< the joint's orientation quaternion
};

//! Representation of a vertex mesh used to skin a skeleton
struct Skin {
    int		nVerts;		//!< number of vertices in the mesh
    int		nTris;		//!< number of triangles in the mesh
    std::string	texBase;	//!< base name of the skin's texture
    cs237::vec2f *texCoord;	//!< array of texture coordinates for the vertices
    Vertex	*vert;		//!< vertex info
    unsigned short *elem;	//!< element array (3 per triangle)
};

/*! \brief information needed to compute the position of a vertex.
 *
 * To compute the position of a vertex, we compute the sum of the positions of
 * its weights.
 */
struct Vertex {
    int		nWeights;	//!< number of weights used to compute this vertice's position
    Weight	*weight;	//!< array of weights used to compute this vertice's position
};

//! A weight that connects a skin vertex to a joint in the skeleton
struct Weight {
    int		joint;		//!< joint that this weight is attached to
    float	bias;		//!< value between 0 and 1 that defines the
				//!  contribution of this weight
    cs237::vec3f pos;		//!< the position of the weight in the Joint's
				//!  coordinate system.
};

//! The animation to be rendered for the project
extern Character *GuardData;  

#endif /* !_CHARACTER_H_ */
