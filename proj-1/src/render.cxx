 /*! \file render.cxx
 *
 * \author John Reppy
 */

/* CMSC23700 Project 1 sample code (Winter 2014)
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "render.hxx"

/***** struct MeshInfo member functions *****/

//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
MeshInfo::MeshInfo (GLenum p)
    : prim(p), nIndices(0)
{
  // generate the vertex array object for the mesh
    CS237_CHECK( glGenVertexArrays (1, &(this->vaoId)) );
    CS237_CHECK( glBindVertexArray (this->vaoId) );

  // generate IDs for the buffers
    GLuint bufIds[3];
    CS237_CHECK( glGenBuffers (3, bufIds) );
    this->vbufId = bufIds[0];
    this->nbufId = bufIds[1];
    this->ebufId = bufIds[2];

}

//! initialize the vertex data buffers for the mesh
void MeshInfo::LoadVertices (int nVerts, cs237::vec3f *verts)
{
	//BindVertexArray attaches VAO to our context
    CS237_CHECK(glBindVertexArray (this->vaoId));
    
	//Bind the vertices buffer to the target GL_ARRAY_BUFFER
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,vbufId));
	
	// Specify the amount of storage we want use for the vertices buffer
	// and "input" data into the buffer
	CS237_CHECK(glBufferData(GL_ARRAY_BUFFER,nVerts*sizeof(cs237::vec3f),verts,GL_STATIC_DRAW));

}

//! initialize the vertex-normal data buffers for the mesh
void MeshInfo::LoadNormals (int nNorms, cs237::vec3f *norms)
{
    //BindVertexArray attaches VAO to our context
    CS237_CHECK(glBindVertexArray (this->vaoId));
    //Bind the vertices buffer to the context using the GL_ARRAY_BUFFER target
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,nbufId));
	// Specify the amount of storage we want use for the vertices buffer
	CS237_CHECK(glBufferData(GL_ARRAY_BUFFER,nNorms*sizeof(cs237::vec3f),norms,GL_STATIC_DRAW));
    
/**** YOUR CODE HERE ****/

}

//! initialize the element array for the mesh
void MeshInfo::LoadIndices (int n, const unsigned short *indices)
{
    //BindVertexArray attaches VAO to our context
    CS237_CHECK(glBindVertexArray (this->vaoId));
 	//Bind the indices buffer to the context using the GL_ELEMENT_ARRAY_BUFFER target
	CS237_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebufId));
	// Specify the amount of storage we want use for the indices buffer
	CS237_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,n*sizeof(const unsigned short),indices,GL_STATIC_DRAW));

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
	this->colorLoc = this->shader->UniformLocation ("color");

      // bind attribute variables
	this->posLoc = this->shader->AttributeLocation ("coord");
	if (this->shader->hasAttribute ("norm"))
	    this->normLoc = this->shader->AttributeLocation ("norm");
	else
	    this->normLoc = -1;
    }
}

// enable the renderer
void Renderer::Enable ()
{
    this->shader->Use();
}

void Renderer::Render (const MeshInfo *mesh)
{
    
    //Bind the meshes VAO id to get the state information
    CS237_CHECK(glBindVertexArray (mesh->vaoId));
    
    //set the position vertex attribute pointer
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,mesh->vbufId));
    
    CS237_CHECK(glVertexAttribPointer(this->posLoc, 3, GL_FLOAT, GL_FALSE,0, (GLvoid*)0));
	CS237_CHECK(glVertexAttribPointer(this->colorLoc, 4, GL_FLOAT, GL_FALSE,0, (GLvoid*)sizeof(cs237::vec3f)));

	
	//Enable our vertex attributes using the data from the GL_ARRAY_BUFFER
	CS237_CHECK(glEnableVertexAttribArray(this->posLoc));
	CS237_CHECK(glEnableVertexAttribArray(this->colorLoc));
    
    
    //if using "direct lighting"
    if(this->shader->hasAttribute ("norm")){
        CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,mesh->nbufId));
        CS237_CHECK(glVertexAttribPointer(this->normLoc, 3, GL_FLOAT, GL_FALSE,0, (GLvoid*)0));
        CS237_CHECK(glEnableVertexAttribArray(this->normLoc));
    }
    
	CS237_CHECK(glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->ebufId));
 	CS237_CHECK(glDrawElements(mesh->prim,mesh->nIndices,GL_UNSIGNED_SHORT,(GLvoid*)0));
    
    CS237_CHECK(glDisableVertexAttribArray(posLoc));
	CS237_CHECK(glDisableVertexAttribArray(colorLoc));
    
    if(this->shader->hasAttribute ("norm")){
        CS237_CHECK(glDisableVertexAttribArray(normLoc));
    }
    
}
