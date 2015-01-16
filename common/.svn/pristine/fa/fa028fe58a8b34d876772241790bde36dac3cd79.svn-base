 /*! \file simpleCube.cxx
 *
 * \brief this file contains function definitions for creating and rendering a cube using GLDrawElements 
 *
 * \author Lamont Samuels 
 */ 

#include "blendedcube.hxx" 

BlendedCube::BlendedCube() { 
	init();  
}
BlendedCube::BlendedCube(GLfloat length){
	
	length/=2; 
	
	//Increase the size of the cube (could potentially put this in a model matrix and use a scaling transformation to get the desired length)	
	for(int v = 0; v < 8; v++)
		vertices[v].pos *= length; 

	init(); 
} 
void BlendedCube::init() { 

	//generate the vertex array object for this mesh (i.e. the simple cube)
    CS237_CHECK(glGenVertexArrays (1, &(this->vaoId)));
    CS237_CHECK(glBindVertexArray (this->vaoId));

	//Generate names for the vertices and indices buffer 
	CS237_CHECK(glGenBuffers(1,&vbufId)); 
	
	//Bind the vertices buffer to the context using the GL_ARRAY_BUFFER target 
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,vbufId)); 
	
	// Specify the amount of storage we want use for the vertices buffer 
	CS237_CHECK(glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW)); 

	//Generate names for the vertices and indices buffer 
	CS237_CHECK(glGenBuffers(1,&ibufId)); 

	//Bind the indices buffer to the context using the GL_ELEMENT_ARRAY_BUFFER target 
	CS237_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ibufId)); 
	
	// Specify the amount of storage we want use for the indices buffer 
	CS237_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW)); 

	//Break this meshes VAO from the context 
    CS237_CHECK(glBindVertexArray (0));
}
void BlendedCube::render(){

	//Bind the meshes VAO id to get the state information 
    CS237_CHECK(glBindVertexArray (this->vaoId));

    //set the position vertex attribute pointer 
	CS237_CHECK(glBindBuffer(GL_ARRAY_BUFFER,vbufId)); 
	CS237_CHECK(glVertexAttribPointer(this->posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)0));
	CS237_CHECK(glVertexAttribPointer(this->colorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)sizeof(vertices[0].pos))); 
	
	//Enable our vertex attributes using the data from the GL_ARRAY_BUFFER 
	CS237_CHECK(glEnableVertexAttribArray(posLoc)); 
	CS237_CHECK(glEnableVertexAttribArray(colorLoc)); 

	CS237_CHECK(glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, ibufId));
	CS237_CHECK(glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,(GLvoid*)0)); 

	CS237_CHECK(glDisableVertexAttribArray(posLoc));
	CS237_CHECK(glDisableVertexAttribArray(colorLoc));
}
