/*! \file cs237-vec.cxx
 *
 * Support code for CMSC 23700 Winter 2014.
 *
 * Operations on vector values.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#include "cs237.hxx"

namespace cs237 {
    namespace __detail {

	#define PROTO(VEC,T) std::ostream& operator<< (std::ostream& s, VEC<T> const &v)

	//! \brief output the vector to a stream
	//! \param s the stream to print to
	//! \param v the vector to print
	template <typename T> PROTO(vec2,T)
	{
	    return (s << "<" << v.x << "," << v.y << ">");
	}
	template PROTO(vec2,float);
	template PROTO(vec2,double);

	//! \brief output the vector to a stream
	//! \param s the stream to print to
	//! \param v the vector to print
	template <typename T> PROTO(vec3,T)
	{
	    return (s << "<" << v.x << "," << v.y << "," << v.z << ">");
	}
	template PROTO(vec3,float);
	template PROTO(vec3,double);

	//! \brief output the vector to a stream
	//! \param s the stream to print to
	//! \param v the vector to print
	template <typename T> PROTO(vec4,T)
	{
	    return (s << "<" << v.x << "," << v.y << "," << v.z << "," << v.w << ">");
	}
	template PROTO(vec4,float);
	template PROTO(vec4,double);

    } // namespace __detail

} // namespace cs237
