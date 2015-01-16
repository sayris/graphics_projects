/*! \file map-cell.cxx
 *
 * \author John Reppy
 *
 * Map cells.
 */

/* CMSC23700 Final project sample code
 *
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "map.hxx"
#include "map-cell.hxx"
#include "qtree-util.hxx"
#include <fstream>
#include <vector>

#define FOVY 65.0f

// A cell file has the following layout on disk.  All data is in little-endian layout.
//
//	uint32_t magic;		// Magic number; should be 0x63656C6C ('cell')
//	uint32_t compressed;	// true if the chunks are compressed
//	uint32_t size;		// cell width (will be width+1 vertices wide)
//	uint32_t nLODs;
//	uint64_t toc[N];	// file offsets of chunks
//
// Each chunk has the layout
//
//	float maxError;		// maximum geometric error for this chunk
//	uint32_t nVerts;	// number of vertices
//	uint32_t nIndices;	// number of indices
//	int16_t minY;		// minimum active elevation value in this chunk
//	int16_t maxY;		// maximum active elevation value in this chunk
//	Vertex verts[nVerts];
//	uint16_t indices[nIndices];
//
// Each Vertex is represented by four 16-bit signed integers.

template <typename T>
inline T readVal (std::ifstream &inS)
{
    T v;
    
    if (inS.read(reinterpret_cast<char *>(&v), sizeof(T)).fail()) {
#ifndef NDEBUG
        std::cerr << "Cell::load: error reading file" << std::endl;
#endif
        exit (1);
    }
    return v;
}

inline int16_t readI16 (std::ifstream &inS) { return readVal<int16_t>(inS); }
inline uint32_t readUI32 (std::ifstream &inS) { return readVal<uint32_t>(inS); }
inline float readF32 (std::ifstream &inS) { return readVal<float>(inS); }
inline uint64_t readUI64 (std::ifstream &inS) { return readVal<uint64_t>(inS); }


/***** class Cell member functions *****/

Cell::Cell (Map *map, uint32_t r, uint32_t c, std::string const &stem)
: _map(map), _row(r), _col(c), _stem(stem), _nLODs(0), _nTiles(0), _tiles(nullptr)
{
}

Cell::~Cell () { }

