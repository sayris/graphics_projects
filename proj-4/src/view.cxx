/*! \file view.c
 *
 * \brief This file defines the viewer operations.
 *
 */

/* CMSC23700 Project 4 (Winter 2014)
 
 */

#include "cs237.hxx"
#include "tqt.hxx"
#include "map.hxx"
#include "map-cell.hxx"
#include "view.hxx"
#include "skybox.hxx"

static cs237::mat4f identity_M = cs237::mat4f
(   1.0f, 0.0f, 0.0f, 0.0f,
 0.0f, 1.0f, 0.0f, 0.0f,
 0.0f, 0.0f, 1.0f, 0.0f,
 0.0f, 0.0f, 0.0f, 1.0f);

/* default motion parameters */
#define DEFAULT_X_ROT   0.0f
#define DEFAULT_Y_ROT   0.0f
#define DEFAULT_Z_ROT   0.0f
#define DEFAULT_SCALE   1.00f
#define PI              3.14159265

/* clip planes in world coordinates */
#define NEAR_Z      0.5f
//#define FAR_Z       1000000.0f
#define FOVY        50.0f   /* field-of-view in Y-dimension */

/* The path to the shaders; this is usually set from the compiler command-line.
 * but it defaults to a path relative to the build directory.
 */
#ifndef DATA_DIR
#  define DATA_DIR "../data"
#endif
/*****************************************************************************
 * Init:
 ******************************************************************************/
void View::Init (int argc, const char **argv)
{
    
	// get the mapfile
    if (argc < 2) {
        std::cerr << "usage: proj-4 <map-dir>" << std::endl;
        exit(1);
    }
	
	
	// initialize map
	std::string mapDir(argv[1]);
    if (!this->map.loadMap(mapDir))
    {
        exit(1);
    }
	
	
	this->cell				 = nullptr;
	this->hScale			 = this->map.hScale();
	this->vScale			 = this->map.vScale();
	
	this->wid                = 800;
    this->ht                 = 800;
    this->isVis              = GL_TRUE;
	
    
    /* initialize the motion parameters */
    this->xRot               = DEFAULT_X_ROT;
    this->yRot               = DEFAULT_Y_ROT;
    this->zRot               = DEFAULT_Z_ROT;
    this->xLastIncr          = 0;
    this->yLastIncr          = 0;
    this->fInertia           = cs237::vec2f(-0.5f, 0.0f);
    this->fScale             = DEFAULT_SCALE;
    this->xLast              = -1;
    this->yLast              = -1;
    this->rotate             = true;
    this->x_angle            = 0.005;
    this->y_angle            = 0.005;
	
    this->mouseTracking      = false;
    this->LOD_diff           = 0;
    this->lastFrame          = 0.0;
	
    /* initialize the camera */
    int y                    = this->map.width() * this->map.height()*.5;
    this->camPos             = cs237::vec3f(10, this->map.maxElevation(), -100);
	
    this->camUp              = cs237::vec3f(0.0, 1.0, 0.0);
    /* initialize the animation clock */
    this->lastDraw           = glfwGetTime();
    this->lighting           = NO_LIGHT;
	
    this->cell               = nullptr;
	this->screen_space_error = -1;
	
    this->shouldExit         = false;
    this->needsRedraw        = true;
    this->vert               = false;
	
    this->fogMode            = NOFOG;
    this->renderMode         = WIREFRAME;
    this->skyMode            = NOSKY;
	
    this->far_z = this->map.width() * this->map.hScale() * 1.4;
}

/*****************************************************************************
 * Reset: Reset View Parameters
 ******************************************************************************/
void View::Reset ()
{
    this->xRot      = DEFAULT_X_ROT;
    this->yRot      = DEFAULT_Y_ROT;
    this->zRot      = DEFAULT_Z_ROT;
    this->xLastIncr = 0;
    this->yLastIncr = 0;
    this->fInertia  = cs237::vec2f(-0.5f, 0.0f);
    this->fScale    = DEFAULT_SCALE;
}

/*****************************************************************************
 * InitProjectionMatrix:
 ******************************************************************************/
void View::InitProjMatrix ()
{
    this->projectionMat = cs237::perspective
	(FOVY, (float)this->wid / (float)this->ht, NEAR_Z, this->far_z);
}

/*****************************************************************************
 * FPS: camera moves in first person view style
 * W: moves the camera foward (increasing along z axis)
 * A: moves camera upward (increasing along the y axis)
 * S: moves camera backwards (decreasing along the y axis)
 * D: moves camera downward (decreasing along y axis)
 ******************************************************************************/
