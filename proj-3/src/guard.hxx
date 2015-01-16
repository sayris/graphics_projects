/*! \file guard.hxx
 *
 * \author Lamont Samuels
 *
 *  \brief The representation of the guard character in the animation.
 */

/*
 * COPYRIGHT (c) 2014 Lamont Samuels (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */
#ifndef _GUARD_HXX_
#define _GUARD_HXX_

struct View; 

#include "cs237.hxx"
#include "render.hxx"
#include "character.hxx"

struct Cube; 

struct SkinInfo {
    cs237::vec3f	*vert;		//!< array for holding a skin's vertices
    cs237::vec3f	*norm;		//!< array for holding a skin's normals (one per vertex)
    cs237::texture2D	*texs[MAX_NUM_TEXS]; //!< textures.
    MeshInfo 		*mesh;		//!< the mesh info for renderering the guard's skin
};

//! a mesh representation for rendering the skeleton
struct SkeletonMesh {
    cs237::vec3f	*verts;		//!< the vertices for the skeleton bones 
    Cube		*joint;		//!< the joints for the skeleton 
    MeshInfo		*boneMesh;	//!< the mesh for the bones 
}; 

/*! \brief The representation of the guard */
class Guard {
  public: 

  //! \brief the default constructor for a Guard
    Guard(); 

  //! the transform to orient the guard standing up at the origin
    cs237::mat4x4f const & ModelMat() const { return this->_modelMat; }

  /*! \brief Draw the joints and bones of a skeleton
   *  \param skel the skeleton to draw
   */
    void RenderSkeleton (View *view, Skeleton_t skel, int mode);
    void RenderWire(View *view, Skeleton_t skel,int mode);
    void RenderGuard(View *view,Skeleton_t skel, int mode);
    void BufferShadow(View *view,Skeleton_t skel, int mode,cs237::mat4f view_mat,cs237::mat4f proj_mat);
    void RenderShadow(View *view,Skeleton_t skel, int mode,cs237::mat4f view_mat,cs237::mat4f proj_mat);
    


    /***** YOUR CODE HERE *****/

  private:
    const Character	*_model;	//!< the information about the animation and model
    SkeletonMesh	*_skeleton;	//!< information for rendering the guard's skeleton
    SkinInfo *_skin;            // info for rendering guard's skin
    cs237::mat4f	_modelMat;      //!< the model matrix for the guard, which maps it to
					//!< origin

};

#endif /* !_GUARD_HXX_ */