// load the cell data
void Cell::load ()
{
    if (this->isLoaded())
        return;
    
    std::string file = this->_stem + "/hf.cell";
    std::ifstream inS(file, std::ifstream::in | std::ifstream::binary);
    if (inS.fail()) {
#ifndef NDEBUG
        std::cerr << "Cell::load: unable to open \"" << file << "\"" << std::endl;
#endif
        exit (1);
    }
    
    // get header info
    uint32_t magic = readUI32(inS);
    bool compressed = (readUI32(inS) != 0);
    uint32_t size = readUI32(inS);
    uint32_t nLODs = readUI32(inS);
    
    if ((magic != Cell::MAGIC)
        || (this->_map->_cellSize != size)
        || (nLODs < Cell::MIN_NUM_LODS) || (Cell::MAX_NUM_LODS < nLODs)) {
#ifndef NDEBUG
        std::cerr << "Cell::load: bogus file header" << std::endl;
#endif
        exit (1);
    }
    
    if (compressed) {
        std::cerr << "Cell::load: compressed files are not supported yet" << std::endl;
        exit (1);
    }
    
    uint32_t qtreeSize = QTree::fullSize(nLODs);
    std::vector<std::streamoff> toc(qtreeSize);
    for (int i = 0;  i < qtreeSize;  i++) {
        toc[i] = static_cast<std::streamoff>(readUI64(inS));
    }
    
    // allocate and load the tiles.  Note that tiles are numbered in a breadth-first
    // order in the LOD quadtree.
    this->_nLODs = nLODs;
    this->_nTiles = qtreeSize;
    this->_tiles = new Tile[qtreeSize];
    
    this->_tiles[0]._init (this, 0, 0, 0, 0);
	
    //load in the Texture Quad Tree
	this->LoadTQT();
    
    // load the tile mesh data
    for (uint32_t id = 0;  id < qtreeSize;  id++) {
        Chunk *cp = &(this->_tiles[id]._chunk);
        // find the beginning of the chunk in the input file
        inS.seekg(toc[id]);
        
        // read the chunk's header
        cp->_maxError = readF32(inS);
        uint32_t nVerts = readUI32(inS);
        uint32_t nIndices = readUI32(inS);
        cp->_minY = readI16(inS);
        cp->_maxY = readI16(inS);
        
        // allocate space for the chunk data
        this->_tiles[id]._allocChunk(nVerts, nIndices);
        
        // read the vertex data
        if (inS.read(reinterpret_cast<char *>(cp->_vertices), cp->vSize()).fail()) {
            std::cerr << "Cell::load: error reading vertex data for tile " << id << std::endl;
            exit (1);
        }
        
        // read the index array
        if (inS.read(reinterpret_cast<char *>(cp->_indices), cp->iSize()).fail()) {
            std::cerr << "Cell::load: error reading index data for tile " << id << std::endl;
            exit (1);
        }
        
        // get world space coords for NW corner of Cell
        cs237::vec3d nwCellCoords = this->_map->nwCellCorner(this->_row,this->_col);
        
        // location of NW corner of Tile, in hScale units, relative to its Cell's NW corner
        cs237::vec3d nwTileUnits = cs237::vec3d(this->_tiles[id].nwCol(), 0,
                                                this->_tiles[id].nwRow());
        
        // world coords of NW corner of Tile is:
        // world space coords of NW corner of Cell, plus world coords of NW corner of Tile
        cs237::vec3d nwCorner = nwCellCoords + nwTileUnits * (double) this->_map->hScale();
        
        nwCorner.y = static_cast<double>(
                                         this->_map->baseElevation() +
                                         this->_map->vScale() *
                                         static_cast<float>(cp->_minY));
        
        // width of the Tile in world space
        double tileWidth = this->_tiles[id].width() * this->_map->hScale();
        
        // SE corner of Tile is its NW corner plus the width of the Tile in world space
        cs237::vec3d seCorner = nwCorner + cs237::vec3d(tileWidth,0,tileWidth);
        
        seCorner.y = static_cast<double>(
                                         this->_map->baseElevation() +
                                         this->_map->vScale() *
                                         static_cast<float>(cp->_maxY));
        
        this->_tiles[id]._bbox = cs237::AABBd(nwCorner, seCorner);
        this->_tiles[id]._mesh = nullptr;
        this->_tiles[id]._vertices = nullptr;
        this->_tiles[id]._tanVecs = nullptr;
        this->_tiles[id]._texCoords = nullptr;
        this->_tiles[id]._tex = false;


		
		// Load mesh info
		//this->_tiles[id]._loadMeshInfo();
		
		// Load texture info
        //		if(this->_tiles[id].lod() < this->texTQT->getDepth()){
        //			this->_tiles[id]._LoadTex();
        //			this->_tiles[id]._loadTexInfo();
        //		}
    }
}


// Renders the cell
void Cell::RenderCell(View *view,bool tex){
	if(view->screen_space_error == -1){
		view->screen_space_error = this->tile(0).ScreenError(view);
	}
	
	tile(0).RenderTile(view, tex);
}

// loads a texture quad tree
void Cell::LoadTQT() {
	std::string tex_file = this->datafile("/color.tqt");
	std::string norm_file = this->datafile("/norm.tqt");
	
	if(this->map()->hasColorMap()){
		this->texTQT = new TQT::TextureQTree(tex_file.c_str());
	}
    
	if(this->map()->hasNormalMap()){
		this->normTQT = new TQT::TextureQTree(norm_file.c_str());
	}
}



/***** class Tile member functions *****/

Tile::Tile ()
{
    this->_chunk._nVertices = 0;
    this->_chunk._nIndices = 0;
    this->_chunk._vertices = nullptr;
    this->_chunk._indices = nullptr;
}

Tile::~Tile ()
{
    delete this->_chunk._vertices;
    delete this->_chunk._indices;
}

void Tile::_allocChunk (uint32_t nv, uint32_t ni)
{
    this->_chunk._nVertices = nv;
    this->_chunk._nIndices = ni;
    this->_chunk._vertices = new Vertex[nv];
    this->_chunk._indices = new uint16_t[ni];
}

// initialize the _cell, _id, etc. fields of this tile and its descendants.  The chunk and
// bounding box get set later
void Tile::_init (Cell *cell, uint32_t id, uint32_t row, uint32_t col, uint32_t lod)
{
    this->_cell = cell;
    this->_id = id;
    this->_row = row;
    this->_col = col;
    this->_lod = lod;
    //std::clog << "tile[" << id << "] row = " << this->_row << ", col = " << this->_col
    //<< ", lod = " << this->_lod << std::endl;
    
    if (lod+1 < cell->depth()) {
        uint32_t halfWid = (cell->width() >> (lod+1));
        struct { uint32_t dr, dc; } offset[4] = {
            { 0,       0       }, // NW
            { 0,       halfWid }, // NE
            { halfWid, halfWid }, // SE
            { halfWid, 0       }, // SW
        };
        
        int kidId = QTree::nwChild(id);
        for (int i = 0;  i < 4;  i++) {
            this->child(i)->_init (cell, kidId+i, row+offset[i].dr, col+offset[i].dc, lod+1);
        }
    }
}

