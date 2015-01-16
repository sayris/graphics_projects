#ifndef _RENDER_HXX_
#define _RENDER_HXX_

//! the different types of textures
enum {
    COLOR_TEX = 0,  //!< index of color texture
    SPEC_TEX = 1,   //!< index of specular-map texture
    NORM_TEX = 2,   //!< index of normal-map texture
    MAX_NUM_TEXS //!< max number of textures (not all textures are present for all skins)
};

//! The different lighting modes
enum LightMode {
    NO_LIGHT = 0, //!< lighting disabled
    DIR_LIGHT = 1, //!< directional lighting
};

//! The different mesh modes
enum RenderMode {
    WIREFRAME = 0, //!< draw the wire frame
    TEXTURE = 1,
};

enum FogMode {
    FOG = 0, //!< enables fog
    NOFOG = 1,
};

enum SkyMode {
    NOSKY = 0,
    SKY = 1, //!< enables skybox
};


//! the information needed to render a mesh
struct MeshInfo {
    GLuint      vaoId;      //!< vertex-array-object ID for this mesh
    GLuint      vbufId;     //!< vertex-buffer ID for the vertex data
    GLuint      nbufId;     //!< buffer ID for the normal-vector data
    GLuint      tcbufId;    //!< buffer ID for the texture coordinate data
    GLuint      ebufId;     //!< buffer ID for the index array
    GLuint      tvbufId;    //!< buffer ID for the texture coordinate data
    GLenum      prim;       //!< the primitive type for rendering the mesh
    //!< (e.g., GL_TRIANGLES, GL_TRIANGLE_FAN, etc.)
    
    cs237::texture2D    *texs[MAX_NUM_TEXS]; //!< Textures; nullptr for unused slots
    int         nIndices;   //!< the number of vertex indices
    
    //! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
    //! loaded separately.
    MeshInfo (GLenum p);
    
    //! initialize the basic vertex data for the mesh (vertices and normals)
    void LoadVertexData (int nVerts, cs237::vec4f *verts, cs237::vec3f *norms);
    
    //! initialize the texture-coordinate data buffer for the mesh
    void LoadTexCoords (int nCoords, cs237::vec2f *tcoords);
    
    //! initialize the textures for the mesh
    void LoadTextures (cs237::texture2D *texs[MAX_NUM_TEXS]);
    
    //! initialize the element array for the mesh
    void LoadIndices (int n, const unsigned short *indices);
    
    //! initialize the tangent vectors for the mesh
    void LoadTangent (int nTan, cs237::vec3f *tanVecs);
    
};


struct Renderer {
    cs237::ShaderProgram	*shader;	//!< the shader program
    // Uniform variable locations
    int				mvLoc;		//!< the model-view matrix uniform
    int				projLoc;	//!< the projection matrix uniform
    int				colorLoc;	//!< the color uniform
    
    // uniform variables for lighting
    int             sunDir;
    int             sunIntensity;
    int             ambIntensity;
    
    // uniform variables for skybox
    int             skyLoc;
    int             skyModeLoc;
    
    // uniform variables for morphing
    int             timeLoc;
    int             morphLoc;
    
    // uniform variables for texture locations
    int             colorTexLoc;    // texture map
    int             normTexLoc;     // height map

    
    // uniform variables for indicators of which mode to render
    int             lightMode;          // lighting
    int             fogMode;            // fog
    int             texturingMode;      // texturing
    
    // uniform variables that are helpful
    int              hScale;
    
    // Attribute variable locations
    int				posLoc;		//!< the vertex position attribute
    int				normLoc;	//!< the vertex-normal vector attribute; this is

    
    //int             offsetLoc;
    
    Renderer () : shader(nullptr) { }
    
    //! initialize a renderer from the given shader program
    void Init (std::string const & shaderPrefix);
    
    //! enable the renderer
    void Enable ();
    
    //! render a mesh using this renderer
    void Render (const MeshInfo *mesh);

    
};

#endif // !_RENDER_HXX_

