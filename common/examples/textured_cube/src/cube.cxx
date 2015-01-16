 /*! \file cube.cxx
 *
 * \brief this file contains function definitions for creating and rendering a cube using GLDrawElements 
 *
 * \author Lamont Samuels 
 */ 

#include "Cube.hxx" 
#include "View.hxx"

Cube::Cube(View * v): view(v), cubeFlatImage(nullptr), cubeFlatTexture(nullptr) { 
	init();  
}
Cube::Cube(View * v, GLfloat length): view(v), cubeFlatImage(nullptr), cubeFlatTexture(nullptr){
	
	length/=2; 
	
	//Increase the size of the cube (could potentially put this in a model matrix and use a scaling transformation to get the desired length)	
	for(int s = 0; s < NUM_SIDES; s++)
		for(int v = 0; v < NUM_VERTICES; v++)
			walls[s].corners[v] *= length; 

	init(); 
} 
void Cube::loadVertices(int i) { 

	//Generate names for the vertices and indices buffer 
	CS237_CHECK(glGenBuffers(1,&walls[i].vbufId)); 

	//Bind the vertices buffer to the context using the GL_ARRAY_BUFFER target 
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,walls[i].vbufId)); 

	// Specify the amount of storage we want use for the vertices buffer 
	CS237_CHECK(glBufferData(GL_ARRAY_BUFFER,sizeof(cs237::vec3f) * NUM_VERTICES,walls[i].corners,GL_STATIC_DRAW)); 

	//set the position vertex attribute pointer 
	CS237_CHECK(glVertexAttribPointer(view->posLoc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));

}
void Cube::loadNorms(int i) { 

	//Bind the vertices buffer to the context using the GL_ARRAY_BUFFER target 
	CS237_CHECK(glGenBuffers(1,&walls[i].nbufId)); 
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,walls[i].nbufId)); 

	cs237::vec3f norms[NUM_VERTICES] = {walls[i].norm,walls[i].norm, walls[i].norm,walls[i].norm};  

	// Specify the amount of storage we want use for the vertices buffer 
	CS237_CHECK(glBufferData(GL_ARRAY_BUFFER,sizeof(cs237::vec3f) * NUM_VERTICES,norms,GL_STATIC_DRAW)); 

	//set the position vertex attribute pointer 
	CS237_CHECK(glVertexAttribPointer(view->normLoc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));

}
void Cube::loadIndices(int i) {

	//Generate names for the vertices and indices buffer 
	CS237_CHECK(glGenBuffers(1,&walls[i].ibufId)); 

	//Bind the indices buffer to the context using the GL_ELEMENT_ARRAY_BUFFER target 
	CS237_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,walls[i].ibufId)); 

	// Specify the amount of storage we want use for the indices buffer 
	CS237_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,NUM_VERTICES * sizeof(unsigned short),indices,GL_STATIC_DRAW)); 

}
void Cube::loadTexCoords(int i){

	//Generate names for the vertices and indices buffer 
	CS237_CHECK(glGenBuffers(1,&walls[i].tbufId)); 

	//Bind the vertices buffer to the context using the GL_ARRAY_BUFFER target 
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,walls[i].tbufId)); 

	// Specify the amount of storage we want use for the vertices buffer 
	CS237_CHECK(glBufferData(GL_ARRAY_BUFFER,sizeof(cs237::vec2f) * NUM_VERTICES,walls[i].texCoords,GL_STATIC_DRAW)); 

	//set the position vertex attribute pointer 
	CS237_CHECK(glVertexAttribPointer(view->texLoc, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0));

}
void Cube::init() { 

	//Initialize the OpenGL buffer information for each wall. 
	for(int i = 0; i < NUM_SIDES; i++) { 
		//generate the vertex array object for this mesh (i.e. the simple cube)
    	CS237_CHECK(glGenVertexArrays (1, &(walls[i].vaoId)));
    	CS237_CHECK(glBindVertexArray (walls[i].vaoId));	

		loadVertices(i); 
		loadNorms(i); 
		loadIndices(i); 
		loadTexCoords(i); 

    	walls[i].prim = GL_TRIANGLE_FAN; 

    	initTextures(); 

		//Break this meshes VAO from the context 
    	CS237_CHECK(glBindVertexArray (0));
	}


}
void Cube::initTextures() {

	this->cubeFlatImage = new cs237::image2d("../data/cubeTex1.png"); 
	this->cubeFlatTexture = new cs237::texture2D(GL_TEXTURE_2D,this->cubeFlatImage);  
	this->cubeFlatTexture->Bind(); 
	this->cubeFlatTexture->Parameter(GL_TEXTURE_MIN_FILTER,GL_LINEAR); 
	this->cubeFlatTexture->Parameter(GL_TEXTURE_MAG_FILTER,GL_LINEAR); 

}
void Cube::render(){

	//Initialize the OpenGL buffer information for each wall. 
	for(int i = 0; i < NUM_SIDES; i++) { 

		//Bind the meshes VAO id to get the state information 
    	CS237_CHECK(glBindVertexArray(walls[i].vaoId));

		//Enable our vertex attributes using the data from the GL_ARRAY_BUFFER 
		CS237_CHECK(glEnableVertexAttribArray(view->posLoc)); 
		CS237_CHECK(glEnableVertexAttribArray(view->normLoc));
		CS237_CHECK(glEnableVertexAttribArray(view->texLoc)); 

		CS237_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, walls[i].ibufId));

		//Activate the first texture unit. 
		CS237_CHECK(glActiveTexture(GL_TEXTURE0));
		//Bind the texture to its target 
		this->cubeFlatTexture->Bind(); 
		//Assign the uniform sampler to the first texture unit. 
		CS237_CHECK(glUniform1i(view->texSamplerLoc,0)); 

		CS237_CHECK(glDrawElements(walls[i].prim,NUM_VERTICES,GL_UNSIGNED_SHORT,(GLvoid*)0)); 

		CS237_CHECK(glDisableVertexAttribArray(view->posLoc));
		CS237_CHECK(glDisableVertexAttribArray(view->normLoc));
		CS237_CHECK(glDisableVertexAttribArray(view->texLoc));
	}
}