// assigns different color to each LOD being rendered
void Tile::_ChunkColor (View *view){
	cs237::color3ub *color_Array = new cs237::color3ub[6];
    switch (this->lod()){
		case 0:
			//green
			color_Array[0] = cs237::color3ub {255, 204, 204};
			cs237::setUniform(view->terrainRender.colorLoc, color_Array[0]);
			break;
		case 1:
			//maroon
			color_Array[1] = cs237::color3ub {102, 0, 53};
			cs237::setUniform(view->terrainRender.colorLoc, color_Array[1]);
			break;
		case 2:
			//lilac
			color_Array[2] = cs237::color3ub {127, 0, 254};
			cs237::setUniform(view->terrainRender.colorLoc, color_Array[2]);
			break;
		case 3:
			//orange
			color_Array[3] = cs237::color3ub {255, 128, 0};
			cs237::setUniform(view->terrainRender.colorLoc, color_Array[3]);
			break;
		case 4:
			//blue
			color_Array[4] = cs237::color3ub {0, 128, 255};
			cs237::setUniform(view->terrainRender.colorLoc, color_Array[4]);
			break;
		default:
			cs237::setUniform(view->terrainRender.colorLoc, cs237::color3ub {0, 0, 0});
			break;
    }
}

// calculates the position of the vertices
void Tile::_VertPos(){
    //    std::cout<<"_VertPos: entering the function\n";
    
	int i;
	int x_offset, y_offset;
	int row				= this->_cell->row();
	int col				= this->_cell->col();
	int num_verts		= this->_chunk._nVertices;
	float hScale		= this->_cell->map()->hScale();
	float vScale		= this->_cell->map()->vScale();
	float baseElevation = this->_cell->map()->baseElevation();
    
    //    std::cout<<"_VertPos: extracting all the necessary values\n";
    
	this->_vertices = new cs237::vec4f[num_verts];
    //    std::cout<<"_VertPos: allocated space for storing the vertices within the tile struct\n";
    
	
	x_offset = this->_cell->width() * col;
	y_offset = this->_cell->width() * row;
    
    //    std::cout<<"_VertPos: calculated x and y offset\n";
    
    if(this->_cell->map()->name() == "Grand Canyon"){
        vScale = 1.0;
        hScale = 1.0;
    }
    
	for (i=0; i < num_verts; i++){
		this->_vertices[i].x=(float)(this->_chunk._vertices[i]._x + x_offset)*hScale;
		this->_vertices[i].y=(float)((this->_chunk._vertices[i]._y )*vScale) + baseElevation;
		this->_vertices[i].z=(float)(this->_chunk._vertices[i]._z + y_offset)*hScale;
		this->_vertices[i].w=(float)(this->_chunk._vertices[i]._morphDelta);
	}
    
    //    std::cout<<"_VertPos: calculated vertex position\n";
    
}

// calculates the texture coordinates
void Tile::_TexPos(){
    //    std::cout<<"_TexPos: entering the function\n";
    
    
	int i;
	int x_offset, y_offset;
	int tRow			= this->nwRow();
	int tCol			= this->nwCol();
	int num_verts		= this->_chunk._nVertices;
	int effLOD			= (this->lod() > this->_cell->colorLOD()) ? this->_cell->colorLOD() : this->lod();
	int e_width			= this->_cell->width() >> effLOD;
	
	
	this->_texCoords = new cs237::vec2f[num_verts];
    //    std::cout<<"_TexPos: extracting all the necessary values\n";
    
	
	x_offset = tCol % e_width;
	y_offset = tRow % e_width;
    
    //    std::cout<<"_TexPos: calculated x and y offset\n";
    
	
	for (i=0; i < num_verts; i++){
		this->_texCoords[i].x=(float)(this->_chunk._vertices[i]._x + x_offset)/e_width;
		
		// was doing this with x_offset in original function
		this->_texCoords[i].y=(float)(this->_chunk._vertices[i]._z + y_offset)/e_width;
	}
    
    //    std::cout<<"_TexPos: calculated vertex position\n";
    
}


