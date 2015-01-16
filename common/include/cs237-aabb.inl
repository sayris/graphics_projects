/*! \file cs237-aabb.inl
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2014 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_AABB_INL_
#define _CS237_AABB_INL_

#ifndef _CS237_HXX_
#  error "c237-aabb.hxx should not be included directly"
#endif

namespace cs237 {

    namespace __detail {

	template <typename T>
	inline bool AABB<T>::isEmpty () const
	{
	    return this->_empty;
	}

	template <typename T>
	inline bool AABB<T>::includesPt (vec3<T> const &pt) const
	{
	    return (! this->_empty)
		&& (this->_min.x <= pt.x) && (pt.x <= this->_max.x)
		&& (this->_min.y <= pt.y) && (pt.y <= this->_max.y)
		&& (this->_min.z <= pt.z) && (pt.z <= this->_max.z);
	}

	template <typename T>
	inline T AABB<T>::distanceToPt (vec3<T> const &pt) const
	{
#define DELTA(A)							\
	(pt.A < this->_min.A)						\
	    ? (this->_min.A - pt.A)					\
	    : ((this->_max.A < pt.A) ? (pt.A - this->_max.A) : T(0))

	    vec3<T> dv(DELTA(x), DELTA(y), DELTA(z));
	    T lenSq = dot(dv, dv);
	    if (lenSq == 0.0)
		return lenSq;
	    else
		return sqrt(lenSq);
#undef DELTA
	}

	template <typename T>
	inline void AABB<T>::addPt (vec3<T> const &pt)
	{
	    if (this->_empty) {
		this->_empty = false;
		this->_min = pt;
		this->_max = pt;
	    }
	    else if (pt.x < this->_min.x) this->_min.x = pt.x;
	    else if (this->_max.x < pt.x) this->_max.x = pt.x;
	    if (pt.y < this->_min.y) this->_min.y = pt.y;
	    else if (this->_max.y < pt.y) this->_max.y = pt.y;
	    if (pt.z < this->_min.z) this->_min.z = pt.z;
	    else if (this->_max.z < pt.z) this->_max.z = pt.z;
	}

	template <typename T>
	inline AABB<T> AABB<T>::merge (AABB<T> const &bb1, AABB<T> const &bb2)
	{
	    if ((! bb1._empty) && (! bb2.empty))
		return AABB<T>(
		    vec3<T>(
			(bb1.min.x <= bb2.min.x) ? bb1.min.x : bb2.min.x,
			(bb1.min.y <= bb2.min.y) ? bb1.min.y : bb2.min.y,
			(bb1.min.z <= bb2.min.z) ? bb1.min.z : bb2.min.z),
		    vec3<T>(
			(bb1.max.x <= bb2.max.x) ? bb2.max.x : bb1.max.x,
			(bb1.max.y <= bb2.max.y) ? bb2.max.y : bb1.max.y,
			(bb1.max.z <= bb2.max.z) ? bb2.max.z : bb1.max.z));
	    else if (bb1.empty)
		return bb2;
	    else
		return bb1;
	}

	  //! center of the box
	template <typename T>
	inline vec3<T> AABB<T>::center () const
	{
	    assert (! this->_empty);
	    return T(0.5) * (this->_min + this->_max);
	}

	template <typename T>
	inline vec3<T> const & AABB<T>::min() const
	{
	    assert (! this->_empty);
	    return this->_min;
	}

	template <typename T>
	inline vec3<T> const & AABB<T>::max() const
	{
	    assert (! this->_empty);
	    return this->_max;
	}

	template <typename T>
	inline T AABB<T>::minX () const
	{
	    assert (! this->_empty);
	    return this->_min.x;
	}

	template <typename T>
	inline T AABB<T>::minY () const
	{
	    assert (! this->_empty);
	    return this->_min.y;
	}

	template <typename T>
	inline T AABB<T>::minZ () const
	{
	    assert (! this->_empty);
	    return this->_min.z;
	}

	template <typename T>
	inline T AABB<T>::maxX () const
	{
	    assert (! this->_empty);
	    return this->_max.x;
	}

	template <typename T>
	inline T AABB<T>::maxY () const
	{
	    assert (! this->_empty);
	    return this->_max.y;
	}

	template <typename T>
	inline T AABB<T>::maxZ () const
	{
	    assert (! this->_empty);
	    return this->_max.z;
	}

	  //! return the coordinates of the i'th corner of the box, where
	  //! _min is corner 0 and _max is corner 7.
	template <typename T>
	inline vec3<T> AABB<T>::corner (int i) const
	{
	    assert (! this->_empty);
	    return vec3<T>(
		(i & 4) ? this->_max.x : this->_min.x,
		(i & 2) ? this->_max.y : this->_min.y,
		(i & 1) ? this->_max.z : this->_min.z);
	}

    } // namespace __detail

} //namespace cs237

#endif /* !_CS237_AABB_INL_ */
