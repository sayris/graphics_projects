/*! \file view.cxx
 *
 * \author Lamont Samuels 
 */
#include "view.hxx" 

/* clip planes in world coordinates */
#define NEAR_Z		0.2f 
#define FAR_Z		100.0f
#define FOVY		65.0f	/* field-of-view in Y-dimension */

View::View() : shaderProg(nullptr),width(800),height(800){
	init(); 
} 

View::View(int w, int h) : shaderProg(nullptr), width(w), height(h){

	init(); 
} 

/*! \brief performs initializations needed before starting up the view*/ 
void View::init() { 
	/* Initialize the camera vectors for the view matrix */ 
    this->camPos	= cs237::vec3f(0.0, 0.0, 1.0);
    this->camTarget	= cs237::vec3f(0.0, 0.0, 0.0);
    this->camUp		= cs237::vec3f(0.0, 1.0, 0.0);
}

/*! \brief performs any initialization before using the view */  
void View::startup() { 
	
	//Compile and initialize the shaders; 
	initShaders(); 
	    
	//Retrieve the uniform locations for the model and projection matrices 
	modelViewLoc = shaderProg->UniformLocation("modelView"); 
	projectionLoc = shaderProg->UniformLocation("projection"); 
	texSamplerLoc = shaderProg->UniformLocation("texSampler"); 

	//Find the locations of the vertex attributes in the shaders 
	posLoc = shaderProg->AttributeLocation("position"); 
	normLoc = shaderProg->AttributeLocation("norm"); 
	texLoc = shaderProg->AttributeLocation("tCoord"); 

	//Generate the cubes for the application 
	c1 = new Cube(this,5.0f); 

	//Initialize the projection and model-view matrices 
	initProjMatrix(); 
	initModelViewMatrix(); 

}

/*! \brief performs any cleanup before we destroy the view */ 
void View::shutdown() {}

/*! \brief compiles the shaders */ 
void View::initShaders() { 	
    if (this->shaderProg == nullptr) {
		cs237::VertexShader vsh("../data/shader.vert");
		cs237::FragmentShader fsh("../data/shader.frag");
		this->shaderProg = new cs237::ShaderProgram (vsh, fsh);
		if (this->shaderProg == nullptr)
	    	exit (1);
	}
}
/*! \brief initialize the projection matrix based on the view state. */
void View::initProjMatrix(){
	this->projectionMat = cs237::perspective (
		FOVY,
		(float)this->width / (float)this->height,NEAR_Z, FAR_Z); 
}

/*! \brief initialize the model-view matrix based on the view state. */
void View::initModelViewMatrix(){    
    this->modelViewMat = cs237::lookAt(
	this->camPos,
	this->camTarget,
	this->camUp);
}   	
void View::render() { 

	//Declare a black color 
	const GLfloat color[] = { 0.0f, 0.0f , 0.0f, 1.0f}; 
							
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the background to black 
	glClearBufferfv(GL_COLOR,0,color); 

    glEnable(GL_DEPTH_TEST);

    //Begin using the shader program 
	shaderProg->Use();

	//Pass in the modelView and projection matrices 
	cs237::setUniform(modelViewLoc,modelViewMat); 
	cs237::setUniform(projectionLoc,projectionMat);
 	
 	c1->render(); 

}