// calculates the texture coordinates
void Tile::_TanPos(){
    //    std::cout<<"_TanPos: entering the function\n";
    
	int i;
    int num_verts		= this->_chunk._nVertices;
    cs237::vec3f tangent;
    //    std::cout<<"_TanPos: before calling chunk._indices\n";
    uint16_t* index_array = this->_chunk._indices;
    //    std::cout<<"_TanPos: after calling chunk._indices\n";
    
	
	this->_tanVecs = new cs237::vec3f[num_verts];
    //    std::cout<<"_TanPos: extracting all the necessary values\n";
    
    //initializing the array of tangent positions to 0
    for(i=0;i<num_verts;i++){
        this->_tanVecs[i]=cs237::vec3f(0.0,0.0,0.0);
        //        std::cout<<"_TanPos: set all tanVecs to 0\n";
        
    }
    
	
	for(i=0;i<num_verts;i +=3){
        cs237::vec4f v0_four=this->_vertices[index_array[i]];
        //        std::cout<<"_TanPos: extracting index ["<<index_array[i]<<"] from vertices\n";
        
        cs237::vec4f v1_four=this->_vertices[index_array[i+1]];
        //        std::cout<<"_TanPos: extracting index ["<<index_array[i+1]<<"] from vertices\n";
        
        cs237::vec4f v2_four=this->_vertices[index_array[i+2]];
        //        std::cout<<"_TanPos: extracting index ["<<index_array[i+2]<<"] from vertices\n";
        
        
        cs237::vec3f v0=cs237::vec3f(v0_four.x, v0_four.y, v0_four.z);
        cs237::vec3f v1=cs237::vec3f(v1_four.x, v1_four.y, v1_four.z);
        cs237::vec3f v2=cs237::vec3f(v2_four.x, v2_four.y, v2_four.z);
        
        //        std::cout<<"_TanPos: converting vertices to vec3f\n";
        
        
        
        
        cs237::vec3f e1 = v1-v0;
        cs237::vec3f e2 = v2-v0;
        
        //        std::cout<<"_TanPos: e1="<<e2<<"   e2="<<e2<<"\n";
        
        
        cs237::vec2f t0 = this->_texCoords[index_array[i]];
        //        std::cout<<"_TanPos: extracting index ["<<index_array[i]<<"] from texCoords\n";
        
        cs237::vec2f t1 = this->_texCoords[index_array[i+1]];
        //        std::cout<<"_TanPos: extracting index ["<<index_array[i+1]<<"] from texCoords\n";
        
        cs237::vec2f t2 = this->_texCoords[index_array[i+2]];
        //        std::cout<<"_TanPos: extracting index ["<<index_array[i+2]<<"] from texCoords\n";
        
        
        float U1 = t1.x - t0.x;
        float V1 = t1.y - t0.y;
        float U2 = t2.x - t0.y;
        float V2 = t2.y - t0.y;
        
        //        std::cout<<"_TanPos: calculated U1 U2 V1 V2\n";
        
        
        float f = 1.0f/(U1 * V2 - U2 * V1);
        
        tangent.x = f * (V2 * e1.x - V1 * e2.x);
        tangent.y = f * (V2 * e1.y - V1 * e2.y);
        tangent.z = f * (V2 * e1.z - V1 * e2.z);
        //        std::cout<<"_TanPos: calculated tangent\n";
        
        
        //        bitangent.x = f * (-U2 * e1.x - U1 * e2.x);
        //        bitangent.y = f * (-U2 * e1.y - U1 * e2.y);
        //        bitangent.z = f * (-U2 * e1.z - U1 * e2.z);
        
        
        this->_tanVecs[index_array[i]]+=tangent;
        this->_tanVecs[index_array[i+1]]+=tangent;
        this->_tanVecs[index_array[i+2]]+=tangent;
        
        //        std::cout<<"_TanPos: summing of tanVecs\n";
        
    }
    
    //    std::cout<<"_TanPos: calculated tangent position\n";
    
}

