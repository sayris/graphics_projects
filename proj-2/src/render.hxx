/*! \file render.hxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 2 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _RENDER_HXX_
#define _RENDER_HXX_

//! The different lighting modes
enum LightMode {
    NO_LIGHT = 0,	//!< lighting disabled
    DIR_LIGHT = 1,	//!< directional lighting
    POINT_LIGHT = 2	//!< point lighting
};

//! The different texturing modes
enum TextureMode {
    NO_TEXTURE = 0,	//!< texturing disabled
    TEXTURE = 1,	//!< walls are textured
    BUMP_MAPPING = 2,	//!< walls are textured and bump mapping is enabled
};

enum {
    COLOR_TEX = 0,	//!< texture index for color texture
    BUMP_TEX = 1,	//!< texture index for bump map
    NUM_TEXTURES = 2	//!< number of textures
};

//! the information needed to render a mesh
struct MeshInfo {
    GLuint		vaoId;		//!< vertex-array-object ID for this mesh
    GLuint		vbufId;		//!< vertex-buffer ID for the vertex data
    GLuint		nbufId;		//!< buffer ID for the normal-vector data
    GLuint		tanbufId;	//!< buffer ID for the tangent data
    GLuint		tcbufId;	//!< buffer ID for the texture coordinate data
    GLuint		ebufId;		//!< buffer ID for the index array
    GLenum		prim;		//!< the primitive type for rendering the mesh
					//!< (e.g., GL_TRIANGLES, GL_TRIANGLE_FAN, etc.)
    cs237::texture2D	*texs[NUM_TEXTURES]; //!< Textures
    int			nIndices;	//!< the number of vertex indices

  //! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
  //! loaded separately.
    MeshInfo (GLenum p);

  //! initialize the basic vertex data for the mesh (vertices and normals)
    void LoadVertexData (int nVerts, cs237::vec3f *verts, cs237::vec3f *norms);

  //! initialize the texture-coordinate data buffer for the mesh
    void LoadTangents (int nTans, cs237::vec4f *tangents);

  //! initialize the texture-coordinate data buffer for the mesh
    void LoadTexCoords (int nCoords, cs237::vec2f *tcoords);

  //! initialize the element array for the mesh
    void LoadIndices (int n, const unsigned short *indices);

  //! initialize the textures for the mesh
    void LoadTextures (cs237::texture2D *texs[NUM_TEXTURES]);

};

struct Renderer {
    cs237::ShaderProgram	*shader;	//!< the shader program
  // Uniform variable locations
    int				lightingModeLoc; //!< the lighting mode location
    int				texturingModeLoc; //!< the texturing mode location
    int				mvLoc;		//!< the model-view atrix uniform
    int				projLoc;	//!< the projection matrix uniform
    int				lightLoc;	//!< the light location
    int				colorLoc;	//!< the color uniform
    int				colorTxLoc;	//!< the color texture location
    int				bumpTxLoc;	//!< the bump-map texture location

    Renderer () : shader(nullptr) { }

  //! initialize a renderer from the given shader program
    void Init (std::string const & shaderPrefix);

  //! enable the renderer
    void Enable ();

  //! render a mesh using this renderer
    void Render (const MeshInfo *mesh);

};

#endif // !_RENDER_HXX_
