/*! \file cs237-vec4.hxx
 *
 * Support code for CMSC 23700 Winter 2014.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC4_HXX_
#define _CS237_VEC4_HXX_

#ifndef _CS237_HXX_
#error "c237-vec4.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {
	template <typename T>
	struct vec4 {
	    T x, y, z, w;

	    vec4 () : x(T(0)), y(T(0)), z(T(0)), w(T(0)) { }
	    vec4 (vec4 const &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
	    explicit vec4 (T xx) : x(xx), y(T(0)), z(T(0)), w(T(0)) { }
	    explicit vec4 (T xx, T yy) : x(xx), y(yy), z(T(0)), w(T(0)) { }
	    explicit vec4 (T xx, T yy, T zz) : x(xx), y(yy), z(zz), w(T(0)) { }
	    explicit vec4 (T xx, T yy, T zz, T ww) : x(xx), y(yy), z(zz), w(ww) { }
	    explicit vec4 (vec2<T> const &v) : x(v.x), y(v.y), z(T(0)), w(T(0)) { }
	    explicit vec4 (vec2<T> const &v, T zz) : x(v.x), y(v.y), z(zz), w(T(0)) { }
	    explicit vec4 (vec2<T> const &v, T zz, T ww) : x(v.x), y(v.y), z(zz), w(ww) { }
	    explicit vec4 (vec3<T> const &v) : x(v.x), y(v.y), z(v.z), w(T(0)) { }
	    explicit vec4 (vec3<T> const &v, T ww) : x(v.x), y(v.y), z(v.z), w(ww) { }

	    T & operator[] (unsigned int const &i);
	    T const & operator[] (unsigned int const &i) const;

	    vec4 & operator= (vec4 const &v);

	    vec4 & operator+= (T const &s);
	    vec4 & operator+= (vec4 const &v);

	    vec4 & operator-= (T const &s);
	    vec4 & operator-= (vec4 const &v);

	    vec4 & operator*= (T const &s);
	    vec4 & operator*= (vec4 const &v);

	    vec4 & operator/= (T const &s);

	    T length () const;
	    vec4 & normalize ();
	};

      //! \brief output the vector to a stream
      //! \param s the stream to print to
      //! \param v the vector to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, vec4<T> const &v);

    } /* _namespace __detail */

    typedef __detail::vec4<float> vec4f;
    typedef __detail::vec4<double> vec4d;

} /* namespace cs237 */

#endif /* !_CS237_VEC4_HXX_ */