// loads in the textures images
void Tile::_LoadTex(){
        std::cout<<"_LoadTex: entering the function\n";
    
	int effLOD = (this->lod() > this->_cell->colorLOD()) ? this->_cell->colorLOD() : this->lod();
	int e_width	= this->_cell->width() >> effLOD;
	int tRow    = this->nwRow();
	int tCol    = this->nwCol();
	int tex_row = floor(tRow/e_width);
    int tex_col = floor(tCol/e_width);
    
        std::cout<<"_LoadTex: extracting all the necessary values\n";
    
	
	if(this->_cell->colorMap()->isValid()){
            std::cout<<"_LoadTex: colorMap() is valid!!\n";
        
		cs237::image2d *texImg = this->_cell->colorMap()->loadImage(effLOD, tex_row, tex_col, false);
                std::cout<<"_LoadTex: (colorMap) extracting the image\n";
        
		this->_textures[0] = new cs237::texture2D (GL_TEXTURE_2D, texImg);
		this->_textures[0]->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        this->_textures[0]->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //this->_textures[0]->Parameter (GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //this->_textures[0]->Parameter (GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //this->_textures[0]->Parameter (GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



        
                std::cout<<"_LoadTex: (colorMap) storing within this->_textures[0]\n";
        
	}
	
	if(this->_cell->normalMap()->isValid()){
                std::cout<<"_LoadTex: normalMap() is valid!!\n";
        
		cs237::image2d *normImg = this->_cell->normalMap()->loadImage(effLOD, tex_row, tex_col, false);
                std::cout<<"_LoadTex: (normMap) extracting the image\n";
        
		this->_textures[1] = new cs237::texture2D (GL_TEXTURE_2D, normImg);
		this->_textures[1]->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        this->_textures[1]->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //this->_textures[1]->Parameter (GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //this->_textures[1]->Parameter (GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //this->_textures[1]->Parameter (GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        
                std::cout<<"_LoadTex: (normMap) storing within this->_textures[1]\n";
        
	}
	
	this->_tex = true;
    	std::cout<<"_LoadTex: textures loaded\n";
}

// loads the textures into the Mesh
void Tile::_loadTexInfo() {
    //    std::cout<<"_loadTexInfo: entering the function\n";
    
	
	int num_verts = this->_chunk._nVertices;
	this->_TexPos();
    //    std::cout<<"_loadTexInfo: returning from _TexPos\n";
    
    this->_TanPos();
    //    std::cout<<"_loadTexInfo: returning from _TexPos\n";
    
    
	if(this->_texCoords != nullptr && this->_mesh != nullptr){
        //        std::cout<<"_loadTexInfo: texCoords exist and mesh isnt' a nullptr\n";
        
        this->_mesh->LoadTexCoords(num_verts, this->_texCoords);
        //        std::cout<<"_loadTexInfo: loaded the texture coordinates into the mesh\n";
        
		this->_mesh->LoadTextures(this->_textures);
        //        std::cout<<"_loadTexInfo: loaded the textures into the mesh\n";
        
	}
}


// stores all the information into the mesh
void Tile::_loadMeshInfo(){
    std::cout<<"_loadMeshInfo: entering the function\n";
    
    
	if(this->_mesh == nullptr){
        std::cout<<"_loadMeshInfo: mesh is a nullptr\n";
        
		this->_mesh = new MeshInfo(GL_TRIANGLE_STRIP);
        
        std::cout<<"_loadMeshInfo: assign mesh to be a GL_TRIANGLE_STRIP\n";
        
		
		// store vertex and texture coordinates
		this->_VertPos();
        std::cout<<"_loadMeshInfo:after _VertPos\n";
        
		//this->_TexPos();
		//this->Tangent();
		this->_mesh->LoadVertexData(this->_chunk._nVertices, this->_vertices, nullptr);
        std::cout<<"_loadMeshInfo: LoadedVertexData\n";
        
		this->_mesh->LoadIndices(this->_chunk._nIndices, this->_chunk._indices);
        std::cout<<"_loadMeshInfo: LoadedIndices\n";
        
	}
}

// calculates the screen space error
float Tile::ScreenError (View *view){
	float* result = new float[4];
    CS237_CHECK(glGetFloatv(GL_VIEWPORT, result));
    float viewport_width = result[2];
	
	float coeff = (view->ht) / (2*tan(FOVY * 3.141592653/360));
	float chunk_E = this->chunk()._maxError;
    
	
	// Camera position as a vec3 double
    cs237::vec3d pos = cs237::vec3d((double)view->camPos.x,
									(double)view->camPos.y,
									(double)view->camPos.z);
	
    // Distance from viewerPosition (camera position) to this tile's bounding box
    float D = this->bbox().distanceToPt(pos);
	
	return ((coeff) * (chunk_E/D))/sqrt(2);
	
	//alt
	//return (viewport_width/(2*tan(FOVY/2))) * (this->_chunk._maxError / D);
}

