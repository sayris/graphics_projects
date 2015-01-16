/*! \file view.c
 *
 * \brief Modelling and rendering of the guard.
 *
 * \author Lamont Samuels
 */

/* CMSC23700 Sample code
 *
 * COPYRIGHT (c) 2014 Lamont Samuels (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "guard.hxx"
#include "view.hxx"
#include "cube.hxx"

//! Colors to use when rendering a skin as a wireframe (the i'th skin uses SkinColor[i]).
static cs237::color3ub SkinColor[6] = {
	cs237::color3ub{ 255,   0,   0},
	cs237::color3ub{   0, 255,   0 },
	cs237::color3ub{   0,   0, 255},
	cs237::color3ub{ 255, 255,   0},
	cs237::color3ub{ 255,   0, 255},
	cs237::color3ub{   0, 255, 255},
};

static cs237::mat4f BiasMat=cs237::mat4f(0.5f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 0.5f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 0.5f, 0.0f,
                                        0.5f, 0.5f, 0.49f, 1.0f);



// Constructor for a guard.
//
Guard::Guard ()
: _model(GuardData), _skeleton(nullptr),
_modelMat(cs237::rotateX(cs237::scale(.5f), 90.0f))
{
}

/*! \brief Draw the joints and bones of a skeleton
 *  \param skel the skeleton to draw
 */
void Guard::RenderSkeleton (View *view, Skeleton_t skel,int mode)
{
    bool isFirst = (this->_skeleton == nullptr);
    int numVertices = this->_model->nJoints * 2;
    
    if (isFirst) {
        // we need to allocate and initialize space for the bones and joints
        this->_skeleton = new SkeletonMesh;
        this->_skeleton->verts = new cs237::vec3f[numVertices];
        this->_skeleton->joint = new Cube(1.5);
        this->_skeleton->boneMesh = new MeshInfo(GL_LINES);
    }
    
    view->skeletonRender.Enable();
    
    // set the model-view matrix
    cs237::mat4x4f mvMat = view->modelViewMat * this->_modelMat;
    cs237::setUniform (view->skeletonRender.mvLoc, mvMat);
    
    // set projection matrix
    cs237::setUniform (view->skeletonRender.projLoc, view->projectionMat);
    
    //pass render mode to shader
    cs237::setUniform (view->skeletonRender.renderMode,mode);
    
    cs237::setUniform(view->skeletonRender.ShadowLoc,1);
    cs237::setUniform(view->skeletonRender.TextureLoc,0);
    cs237::setUniform(view->skeletonRender.isFloor,false);

    
    // compute bones
    for (int i = 0;  i < this->_model->nJoints;  i++) {
        if (skel[i].parent >= 0) {
            this->_skeleton->verts[i * 2] = skel[i].pos;
            this->_skeleton->verts[i * 2 + 1]  = skel[skel[i].parent].pos;
        }
    }
    cs237::setUniform(view->skeletonRender.colorLoc, cs237::color3ub{0,255,0});
    
    // update bone vertex data
    if (isFirst) {
        this->_skeleton->boneMesh->LoadVertexData(numVertices, this->_skeleton->verts, nullptr);
        
    }
    else {
        this->_skeleton->boneMesh->UpdateVertexData(numVertices, this->_skeleton->verts, nullptr);
    }
    
    // draw bones
    view->skeletonRender.RenderArray(this->_skeleton->boneMesh, numVertices);
    
    // draw joints
    for (int i = 0;  i < GuardData->nJoints;  i++) {
        cs237::mat4x4f translate = cs237::translate(skel[i].pos); // translate
        cs237::setUniform (view->skeletonRender.mvLoc, mvMat * translate);
        this->_skeleton->joint->Draw (view, view->skeletonRender);
    }
    
}


