/* CMSC23700 Project 4 */

#ifndef _VIEW_HXX_
#define _VIEW_HXX_

#include "cs237.hxx"
#include "map.hxx"
//#include "map-cell.hxx"
#include "render.hxx"

/*! \brief The current state of the view */
struct View {
    
    //is this necessary?
    GLFWwindow		*window;	//!< the main window
    
    bool		    shouldExit;
    bool		    needsRedraw;
    // Camera state
    cs237::vec3f	camPos;		//!< camera position in world space
    cs237::vec3f	camDir;		//!< camera direction in world space
    cs237::vec3f	camUp;		//!< camera up vector in world space
    cs237::vec3f    camTarget;  //!< camera target in world space
    
    // Motion state
    float		    xRot;		//!< x-axis rotation
    float		    yRot;		//!< y-axis rotation
    float		    zRot;		//!< z-axis rotation
    float           x_angle;
    float           y_angle;
    cs237::vec2f	fInertia;
    cs237::vec2f	fInertiaOld;
    float		    fScale;		//!< for zooming
    
    float           screen_space_error; //!< screen space error to calculate LOD
    
    int			    xLast, yLast;
    int			    xLastIncr, yLastIncr; //!< used to track mouse coords
    bool		    rotate;		//!< when true, the model rotates
    bool		    mouseTracking;	//!< true when tracking mouse to control view
    bool            vert;
    
    // animation state
    double		    lastStep;	//!< time of last simulation step
    double		    lastFrame;
    double          lastDraw;	//!< time of last redraw
    // view info
    cs237::mat4f	modelViewMat;	//!< the current model-view matrix
    cs237::mat4f	projectionMat;	//!< the current projection matrix
    cs237::mat4f    viewMat;    //!< the current view matrix
    cs237::mat4f    skyMat;    //!< the current view matrix
    cs237::mat4f    inverseModel;
	
	
    int			     wid, ht;	//!< window dimensions
    bool		     isVis;		//!< true, when the window is visible
    
    int             LOD_diff;
    float           far_z;
	float			hScale;
	float			vScale;
	
	
    // rendering state
    RenderMode      renderMode;
    FogMode         fogMode;
    SkyMode         skyMode;
    LightMode		lighting;	//!< the current lighting mode
   
	// Renderers
	Renderer		terrainRender;		//!< the renderer
	Renderer        skyRender;

    
    
	// the models
    Map map;		// map to render
	Cell *cell;		// cell currently rendering
   
    // Initialize
	void Init (int argc, const char **argv);
	
	// Resets view parameters
	void Reset();
    
	/*! \brief initialize the projection matrix based on the view state. */
    void InitProjMatrix ();
    
	// moves camera in first person view style
	void FPS(int key, int inc);
	
    /*! \brief initialize the renderer by loading and compiling the shader. */
    void InitRenderer ();
    
       /*! \brief initialize the model-view matrix based on the view state. */
    void InitModelViewMatrix ();
	
	// renders current state of animation
    void Render ();
	
};

#endif /* !_VIEW_HXX_ */
