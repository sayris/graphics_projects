#include "cs237.hxx"
#include "render.hxx"

/*! The locations of the standard mesh attributes.  The layout directives in the shaders
 * should match these values.
 */
const GLint CoordAttrLoc = 0;   //!< location of vertex coordinates attribute
const GLint NormAttrLoc = 1;    //!< location of vertex normal attribute
const GLint TexCoordAttrLoc = 2;    //!< location of texture coordniate attribute
const GLint TanCoordAttrLoc = 3;    //!< location of texture coordniate attribute





//! create a MeshInfo object by initializing its buffer Ids.  The buffer data is
//! loaded separately.
MeshInfo::MeshInfo (GLenum p)
: vbufId(0), nbufId(0), tcbufId(0), ebufId(0), tvbufId(0), prim(p),
nIndices(0)
{
    // generate the vertex array object for the mesh
    CS237_CHECK( glGenVertexArrays (1, &(this->vaoId)));
}


//! initialize the vertex data buffers for the mesh
void MeshInfo::LoadVertexData (int nVerts, cs237::vec4f *verts, cs237::vec3f *norms)
{
    GLuint ids[2];
    
    // generate buffer IDs for both buffers
    CS237_CHECK( glGenBuffers (2, ids) );
    
    CS237_CHECK( glBindVertexArray (this->vaoId) );
    
    // vertex buffer initialization
    this->vbufId = ids[0];
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->vbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nVerts*sizeof(cs237::vec4f), verts, GL_STREAM_DRAW) );
    CS237_CHECK( glVertexAttribPointer (CoordAttrLoc, 4, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (CoordAttrLoc) );
    
    // normal-vector buffer initialization
    this->nbufId = ids[1];
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->nbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nVerts*sizeof(cs237::vec3f), norms, GL_STREAM_DRAW) );
    CS237_CHECK( glVertexAttribPointer (NormAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (NormAttrLoc) );
    
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


//! initialize the texture-coordinate data buffer for the mesh
void MeshInfo::LoadTangent (int nTan, cs237::vec3f *tanCoords)
{
    CS237_CHECK( glBindVertexArray (this->vaoId) );
    CS237_CHECK( glGenBuffers (1, &this->tvbufId) );
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, this->tvbufId) );
    CS237_CHECK( glBufferData (GL_ARRAY_BUFFER, nTan*sizeof(cs237::vec3f), tanCoords, GL_STATIC_DRAW) );
    CS237_CHECK( glVertexAttribPointer (TanCoordAttrLoc, 3, GL_FLOAT, GL_FALSE, 0, 0) );
    CS237_CHECK( glEnableVertexAttribArray (TanCoordAttrLoc) );
    
    CS237_CHECK( glBindBuffer (GL_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );
}











//! initialize the textures for the mesh
void MeshInfo::LoadTextures (cs237::texture2D *texs[MAX_NUM_TEXS])
{
    for (int i = 0;  i < MAX_NUM_TEXS;  i++) {
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
        //bind uniform variables
        if(this->shader->hasUniform("modelView"))
            this->mvLoc = this->shader->UniformLocation ("modelView");
        else
            this->mvLoc = -1;
        
        if(this->shader->hasUniform("projection"))
            this->projLoc = this->shader->UniformLocation ("projection");
        else
            this->projLoc = -1;
        
        if(this->shader->hasUniform("primColor"))
            this->colorLoc = this->shader->UniformLocation ("primColor");
        else
            this->colorLoc = -1;

        // uniform variables for lighting
        
        if(this->shader->hasUniform("sunDir"))
            this->sunDir = this->shader->UniformLocation("sunDir");
        else
            this->sunDir = -1;

        if(this->shader->hasUniform("intensity"))
            this->sunIntensity = this->shader->UniformLocation("intensity");
        else
            this->sunIntensity = -1;
        
        if(this->shader->hasUniform("ambient"))
            this->ambIntensity = this->shader->UniformLocation("ambient");
        else
            this->ambIntensity = -1;

        // uniform variables for skybox

        if(this->shader->hasUniform("SkyMap"))
            this->skyLoc = this->shader->UniformLocation("SkyMap");
        else
            this->skyLoc = -1;
        
        if(this->shader->hasUniform("skyMode"))
            this->skyModeLoc = this->shader->UniformLocation ("skyMode");
        else
            this->skyModeLoc = -1;
        
        
        // uniform variables for morphing
        
        if(this->shader->hasUniform("morph"))
            this->morphLoc = this->shader->UniformLocation("morph");
        else
            this->morphLoc = -1;
        
        if(this->shader->hasUniform("time"))
            this->timeLoc = this->shader->UniformLocation("time");
        else
            this->timeLoc = -1;
        
        
        // uniform variables for texture locations
        
        if(this->shader->hasUniform("colorMap"))
            this->colorTexLoc = this->shader->UniformLocation ("colorMap");
        else
            this->colorTexLoc = -1;
        
        
        if(this->shader->hasUniform("normMap"))
            this->normTexLoc = this->shader->UniformLocation ("normMap");
        else
            this->normTexLoc = -1;
        
        // uniform variables for indicators of which mode to render
      
        if(this->shader->hasUniform("lightingMode"))
            this->lightMode = this->shader->UniformLocation("lightingMode");
        else
            this->lightMode = -1;
        
        if(this->shader->hasUniform("fogMode"))
            this->fogMode = this->shader->UniformLocation ("fogMode");
        else
            this->fogMode = -1;
     
        if(this->shader->hasUniform("texturingMode"))
            this->texturingMode = this->shader->UniformLocation ("texturingMode");
        else
            this->texturingMode = -1;
        
        
        // uniform variables that are helpful
        if(this->shader->hasUniform("hScale"))
            this->hScale = this->shader->UniformLocation ("hScale");
        else
            this->hScale = -1;

        
    }
}


// enable the renderer
void Renderer::Enable ()
{
    assert (this->shader != nullptr);
    this->shader->Use();
    //printf("entering prim restart\n");
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(0xFFFF);
}


void Renderer::Render (const MeshInfo *mesh)
{
    
//    std::cout<<"Render: vaoId:"<<mesh->vaoId<<"\n";
//    std::cout<<"Render: vbufId:"<<mesh->vbufId<<"\n";
//    std::cout<<"Render: nbufId:"<<mesh->nbufId<<"\n";
//    std::cout<<"Render: tcbufId:"<<mesh->tcbufId<<"\n";
//    std::cout<<"Render: ebufId:"<<mesh->ebufId<<"\n";
//    std::cout<<"Render: tvbufId:"<<mesh->tvbufId<<"\n";
//    std::cout<<"Render: prim:"<<mesh->prim<<"\n";
//    std::cout<<"Render: nIndices:"<<mesh->nIndices<<"\n";

    CS237_CHECK( glBindVertexArray (mesh->vaoId) );
    
    
    // bind the indices array
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->ebufId) );
    
    // render
    CS237_CHECK( glDrawElements (mesh->prim, mesh->nIndices, GL_UNSIGNED_SHORT, 0) );
    
    // cleanup
    CS237_CHECK( glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0) );
    CS237_CHECK( glBindVertexArray (0) );
}