// checks screen space error for rendering purposes
bool Tile::ErrorCheck(View *view)
{
    float screenspace_error = this->ScreenError(view);
    if (screenspace_error <= view->screen_space_error){
        return true;
    }
    else
        return false;
}



// recursively loops over the LODs in the tree and renders if a leaf
// or if has sufficiently low LOD
void Tile::RenderTile (View *view, bool tex){
	
	Tile *child1 = this->child(0);
    Tile *child2 = this->child(1);
    Tile *child3 = this->child(2);
    Tile *child4 = this->child(3);
	
	//returns nullptr if the tile is a leaf, so will render the tile regardless
    if (ErrorCheck(view) || (child1 == nullptr) || (child2 == nullptr) || (child3 == nullptr) || (child4 == nullptr)) {
        std::cout<<"Render Tile: passed test... lod="<<this->lod()<<"\n";
        this->RenderChunk(view, tex);
		return;
    }
	
	child1->RenderTile(view, tex);
	child2->RenderTile(view, tex);
	child3->RenderTile(view, tex);
	child4->RenderTile(view, tex);
}

// Renders the chunk of the given tile
void Tile::RenderChunk (View *view, bool tex){
	
//    std::cout<<"RenderChunk: about to _loadMeshInfo\n";
    this->_loadMeshInfo();
    
	// case of textures
    view->terrainRender.Enable();
//    std::cout<<"RenderChunk: enabled terrainRender\n";
    
    
	if(tex){
//        std::cout<<"RenderChunk: case TEXTURE\n";
        
        if(this->lod() < this->_cell->colorLOD()){
            this->_LoadTex();
//            std::cout<<"RenderChunk: after _LoadTex\n";
            
            this->_loadTexInfo();
//            std::cout<<"RenderChunk: after _loadTexInfo\n";
            
            if (this->_mesh->texs[0] != nullptr) {
//                std::cout<<"RenderChunk: _mesh->texs[0] is not a nullptr\n";
                
                CS237_CHECK(glActiveTexture(GL_TEXTURE0));
                this->_mesh->texs[0]->Bind();
//                std::cout<<"RenderChunk: bound tex 0\n";
                
                
                cs237::setUniform (view->terrainRender.colorTexLoc, 0);
                //std::cout<<"colorTexLoc:"<<view->terrainRender.colorTexLoc<<"\n";
            }
            
            if (this->_mesh->texs[1] != nullptr) {
//                std::cout<<"RenderChunk: _mesh->texs[1] is not a nullptr\n";
                
                CS237_CHECK(glActiveTexture(GL_TEXTURE1));
                this->_mesh->texs[1]->Bind();
//                std::cout<<"RenderChunk: bound tex 1\n";
                
                
                cs237::setUniform (view->terrainRender.normTexLoc, 1);
                //std::cout<<"normTexLoc:"<<view->terrainRender.normTexLoc<<"\n";
            }
            
		}
	}
	
    
//        std::cout<<"RenderChunk: about to set uniforms!\n";
	// set uniforms
	cs237::setUniform(view->terrainRender.mvLoc, view->modelViewMat);
	cs237::setUniform(view->terrainRender.projLoc, view->projectionMat);
	this->_ChunkColor(view);
	
	//lighting variables
	cs237::setUniform(view->terrainRender.sunDir, this->_cell->map()->sunDirection());
	cs237::setUniform(view->terrainRender.sunIntensity, this->_cell->map()->sunIntensity());
	cs237::setUniform(view->terrainRender.ambIntensity, this->_cell->map()->ambientIntensity());
	
	// indicators of which mode to render
	cs237::setUniform(view->terrainRender.lightMode, view->lighting);
    cs237::setUniform(view->terrainRender.fogMode, view->fogMode);
	cs237::setUniform(view->terrainRender.texturingMode, view->renderMode);
    
    cs237::setUniform(view->terrainRender.hScale, view->hScale);
    
    
//        std::cout<<"RenderChunk: done setting uniforms\n";
    
	if(!tex){
//        std::cout<<"RenderChunk: rendering WIREFRAME\n";
        
		CS237_CHECK (glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
		view->terrainRender.Render(this->_mesh);
		CS237_CHECK (glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
        
//        std::cout<<"RenderChunk: after rendering WIREFRAME\n";

	} else {
//        std::cout<<"RenderChunk: rendering texture\n";
        
		view->terrainRender.Render(this->_mesh);
//        std::cout<<"RenderChunk: after terrain Render\n";

        
	}
    
}



