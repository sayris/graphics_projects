/*! \file cs237-util.cxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"

void cs237::__detail::ReportError (GLenum err, const char *file, int line, const char *msg)
{
    const char *error;

    switch (err) {
      case GL_INVALID_ENUM: error = "invalid enum"; break;
      case GL_INVALID_VALUE: error = "invalid value"; break;
      case GL_INVALID_OPERATION: error = "invalid operation"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: error = "invalid framebuffer operation"; break;
      case GL_OUT_OF_MEMORY: error = "out of memory"; break;
      default: error = "unknown"; break;
    }

    std::cerr << "[" << file << ":" << line << "] " << error << "; " << msg << "\n" << std::endl;

    exit (1);
}

