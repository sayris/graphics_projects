/*! \file cs237-mat2x2.hxx
 *
 * Support code for CMSC 23700 Winter 2014.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT2X2_HXX_
#define _CS237_MAT2X2_HXX_

#ifndef _CS237_HXX_
#error "c237-mat2x2.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

	template <typename T>
	struct mat2x2 {
	    vec2<T> cv[2];	// column vectors

	    mat2x2 () {}
	    mat2x2 (mat2x2 const &m);
	    explicit mat2x2 (
		T m00, T m10,	// first column
		T m01, T m11);	// second column
	    explicit mat2x2 (vec2<T> const &c0, vec2<T> const &c1);
	    explicit mat2x2 (mat2x3<T> const &mm);
	    explicit mat2x2 (mat3x2<T> const &mm);
	    explicit mat2x2 (mat3x3<T> const &mm);
	  //! construct a diagonal matrix with diagonal elements initialized to x
	    explicit mat2x2 (T const &x);

	  // return the i'th column vector
	    vec2<T> & operator[] (unsigned int const &i);
	    vec2<T> const & operator[] (unsigned int const &i) const;

	    mat2x2 & operator= (mat2x2 const &mm);

	    mat2x2 & operator+= (T const &s);
	    mat2x2 & operator+= (mat2x2 const &v);

	    mat2x2 & operator-= (T const &s);
	    mat2x2 & operator-= (mat2x2 const &v);

	    mat2x2 & operator*= (T const &s);
	    mat2x2 & operator*= (mat2x2 const &m);

	    mat2x2 inverse() const;
	    mat2x2 transpose() const;
	    T determinant() const;
	};

    } /* _namespace __detail */

    typedef __detail::mat2x2<float> mat2x2f;
    typedef __detail::mat2x2<double> mat2x2d;

} /* namespace cs237 */

#endif /* !_CS237_MAT2X2_HXX_ */
