#include "cs237.hxx"
#include "skybox.hxx"
#include "view.hxx"

#ifndef DATA_DIR
#  define DATA_DIR "../data"
#endif

struct Wall {
	cs237::vec3f	norm;		//!< the wall's normal vector
    cs237::image2d	image;		//!< the wall's color
    cs237::vec4f	corner[4];	//!< the corners of the wall
    MeshInfo		*mesh;		//!< the mesh info for renderering the wall
};
bool scaled = false;
Wall 	Wall[NUM_SIDES] = {

	{ /* left */
	    cs237::vec3f{ -1.0f,  0.0f,  0.0f },	// norm
		cs237::image2d(DATA_DIR "/SunSet/SunSetRight2048.png"),

	    {						// corner
		cs237::vec4f{  1.0f, -1.0f, -1.0f, 1.0f},
		cs237::vec4f{  1.0f, -1.0f, 1.0f, 1.0f},
		cs237::vec4f{  1.0f, 1.0f, 1.0f, 1.0f},
		cs237::vec4f{  1.0f, 1.0f, -1.0f, 1.0f},
	    }
	},
	{ /* right */
		cs237::vec3f{  1.0f,  0.0f,  0.0f },	// norm
		cs237::image2d(DATA_DIR "/SunSet/SunSetLeft2048.png"),
	    {						// corner
		cs237::vec4f{ -1.0f, -1.0f, 1.0f, 1.0f},
		cs237::vec4f{ -1.0f, -1.0f, -1.0f, 1.0f},
		cs237::vec4f{ -1.0f, 1.0f, -1.0f, 1.0f},
		cs237::vec4f{ -1.0f, 1.0f, 1.0f, 1.0f},
	    }
	},
	{ /* top */
		cs237::vec3f{  0.0f, -1.0f,  0.0f },	// norm
		cs237::image2d(DATA_DIR "/SunSet/SunSetUp2048.png"),
	    {						// corner
		cs237::vec4f{ 1.0f,  1.0f, 1.0f, 1.0f},
		cs237::vec4f{ -1.0f,  1.0f, 1.0f, 1.0f},
		cs237::vec4f{ -1.0f,  1.0f, -1.0f, 1.0f},
		cs237::vec4f{ 1.0f,  1.0f, -1.0f, 1.0f},
	    }
	},
	{ /* bottom */
		cs237::vec3f{  0.0f,  1.0f,  0.0f },	// norm
		cs237::image2d(DATA_DIR "/SunSet/SunSetDown2048.png"),
	    {						// corner
		cs237::vec4f{ 1.0f, -1.0f, -1.0f, 1.0f},
		cs237::vec4f{ -1.0f, -1.0f, -1.0f, 1.0f},
		cs237::vec4f{ -1.0f, -1.0f, 1.0f, 1.0f},
		cs237::vec4f{ 1.0f, -1.0f, 1.0f, 1.0f},
	    }
	},
	{ /* back */
		cs237::vec3f{  0.0f,  0.0f,  1.0f },	// norm
		cs237::image2d(DATA_DIR "/SunSet/SunSetBack2048.png"),
	    {						// corner
		cs237::vec4f{ -1.0f, -1.0f, -1.0f, 1.0f},
		cs237::vec4f{ 1.0f, -1.0f, -1.0f, 1.0f},
		cs237::vec4f{ 1.0f, 1.0f, -1.0f, 1.0f},
		cs237::vec4f{ -1.0f, 1.0f, -1.0f, 1.0f},
	    }
	},
	{ /* front */
		cs237::vec3f{  0.0f,  0.0f, -1.0f },	// norm
		cs237::image2d(DATA_DIR "/SunSet/SunSetFront2048.png"),
	    {						// corner
		cs237::vec4f{ 1.0f, -1.0f, 1.0f, 1.0f},
		cs237::vec4f{ -1.0f, -1.0f, 1.0f, 1.0f},
		cs237::vec4f{ -1.0f, 1.0f, 1.0f, 1.0f},
		cs237::vec4f{ 1.0f, 1.0f, 1.0f, 1.0f},
		}
	},
 
 };
/*****************************************************************************
* Inline Functions
******************************************************************************/
void InitBox(View *view){

    cs237::texture2D **sky;
    sky = new cs237::texture2D *[1];
   
    for (Side i = LEFT;  i < NUM_SIDES; i++) {
		sky[0] = new cs237::texture2D (GL_TEXTURE_2D, &Wall[i].image);
			//std::cout << Wall[i].image << "\n"; 
    	sky[0]->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	sky[0]->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	sky[0]->Parameter (GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    	sky[0]->Parameter (GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		Wall[i].mesh = new MeshInfo (GL_TRIANGLE_FAN);

		cs237::vec2f texCoord[4] = {
			cs237::vec2f{ 0.0f, 0.0f },
			cs237::vec2f{ 1.0f, 0.0f },
			cs237::vec2f{ 1.0f, 1.0f },
			cs237::vec2f{ 0.0f, 1.0f }
		};

		Wall[i].mesh->LoadTexCoords (4, texCoord);

		Wall[i].mesh->LoadVertexData (4, Wall[i].corner, nullptr);
		
		unsigned short indices[4] = {0, 1, 2, 3};
		Wall[i].mesh->LoadIndices (4, indices);
		Wall[i].mesh->LoadTextures (sky);
	}
}

void DrawWall (View *view, int i)
{
    view->skyRender.Enable();
  
    if(Wall[i].mesh == nullptr)
		InitBox(view);

   	cs237::setUniform(view->skyRender.skyLoc, 0);
   	cs237::setUniform(view->skyRender.mvLoc, cs237::translate(view->modelViewMat, view->camPos));
    cs237::setUniform(view->skyRender.texturingMode, view->renderMode);
    cs237::setUniform(view->skyRender.projLoc, view->projectionMat);

	glActiveTexture (GL_TEXTURE0);
	Wall[i].mesh->texs[0]->Bind();
	//std::cout << Wall[i].mesh->texs[1] << "\n";

    view->skyRender.Render(Wall[i].mesh);
}



