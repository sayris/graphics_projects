/*! \file cs237-aabb.hxx
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_AABB_HXX_
#define _CS237_AABB_HXX_

#ifndef _CS237_HXX_
#  error "c237-aabb.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

      //! Axis-Aligned Bounding Box
	template <typename T>
	struct AABB {
	    vec3<T> _min, _max;
	    bool _empty;

	    AABB () : _min(), _max(), _empty(true) {}
	    AABB (vec3<T> const &pt) : _min(pt), _max(pt), _empty(false) {}
	    AABB (vec3<T> const &min, vec3<T> const &max) : _min(min), _max(max), _empty(false) {}

	  //! is the box empty
	    bool isEmpty () const;

	  //! is a point inside the box?
	    bool includesPt (vec3<T> const &pt) const;

	  //! distance to a point; will be 0.0 if the point is inside the box
	    T distanceToPt (vec3<T> const &pt) const;

	  //! extend the box as necessary to include the point
	    void addPt (vec3<T> const &pt);

	  //! return the union of two AABBs
	    AABB merge (AABB const & bb1, AABB const & bb2);

	  /***** Warning: the following functions are undefined on empty boxes! *****/

	  //! minimum extents of the box
	    vec3<T> const & min() const;

	  //! maximum extents of the box
	    vec3<T> const & max() const;

	  //! center of the box
	    vec3<T> center () const;

	  //! minimum X-coordinate of the box
	    T minX () const;
	  //! minimum Y-coordinate of the box
	    T minY () const;
	  //! minimum Z-coordinate of the box
	    T minZ () const;
	  //! maximum X-coordinate of the box
	    T maxX () const;
	  //! maximum Y-coordinate of the box
	    T maxY () const;
	  //! maximum Z-coordinate of the box
	    T maxZ () const;

	  //! return the coordinates of the i'th corner of the box, where
	  //! _min is corner 0 and _max is corner 7.
	    vec3<T> corner (int i) const;
	};

      //! create an isotropic (uniform) scaling matrix
	template <typename T>
	std::ostream& operator<< (std::ostream& s, __detail::AABB<T> const &bb);

    } // namespace __detail

    typedef __detail::AABB<float> AABBf;
    typedef __detail::AABB<double> AABBd;

} //namespace cs237

#endif /* !_CS237_AABB_HXX_ */
