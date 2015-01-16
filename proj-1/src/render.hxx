/*! \file render.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _RENDER_HXX_
#define _RENDER_HXX_

/*! The different lighting modes */
enum LightMode {
    NO_LIGHT,		//!< lighting disabled
    DIR_LIGHT,		//!< directional lighting
};

//! the information needed to render a mesh
struct MeshInfo {
    GLuint		vaoId;		//!< vertex-array-object ID for this mesh
    GLuint		vbufId;		//! vertex-buffer ID for the vertex data
    GLuint		nbufId;		//! buffer ID for the normal-vector data
    GLuint		ebufId;		//! buffer ID for the index array
    GLenum		prim;		//! the primitive type for rendering the mesh
					//! (e.g., GL_TRIANGLES, GL_TRIANGLE_FAN, etc.)
    int			nIndices;	//! the number of vertex indices
    
    
   
    
    
    

  //! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
  //! loaded separately.
    MeshInfo (GLenum p);

  //! initialize the vertex data buffers for the mesh
    void LoadVertices (int nVerts, cs237::vec3f *verts);

  //! initialize the vertex-normal data buffers for the mesh
    void LoadNormals (int nNorms, cs237::vec3f *norms);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const unsigned short *indices);

};

struct Renderer {
    cs237::ShaderProgram	*shader;	//!< the shader program
  // Uniform variable locations
    int				mvLoc;		//!< the model-view atrix uniform
    int				projLoc;	//!< the projection matrix uniform
    int				colorLoc;	//!< the color uniform
  // Attribute variable locations
    int				posLoc;		//!< the vertex position attribute
    int				normLoc;	//!< the vertex-normal vector attribute; this is
						//!< -1 for the NO_LIGHT renderer.

    Renderer () : shader(nullptr) { }

  // initialize a renderer from the given shader program
    void Init (std::string const & shaderPrefix);

  // enable the renderer
    void Enable ();

  // render a mesh using this renderer
    void Render (const MeshInfo *mesh);

};

#endif // !_RENDER_HXX_
