/*! \file depth-buffer.cxx
 *
 * \brief An implementation of depth buffers based on OpenGL
 * frame-buffer objects.
 *
 * \author John Reppy & Lamont Samuels
 */

/* CMSC23700 Project 3 Sample code
 *
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */

#include "cs237.hxx"
#include "depth-buffer.hxx"

DepthBuffer::DepthBuffer()
    :fboId(0), depthBuf(nullptr), wid(0), ht(0)
{}

/* 
* Init
*/ 
bool DepthBuffer::Init(unsigned int winWidth, unsigned int winHeight)
{
    this->wid = winWidth; 
    this->ht = winHeight; 

  // create the framebuffer
    CS237_CHECK( glGenFramebuffers(1, &fboId) );    
    CS237_CHECK( glBindFramebuffer(GL_FRAMEBUFFER, fboId) );

  // Generate a texture to represent the depth texture
    this->depthBuf = new cs237::texture2D (
	GL_TEXTURE_2D, GL_DEPTH_COMPONENT, winWidth, winHeight,
	GL_DEPTH_COMPONENT, GL_FLOAT);
    this->depthBuf->Parameter (GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    this->depthBuf->Parameter (GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    this->depthBuf->Parameter (GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    this->depthBuf->Parameter (GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);         // allows which z value is stored in texture... need to specify z coordinate for each of the fragments within the fragment shader

    CS237_CHECK( glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthBuf->Id(), 0) );      // bind depth to depth texture... so during 2nd pass can figure out with fragments to be somewhat shaded or not

    // Disable writes to the color buffer
    CS237_CHECK( glDrawBuffer (GL_NONE));

    // check that the FBO is complete
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
	std::cerr << "Depth Buffer error, status = " << status << std::endl;
        exit (EXIT_FAILURE); 
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}



/* 
* BindForWriting
*/ 
void DepthBuffer::BindForWriting()          //call when do first pass... will bind particular depth frame buffer to openGL frame buffer
{
    CS237_CHECK( glBindFramebuffer (GL_FRAMEBUFFER, this->fboId) );
    CS237_CHECK( glViewport (0, 0, this->wid, this->ht) );
}


// then render guard and floor
// then render guard and floor to screen (2nd pass)
// call BindFrameBuffer (in view.cxx)... will bind it back to the screen
/*
 the call BindforReading... need to bind depth texture w/z-values to particular texture unit so can use depth texture inside second phase when doing actual rendering of objects to screen
 then render object to screen
 
 
 
 1- create depth buffer
 2- initialize depth buffer
 3- when calculating z-values call BindforWriting
 4- render both objects
 
 FIRST PASS DONE
 now depth texture now has z-values for rendering
 
 1- call BindFrameBuffer in order to bind back to screen
 2- call BindForWriting given texture unit so can use texture inside shader program
 3- render the object 
 
 when create texturing, need to create
 cs237::image2D(**path_to_texture***,FALSE);   --> this flips the UV coordinates on the texture map... so coordinates start from top left hand corner as opposed to the bottom left hand corner
 
 
 */

/*
* BindForReading
*/
void DepthBuffer::BindForReading (GLenum texUnit)
{
    CS237_CHECK( glActiveTexture (texUnit) );
    CS237_CHECK( glBindTexture (GL_TEXTURE_2D, this->depthBuf->Id()) );
}
