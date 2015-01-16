/*! \file render.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 2 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "render.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint	CoordAttrLoc = 0;	//!< location of vertex coordinates attribute
const GLint	NormAttrLoc = 1;	//!< location of vertex normal attribute
const GLint	TanAttrLoc = 2;		//!< location of tangent vector attribute
const GLint	TexCoordAttrLoc = 3;	//!< location of texture coordniate attribute

/***** struct MeshInfo member functions *****/

//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
MeshInfo::MeshInfo (GLenum p)
    : vbufId(0), nbufId(0), tanbufId(0), tcbufId(0), ebufId(0), prim(p),
      texs{nullptr,nullptr}, nIndices(0)
{
  // generate the vertex array object for the mesh
    CS237_CHECK( glGenVertexArrays (1, &(this->vaoId)) );

}

//! initialize the vertex data buffers for the mesh
void MeshInfo::LoadVertexData (int nVerts, cs237::vec3f *verts, cs237::vec3f *norms)
{
    GLuint ids[2];

  // generate buffer IDs for both buffers
    CS237_CHECK( glGenBuffers (2, ids) );

    CS237_CHECK( glBindVertexArray (this->vaoId) );

  // vertex buffer initialization
    this->vbufId = ids[0];
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->vbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nVerts*sizeof(cs237::vec3f), verts, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (CoordAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (CoordAttrLoc) );

  // normal-vector buffer initialization
    this->nbufId = ids[1];
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->nbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nVerts*sizeof(cs237::vec3f), norms, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (NormAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (NormAttrLoc) );

    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );
}

//! initialize the tangent-vector data buffer for the mesh
void MeshInfo::LoadTangents (int nTans, cs237::vec4f *tangents)
{
    CS237_CHECK( glBindVertexArray (this->vaoId) );
    CS237_CHECK( glGenBuffers (1, &this->tanbufId) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->tanbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nTans*sizeof(cs237::vec4f), tangents, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (TanAttrLoc, 4, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (TanAttrLoc) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );
}

//! initialize the texture-coordinate data buffer for the mesh
void MeshInfo::LoadTexCoords (int nCoords, cs237::vec2f *tcoords)
{
    CS237_CHECK( glBindVertexArray (this->vaoId) );
    CS237_CHECK( glGenBuffers (1, &this->tcbufId) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->tcbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nCoords*sizeof(cs237::vec2f), tcoords, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (TexCoordAttrLoc, 2, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (TexCoordAttrLoc) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );
}

//! initialize the element array for the mesh
void MeshInfo::LoadIndices (int n, const unsigned short *indices)
{
    this->nIndices = n;
    CS237_CHECK( glBindVertexArray (this->vaoId) );
    CS237_CHECK( glGenBuffers (1, &this->ebufId) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, this->ebufId) );
    CS237_CHECK( glBufferData (GL_ELEMENT_ARRAY_BUFFER, n*sizeof(unsigned short), indices, GL_STATIC_DRAW) );
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );
}

//! initialize the textures for the mesh
void MeshInfo::LoadTextures (cs237::texture2D *texs[NUM_TEXTURES])
{
    for (int i = 0;  i < NUM_TEXTURES;  i++) {
	this->texs[i] = texs[i];
    }

}


/***** struct Renderer member functions *****/

// initialize a renderer from the given shader program
void Renderer::Init (std::string const & shaderPrefix)
{
    if (this->shader == nullptr) {
      // load, compile, and link the shader program
	cs237::VertexShader vsh((shaderPrefix + ".vsh").c_str());
	cs237::FragmentShader fsh((shaderPrefix + ".fsh").c_str());
	this->shader = new cs237::ShaderProgram (vsh, fsh);
	if (this->shader == nullptr) {
	    std::cerr << "Cannot build " << shaderPrefix << "\n" << std::endl;
	    exit (1);
	}

      // bind uniform variables
	this->mvLoc = this->shader->UniformLocation ("modelView");
	this->projLoc = this->shader->UniformLocation ("projection");
	this->lightLoc = this->shader->UniformLocation ("ecLight");
	this->lightingModeLoc = this->shader->UniformLocation ("lightingMode");
	this->texturingModeLoc = this->shader->UniformLocation ("texturingMode");
	this->colorLoc = this->shader->UniformLocation ("primColor");
	this->colorTxLoc = this->shader->UniformLocation ("colorMap");
	this->bumpTxLoc = this->shader->UniformLocation ("bumpMap");
    }
}

// enable the renderer
void Renderer::Enable ()
{
    this->shader->Use();
}

void Renderer::Render (const MeshInfo *mesh)
{
    CS237_CHECK( glBindVertexArray (mesh->vaoId) );

  // bind the indices array
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->ebufId) );

  // render
    CS237_CHECK( glDrawElements (mesh->prim, mesh->nIndices, GL_UNSIGNED_SHORT, 0) );

  // cleanup
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );

}