// function for WIREFRAME mode of rendering
void Guard::RenderWire (View *view,Skeleton_t skel,int mode)
{
    int n,m,l,i,t;
    
    
    cs237::vec3f weight_pos= cs237::vec3f(0.0,0.0,0.0);         // accumulation variable for position of weights
    float b;                        // holds bias for each weight, W
    int j;                          // holds index for joint associated with a weight, W
    cs237::vec3f vw;                // holds vector for a weight W's position in J's coordinate space

    
    
    // loop through each of the 6 skins
    for(i=0;i<this->_model->nSkins;i++){
        n=this->_model->skin[i].nVerts;     // number of vertices per skin
        t=this->_model->skin[i].nTris;  // number of triangles per skin
        
        
        // initializing and allocating space for mesh info
        this->_skin = new SkinInfo();
        this->_skin->vert = new cs237::vec3f[n];
        this->_skin->mesh = new MeshInfo(GL_TRIANGLES);
        
        
        // calculate position of verticies in a given skin
        for(m=0;m<n;m++){
            
            // calculate position of Weights
            for(l=0;l<this->_model->skin[i].vert[m].nWeights;l++){
                
                b=GuardData->skin[i].vert[m].weight[l].bias;        // bias
                j=GuardData->skin[i].vert[m].weight[l].joint;       // index for joint
                vw=GuardData->skin[i].vert[m].weight[l].pos;        // W's position in J's coordinate space
                
                
                // summing position of each weight
                // calculate weight based on mathematical equation given in write up
                weight_pos+=b*(skel[j].pos+skel[j].orient.rotate(vw));
            }
            
            // assign position of vertex
            this->_skin->vert[m]=weight_pos;
            
            //reset accumulation variable
            weight_pos=cs237::vec3f(0.0,0.0,0.0);
        }
        
        //enable renderer
        view->wireRender.Enable();
        
        // set the model-view matrix
        cs237::mat4x4f mvMat = view->modelViewMat * this->_modelMat;
        cs237::setUniform (view->wireRender.mvLoc, mvMat);
        
        //setting uniform for the Shadow Location
        cs237::setUniform(view->wireRender.ShadowLoc,1);

        
        // set projection matrix
        cs237::setUniform (view->wireRender.projLoc, view->projectionMat);
        cs237::setUniform(view->wireRender.colorLoc, SkinColor[i]);
        
        //pass render mode to shader
        cs237::setUniform (view->wireRender.renderMode,mode);
        
        // not rendering the floor
        cs237::setUniform(view->wireRender.isFloor,false);

        
        // load vertex,indicie data
        this->_skin->mesh->LoadVertexData(n,this->_skin->vert,nullptr);
        this->_skin->mesh->LoadIndices(t*3,this->_model->skin[i].elem);
        
        
        // draw wireframe
        CS237_CHECK(glPolygonMode(GL_FRONT_AND_BACK,GL_LINE));      // set polygon mode to LINE so doesn't fill in triangles
        view->wireRender.Render(this->_skin->mesh);
        CS237_CHECK(glPolygonMode(GL_FRONT_AND_BACK,GL_FILL));      // reset to default mode
        
    }
}


