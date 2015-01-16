/*! \file cs237-vec3.hxx
 *
 * Support code for CMSC 23700 Winter 2014.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC3_HXX_
#define _CS237_VEC3_HXX_

#ifndef _CS237_HXX_
#error "c237-vec3.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {
	template <typename T>
	struct vec3 {
	    T x, y, z;

	    vec3 () : x(T(0)), y(T(0)), z(T(0)) { }
	    vec3 (vec3 const &v) : x(v.x), y(v.y), z(v.z) { }
	    explicit vec3 (T xx) : x(xx), y(T(0)), z(T(0)) { }
	    explicit vec3 (T xx, T yy) : x(xx), y(yy), z(T(0)) { }
	    explicit vec3 (T xx, T yy, T zz) : x(xx), y(yy), z(zz) { }
	    explicit vec3 (vec2<T> const &v) : x(v.x), y(v.y), z(T(0)) { }
	    explicit vec3 (vec2<T> const &v, T zz) : x(v.x), y(v.y), z(zz) { }
	    explicit vec3 (vec4<T> const &v);  // first three components of a 4-vector

	    T const & operator[] (unsigned int const &i) const;
	    T & operator[] (unsigned int const &i);

	    vec3 & operator= (vec3 const &v);

	    vec3 & operator+= (T const &s);
	    vec3 & operator+= (vec3 const &v);

	    vec3 & operator-= (T const &s);
	    vec3 & operator-= (vec3 const &v);

	    vec3 & operator*= (T const &s);
	    vec3 & operator*= (vec3 const &v);

	    vec3 & operator/= (T const &s);

	    T length () const;
	    vec3 & normalize ();
	};

      //! \brief output the vector to a stream
      //! \param s the stream to print to
      //! \param v the vector to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, vec3<T> const &v);

    } /* _namespace __detail */

    typedef __detail::vec3<float> vec3f;
    typedef __detail::vec3<double> vec3d;

} /* namespace cs237 */

#endif /* !_CS237_VEC3_HXX_ */
