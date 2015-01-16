/*! \file depth_buffer.hxx
 *
 * \author John Reppy & Lamont Samuels
 *
 * \brief Wrapper code around OpenGL frame-buffer objects to support
 * rendering to a depth buffer.
 */
/*
 * COPYRIGHT (c) 2014 John Reppy (http://www.cs.uchicago.edu/~jhr) & Lamont Samuels (http://www.cs.uchicago.edu/~lamonts)
 * All rights reserved.
 */
#ifndef _DEPTH_BUFFER_HXX_
#define _DEPTH_BUFFER_HXX_

#include "cs237.hxx"

class DepthBuffer
{
  public:
  /*! \brief the default constructor for creating a new depth buffer */
    DepthBuffer();

  /*! \brief allocate and initialize a new depth buffer
   *  \param wid the width of the buffer (should be power of 2)
   *  \param ht the height of the buffer (should be power of 2)
   *  \return the new buffer
   *
   * This function allocates and initializes a frame-buffer object
   * and associated depth texture.  The texture is initialized with the
   * following parameters:
   *   - the filter parameters are set to \c GL_LINEAR
   *   - the wrap parameters are set to \c GL_CLAMP_TO_EDGE
   *   - the texture mode is set to \c GL_INTENSITY
   *   - the texture compare mode is set to \c GL_COMPARE_R_TO_TEXTURE
   *   - the texture compare function is set to \c GL_LEQUAL
   * .
   */
    bool Init (unsigned int winWidth, unsigned int winHeight);

  /*! \brief make the depth buffer be the current render target
   *  \param dbuf the depth-buffer object
   *
   * This function makes the depth buffer be the current render target.
   * If called with \c 0, the window becomes the render target.
   */
    void BindForWriting();

  /*! \brief binds the depth texture to the provided texture unit
   *  \param texUnit the binding texture unit 
   */
    void BindForReading(GLenum texUnit);

  private:
    GLuint fboId;		//!< the frame-buffer's ID
    cs237::texture2D *depthBuf;	//!< the depth renderbuffer
    GLuint wid, ht;		//!< the dimensions of the buffer
};

#endif /* !_DEPTH_BUFFER_HXX_ */