// function for MODEL mode of rendering
void Guard::RenderGuard (View *view,Skeleton_t skel,int mode)
{
    int n,m,l,i,t;
    
    
    cs237::vec3f weight_pos= cs237::vec3f(0.0,0.0,0.0);         // accumulation variable for position of weights
    float b;                        // holds bias for each weight, W
    int j;                          // holds index for joint associated with a weight, W
    cs237::vec3f vw;                // holds vector for a weight W's position in J's coordinate space
    
    cs237::vec3f vec0,vec1,vec2,norm;
    int v0,v1,v2;
    
    std::string fullpath;
    const char* filename;
    
    
    // loop through each of the 6 skins
    for(i=0;i<this->_model->nSkins;i++){
        n=this->_model->skin[i].nVerts;     // number of vertices per skin
        t=this->_model->skin[i].nTris;  // number of triangles per skin
        
        
        // initializing and allocating space for mesh info

        //Lamont: You are wasting memory by creating your structures every render. You should reuse the first one. -5 
        this->_skin = new SkinInfo();
        this->_skin->vert = new cs237::vec3f[n];
        this->_skin->mesh = new MeshInfo(GL_TRIANGLES);
        
        this->_skin->norm=new cs237::vec3f[n];       // array carrying the norms for each of the vertices

        // calculate position of verticies in a given skin
        for(m=0;m<n;m++){
            
            // calculate position of Weights
            //Lamont: This should be sepearated into its own function. -3
            for(l=0;l<this->_model->skin[i].vert[m].nWeights;l++){
                
                b=GuardData->skin[i].vert[m].weight[l].bias;        // bias
                j=GuardData->skin[i].vert[m].weight[l].joint;       // index for joint
                vw=GuardData->skin[i].vert[m].weight[l].pos;        // W's position in J's coordinate space
                
                
                // summing position of each weight
                // calculate weight based on mathematical equation given in write up
                weight_pos+=b*(skel[j].pos+skel[j].orient.rotate(vw));
            }
            
            // assign position of vertex
            this->_skin->vert[m]=weight_pos;
            
            //reset accumulation variable
            weight_pos=cs237::vec3f(0.0,0.0,0.0);
            
            //initializing vert_norms to (0.0,0.0,0.0)
            this->_skin->norm[m]=cs237::vec3f(0.0,0.0,0.0);
        }
        
        
        
		// calculate normals for triangles by taking cross-product of adjacent edges and then normalizing
		for(l=0;l<(t*3)-2;l++){
			v0=this->_model->skin[i].elem[l];
			v1=this->_model->skin[i].elem[++l];     // increment l because want to extract this in sets of 3
            v2=this->_model->skin[i].elem[++l];
            
			vec0=this->_skin->vert[v0];
			vec1=this->_skin->vert[v1];
			vec2=this->_skin->vert[v2];
			
			norm=cross(vec2-vec0,vec1-vec0);
			norm=normalize(norm);
            
			// calculate normals for vertex by summing normals of corresponding triangles
			this->_skin->norm[v0]+=norm;
			this->_skin->norm[v1]+=norm;
			this->_skin->norm[v2]+=norm;
		}
		
		// normalize the values stored within vert_norms
		
		for(m=0;m<n;m++){
			this->_skin->norm[m]=normalize(this->_skin->norm[m]);
		}
		
        // creating the full path to the required texture
        fullpath="../data/"+this->_model->skin[i].texBase+".png";
       
        // convert string into a const char*
        filename=fullpath.c_str();
        
        //load texture coordinates
        this->_skin->mesh->LoadTexCoords(t,this->_model->skin[i].texCoord);
        

        //load image files
        cs237::image2d colorImg(filename,false);
        cs237::image2d *images[MAX_NUM_TEXS]={&colorImg,nullptr,nullptr};

        
        m=0;
        //load and bind textures from images
        // (this will only loop through once...since only doing textures)
        while(images[m]!=nullptr){
            this->_skin->texs[m]=new cs237::texture2D (GL_TEXTURE_2D,images[m]);
            this->_skin->texs[m]->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            this->_skin->texs[m]->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glActiveTexture(GL_TEXTURE0);

            this->_skin->texs[m]->Bind();
            
            m++;
        }
        
        //enable renderer
        view->guardRender.Enable();
        
        //set Texture location
        cs237::setUniform(view->guardRender.TextureLoc,0);
        //set Shadow location
        cs237::setUniform(view->guardRender.ShadowLoc,1);
        //not rendering the floor
        cs237::setUniform(view->guardRender.isFloor,false);



        // set the model-view matrix
        cs237::mat4x4f mvMat= view->modelViewMat * this->_modelMat;
        cs237::setUniform (view->guardRender.mvLoc, mvMat);
        
        // set projection matrix
        cs237::setUniform (view->guardRender.projLoc, view->projectionMat);
        cs237::setUniform(view->guardRender.colorLoc, SkinColor[i]);
        
        //pass render mode to shader
        cs237::setUniform (view->guardRender.renderMode,mode);
        
        // load vertex,indicie, and texture data
        this->_skin->mesh->LoadVertexData(n,this->_skin->vert,this->_skin->norm);
        this->_skin->mesh->LoadIndices(t*3,this->_model->skin[i].elem);
        this->_skin->mesh->LoadTextures(this->_skin->texs);
        
        // draw guard
        view->guardRender.Render(this->_skin->mesh);
        
    }
}