void View::FPS(int key, int inc)
{
    inc = this->map.name() == "Grand Canyon" ? 100.0f : inc;
    inc = this->map.name() == "Puget Sound" ? 100.0f : inc;
    inc = this->map.name() == "Synthetic Map 3" ? 30000.0f : inc;


    switch (key){
		case GLFW_KEY_UP:
			//moves up
			this->camPos = cs237::vec3f(this->camPos.x, this->camPos.y, this->camPos.z += inc);
			this->InitModelViewMatrix();
			break;
		case GLFW_KEY_W:
			//raises
			this->camPos = cs237::vec3f(this->camPos.x, this->camPos.y += inc, this->camPos.z);
			this->InitModelViewMatrix();
			break;
		case GLFW_KEY_DOWN:
			//moves down
			this->camPos = cs237::vec3f(this->camPos.x, this->camPos.y, this->camPos.z -= inc);
			this->InitModelViewMatrix();
			break;
		case GLFW_KEY_S:
			//lowers
			this->camPos = cs237::vec3f(this->camPos.x, this->camPos.y -= inc, this->camPos.z);
			this->InitModelViewMatrix();
			break;
		case GLFW_KEY_A:
			//moves sidewats
			this->camPos = cs237::vec3f(this->camPos.x += inc, this->camPos.y, this->camPos.z);
			this->InitModelViewMatrix();
			break;
		case GLFW_KEY_D:
			//lowers
			this->camPos = cs237::vec3f(this->camPos.x -= inc, this->camPos.y, this->camPos.z);
			this->InitModelViewMatrix();
			break;
		case GLFW_KEY_SPACE:
			//lowers
			inc += 10000;
			this->InitModelViewMatrix();
			break;
    }
}

/*****************************************************************************
 * Init: change to make them movable
 * Initialize the model-view matrix based on the view state.
 ******************************************************************************/
void View::InitModelViewMatrix ()
{
    float hori             = this->x_angle;
    float vert             = this->y_angle;
    cs237::vec3f cam_dir   = cs237::vec3f(cos(vert) * sin(hori),
                                          sin(vert),
                                          cos(vert) * cos(hori));
	
    cs237::vec3f cam_right = cs237::vec3f(sin(hori - PI / 2.0f), 0, cos(hori - PI / 2.0f));
	
	
    cs237::vec3f cam_up    = cross(cam_right, cam_dir);
	
    this->viewMat          = cs237::lookAt (this->camPos,
                                            this->camPos + cam_dir,
                                            cam_up);
    this->modelViewMat     = this->viewMat;
    this->inverseModel     = this->modelViewMat.transpose();
    this->skyMat           = this->viewMat;
}

/*****************************************************************************
 * Init: initializes the shader for each renderer
 ******************************************************************************/
void View::InitRenderer ()
{
    
    this->terrainRender.Init (std::string(DATA_DIR "/shader"));
    //this->fogRender.Init(std::string(DATA_DIR "/shader"));
    this->skyRender.Init(std::string(DATA_DIR "/sky"));
}

/*****************************************************************************
 * Init: renders the current state of the animation (move to main.cxx)
 ******************************************************************************/
void View::Render ()
{
    // glClear(GL_DEPTH_BUFFER_BIT);
    // Back-face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	//glEnable (GL_DEPTH_TEST);
    glDepthMask (GL_TRUE);
	
    glClearColor ((float)183/255, (float)195/255, (float)199/255, 1.0f);  // clear the surface
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    // glDisable(GL_CULL_FACE);
	
    if(this->skyMode == SKY){
		for (int i = 0;  i < NUM_SIDES;  i++)
            DrawWall (this, i);
		//glEnable (GL_DEPTH_TEST);
	}
    glEnable(GL_DEPTH_TEST);
    int r;
    int c;
    for (r = 0; r < this->map.nRows(); r++)
    {
        for (c = 0; c < this->map.nCols(); c++)
        {
            this->cell = this->map.cell(r, c);
			
            if(!this->cell->isLoaded())
                this->cell->load();
			
            /* clear the screen */
            switch (this->renderMode){
				case WIREFRAME:
                    if (this->screen_space_error == -1.0f){
                        this->screen_space_error =this->cell->tile(0).ScreenError(this);
                    }
					this->cell->RenderCell(this, false);
					break;
				case TEXTURE:
                    if (this->screen_space_error == -1.0f){
                        this->screen_space_error =this->cell->tile(0).ScreenError(this);
                    }
					this->cell->RenderCell(this, true);
					break;
				default:
                    if (this->screen_space_error == -1.0f){
                        this->screen_space_error =this->cell->tile(0).ScreenError(this);
                    }
					this->cell->RenderCell(this, false);
					break;
            }
        }
    }
}









