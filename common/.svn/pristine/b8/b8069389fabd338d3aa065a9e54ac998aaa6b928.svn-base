/*! \file cs237-mat3x3.hxx
 *
 * Support code for CMSC 23700 Winter 2014.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_MAT3X3_HXX_
#define _CS237_MAT3X3_HXX_

#ifndef _CS237_HXX_
#error "c237-mat3x3.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

	template <typename T>
	struct mat3x3 {
	    vec3<T> cv[3];	// column vectors

	  //! create an uninitialized matrix
	    mat3x3 () {}
	  //! copy constructor
	    mat3x3 (mat3x3 const &m);
	  //! construct a matrix from individual elements given in column-major order
	    explicit mat3x3 (
		T m00, T m10, T m20,	// first column
		T m01, T m11, T m21,	// second column
		T m02, T m12, T m22);	// third column
	  //! construct a matrix from column vectors
	    explicit mat3x3 (vec3<T> const &c0, vec3<T> const &c1, vec3<T> const &c2);
	  //! extract the upper-left 3x3 matrix from a 4x4 matrix
	    explicit mat3x3 (mat4x4<T> const &mm);
	  //! extract the upper-left 3x3 matrix from a 3x4 matrix
	    explicit mat3x3 (mat3x4<T> const &mm);
	  //! extract the upper-left 3x3 matrix from a 4x3 matrix
	    explicit mat3x3 (mat4x3<T> const &mm);
	  //! construct a diagonal matrix with diagonal elements initialized to x
	    explicit mat3x3 (T const &x);

	  //! return the i'th column vector
	    vec3<T> & operator[] (unsigned int const &i);
	    vec3<T> const & operator[] (unsigned int const &i) const;

	    mat3x3 & operator= (mat3x3 const &mm);

	    mat3x3 & operator+= (T const &s);
	    mat3x3 & operator+= (mat3x3 const &mm);

	    mat3x3 & operator-= (T const &s);
	    mat3x3 & operator-= (mat3x3 const &mm);

	    mat3x3 & operator*= (T const &s);
	    mat3x3 & operator*= (mat3x3 const &mm);

	    mat3x3 inverse() const;
	    mat3x3 transpose() const;
	    T determinant() const;
	};

    } /* _namespace __detail */

    typedef __detail::mat3x3<float> mat3x3f;
    typedef __detail::mat3x3<double> mat3x3d;
    typedef mat3x3f mat3f;
    typedef mat3x3d mat3d;

} /* namespace cs237 */

#endif /* !_CS237_MAT3X3_HXX_ */