void Guard::BufferShadow (View *view,Skeleton_t skel,int mode,cs237::mat4f view_mat,cs237::mat4f proj_mat)
{
    
    //will write to depth buffer
    view->depthBuffer.BindForWriting();
    
    //clear the depth buffer
    float one = 1.0f;
	CS237_CHECK( glClearBufferfv(GL_DEPTH, 0, &one) );


    int n,m,l,i,t;
    
    
    cs237::vec3f weight_pos= cs237::vec3f(0.0,0.0,0.0);         // accumulation variable for position of weights
    float b;                        // holds bias for each weight, W
    int j;                          // holds index for joint associated with a weight, W
    cs237::vec3f vw;                // holds vector for a weight W's position in J's coordinate space
    
    
    
    // loop through each of the 6 skins
    for(i=0;i<this->_model->nSkins;i++){
        n=this->_model->skin[i].nVerts;     // number of vertices per skin
        t=this->_model->skin[i].nTris;  // number of triangles per skin
        
        
        // initializing and allocating space for mesh info
        this->_skin = new SkinInfo();
        this->_skin->vert = new cs237::vec3f[n];
        this->_skin->mesh = new MeshInfo(GL_TRIANGLES);
        
        
        // calculate position of verticies in a given skin
        for(m=0;m<n;m++){
            
            // calculate position of Weights
            for(l=0;l<this->_model->skin[i].vert[m].nWeights;l++){
                
                b=GuardData->skin[i].vert[m].weight[l].bias;        // bias
                j=GuardData->skin[i].vert[m].weight[l].joint;       // index for joint
                vw=GuardData->skin[i].vert[m].weight[l].pos;        // W's position in J's coordinate space
                
                
                // summing position of each weight
                // calculate weight based on mathematical equation given in write up
                weight_pos+=b*(skel[j].pos+skel[j].orient.rotate(vw));
            }
            
            // assign position of vertex
            this->_skin->vert[m]=weight_pos;
            
            //reset accumulation variable
            weight_pos=cs237::vec3f(0.0,0.0,0.0);
            
        }
        
        //enable renderer
        view->shadowRender.Enable();


        //pass in matrix to transform from light's POV
        cs237::setUniform(view->shadowRender.lightPOV,proj_mat*view_mat*this->_modelMat);
        
        // load vertex,indicie, and texture data
        this->_skin->mesh->LoadVertexData(n,this->_skin->vert,nullptr);
        this->_skin->mesh->LoadIndices(t*3,this->_model->skin[i].elem);
        
        // draw guard
        view->shadowRender.Render(this->_skin->mesh);

    }
    //render floor from light POV... so floor data will get sent to depth buffer
    view->ShadowRenderFloor();
    
    //release depth buffer
    view->BindFrameBuffer();

}

