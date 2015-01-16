/*! \file cs237-vec2.hxx
 *
 * Support code for CMSC 23700 Winter 2014.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_VEC2_HXX_
#define _CS237_VEC2_HXX_

#ifndef _CS237_HXX_
#error "c237-vec2.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {
	template <typename T>
	struct vec2 {
	    T x, y;

	    vec2 () : x(T(0)), y(T(0)) { }
	    vec2 (vec2 const &v) : x(v.x), y(v.y) { }
	    explicit vec2 (T xx) : x(xx), y(T(0)) { }
	    explicit vec2 (T xx, T yy) : x(xx), y(yy) { }
	    explicit vec2 (vec3<T> const &v);  // first two components of 3-vector
	    explicit vec2 (vec4<T> const &v);  // first two components of 4-vector

	  // return the i'th element of the vector
	    T & operator[] (unsigned int const &i);
	    T const & operator[] (unsigned int const &i) const;

	    vec2 & operator= (vec2 const &v);

	    vec2 & operator+= (T const &s);
	    vec2 & operator+= (vec2 const &v);

	    vec2 & operator-= (T const &s);
	    vec2 & operator-= (vec2 const &v);

	    vec2 & operator*= (T const &s);
	    vec2 & operator*= (vec2 const &v);

	    vec2 & operator/= (T const &s);

	    T length () const;
	    vec2 & normalize ();
	};

      //! \brief output the vector to a stream
      //! \param s the stream to print to
      //! \param v the vector to print
	template <typename T>
	std::ostream& operator<< (std::ostream& s, vec2<T> const &v);

    } /* _namespace __detail */

    typedef __detail::vec2<float> vec2f;
    typedef __detail::vec2<double> vec2d;

} /* namespace cs237 */

#endif /* !_CS237_VEC2_HXX_ */