void Guard::RenderShadow (View *view,Skeleton_t skel,int mode,cs237::mat4f view_mat,cs237::mat4f proj_mat)
{
    int n,m,l,i,t;
    
    //enable renderer
    view->bRender.Enable();

    
    cs237::vec3f weight_pos= cs237::vec3f(0.0,0.0,0.0);         // accumulation variable for position of weights
    float b;                        // holds bias for each weight, W
    int j;                          // holds index for joint associated with a weight, W
    cs237::vec3f vw;                // holds vector for a weight W's position in J's coordinate space
    
    cs237::vec3f vec0,vec1,vec2,norm;
    int v0,v1,v2;
    
    std::string fullpath;
    const char* filename;
    
    

    // loop through each of the 6 skins
    for(i=0;i<this->_model->nSkins;i++){
        n=this->_model->skin[i].nVerts;     // number of vertices per skin
        t=this->_model->skin[i].nTris;  // number of triangles per skin
        
        
        // initializing and allocating space for mesh info
        this->_skin = new SkinInfo();
        this->_skin->vert = new cs237::vec3f[n];
        this->_skin->mesh = new MeshInfo(GL_TRIANGLES);
        this->_skin->norm=new cs237::vec3f[n];       // array carrying the norms for each of the vertices

        

        // calculate position of verticies in a given skin
        for(m=0;m<n;m++){
            
            // calculate position of Weights
            for(l=0;l<this->_model->skin[i].vert[m].nWeights;l++){
                
                b=GuardData->skin[i].vert[m].weight[l].bias;        // bias
                j=GuardData->skin[i].vert[m].weight[l].joint;       // index for joint
                vw=GuardData->skin[i].vert[m].weight[l].pos;        // W's position in J's coordinate space
                
                
                // summing position of each weight
                // calculate weight based on mathematical equation given in write up
                weight_pos+=b*(skel[j].pos+skel[j].orient.rotate(vw));
            }
            
            // assign position of vertex
            this->_skin->vert[m]=weight_pos;
            
            //reset accumulation variable
            weight_pos=cs237::vec3f(0.0,0.0,0.0);
            
            //initializing vert_norms to (0.0,0.0,0.0)
            this->_skin->norm[m]=cs237::vec3f(0.0,0.0,0.0);
        }
        

        
		// calculate normals for triangles by taking cross-product of adjacent edges and then normalizing
		for(l=0;l<(t*3)-2;l++){
			v0=this->_model->skin[i].elem[l];
			v1=this->_model->skin[i].elem[++l];     // increment l because want to extract this in sets of 3
            v2=this->_model->skin[i].elem[++l];
            
			vec0=this->_skin->vert[v0];
			vec1=this->_skin->vert[v1];
			vec2=this->_skin->vert[v2];
			
			norm=cross(vec2-vec0,vec1-vec0);
			norm=normalize(norm);
            
			// calculate normals for vertex by summing normals of corresponding triangles
			this->_skin->norm[v0]+=norm;
			this->_skin->norm[v1]+=norm;
			this->_skin->norm[v2]+=norm;
		}
		
		// normalize the values stored within vert_norms
		
		for(m=0;m<n;m++){
			this->_skin->norm[m]=normalize(this->_skin->norm[m]);
		}
		
        // creating the full path to the required texture
        fullpath="../data/"+this->_model->skin[i].texBase+".png";
        
        
        // convert string into a const char*
        filename=fullpath.c_str();
        
        //load texture coordinates
        this->_skin->mesh->LoadTexCoords(t,this->_model->skin[i].texCoord);
        
        
        //load .png files
        cs237::image2d colorImg(filename,false);
        
        //create array of images... this is in case of implementation of bump mapping
        // and specular lighting *hahahahaha*
        cs237::image2d *images[MAX_NUM_TEXS]={&colorImg,nullptr,nullptr};
        
        
        //initialize textures
        m=0;
        //this loop will only go through once... see above comment for explanation why
        while(images[m]!=nullptr){
            this->_skin->texs[m]=new cs237::texture2D (GL_TEXTURE_2D,images[m]);
            this->_skin->texs[m]->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            this->_skin->texs[m]->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            
            glActiveTexture(GL_TEXTURE0);
            this->_skin->texs[m]->Bind();
            m++;
        }
            


        
        // set location for texture and shadow map
        cs237::setUniform(view->bRender.TextureLoc,0);
        cs237::setUniform(view->bRender.ShadowLoc,1);
        
        //not rendering the floor
        cs237::setUniform(view->bRender.isFloor,false);


        // set the model-view matrix
        cs237::mat4x4f mvMat= view->modelViewMat * this->_modelMat;
        cs237::setUniform (view->bRender.mvLoc, mvMat);
        
        // set projection matrix
        cs237::setUniform (view->bRender.projLoc, view->projectionMat);
        cs237::setUniform(view->bRender.colorLoc, SkinColor[i]);
        
        //pass in matrix to transform from light's POV
        cs237::setUniform(view->bRender.lightPOV,proj_mat*view_mat*this->_modelMat);
        
        //pass in the BiasMatrix... to translate and scale coorindates into clip space
        cs237::setUniform(view->bRender.BiasMat,BiasMat);
        
        //pass render mode to shader
        cs237::setUniform (view->bRender.renderMode,mode);
        
        // load vertex,indicie, and texture data
        this->_skin->mesh->LoadVertexData(n,this->_skin->vert,this->_skin->norm);
        this->_skin->mesh->LoadIndices(t*3,this->_model->skin[i].elem);
        this->_skin->mesh->LoadTextures(this->_skin->texs);
        
        //Bind buffer for reading (will read the depth info from depth buffer)
        view->depthBuffer.BindForReading(GL_TEXTURE1);

        // draw guard
        view->bRender.Render(this->_skin->mesh);
    }
    //render floor
    view->RenderFloor();
}









