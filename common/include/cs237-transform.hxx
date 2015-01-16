/*! \file cs237-transform.hxx
 *
 * Support code for CMSC 23700 Winter 2014.
 *
 * Standard OpenGL transformation matrices.
 *
 * \author John Reppy
 */

/*
 * COPYRIGHT (c) 2013 John Reppy (http://cs.uchicago.edu/~jhr)
 * All rights reserved.
 */

#ifndef _CS237_TRANSFORM_HXX_
#define _CS237_TRANSFORM_HXX_

#ifndef _CS237_HXX_
#error "cs237-transform.hxx should not be included directly"
#endif

namespace cs237 {

  //! create an isotropic (uniform) scaling matrix
    template <typename T>
    __detail::mat4x4<T> scale (T s);

  //! apply isotropic (uniform) scaling to a matrix
  //! \param m the matrix to transform
    template <typename T>
    __detail::mat4x4<T> scale (__detail::mat4x4<T> const &m, T s);

  //! create an anisotropic (non-uniform) scaling matrix
    template <typename T>
    __detail::mat4x4<T> scale (__detail::vec3<T> const &s);

  //! apply anisotropic (non-uniform) scaling to a matrix
  //! \param m the matrix to transform
    template <typename T>
    __detail::mat4x4<T> scale (__detail::mat4x4<T> const &m, __detail::vec3<T> const &s);

  //! create a rotation about the X axis matrix
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateX (T theta);

  //! apply rotation about the X axis matrix to a matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateX (__detail::mat4x4<T> const &m, T theta);

  //! create a rotation about the Y axis matrix
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateY (T theta);

  //! apply rotation about the X axis matrix to a matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateY (__detail::mat4x4<T> const &m, T theta);

  //! create a rotation about the Z axis matrix
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateZ (T theta);

  //! apply rotation about the X axis matrix to a matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
    template <typename T>
    __detail::mat4x4<T> rotateZ (__detail::mat4x4<T> const &m, T theta);

  //! create a rotation about an arbitrary axis matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
  //! \param axis the axis of rotation
    template <typename T>
    __detail::mat4x4<T> rotate (T angle, __detail::vec3<T> const &axis);

  //! apply a rotation about an arbitrary axis to a matrix
  //! \param m the matrix to transform
  //! \param theta the angle of rotation in degrees
  //! \param axis the axis of rotation
    template <typename T>
    __detail::mat4x4<T> rotate (__detail::mat4x4<T> const &m, T angle, __detail::vec3<T> const &axis);

  // translation
    template <typename T>
    __detail::mat4x4<T> translate (__detail::vec3<T> const &offset);
    template <typename T>
    __detail::mat4x4<T> translate (__detail::mat4x4<T> const &m, __detail::vec3<T> const &offset);

  //! \brief build a a viewing matrix derived from an eye point, a reference point indicating the
  //!        center of the scene, and an up vector.
  //! \param eye[in] specifies the position of the eye point
  //! \param center[in] specifies the point that is being looked at
  //! \param up[in] specifies the up direction of the viewer
    template <typename T>
    __detail::mat4x4<T> lookAt (
	__detail::vec3<T> const &eye,
	__detail::vec3<T> const &center,
	__detail::vec3<T> const &up);

  //! set up a transformation matrix that produces a parallel projection
    template <typename T>
    __detail::mat4x4<T> ortho (
	T const &left, T const &right,
	T const &bottom, T const &top,
	T const &nearVal, T const &farVal);

  //! set up a transformation matrix that produces two-dimensional orthographic viewing region
    template <typename T>
    __detail::mat4x4<T> ortho2D (T const &left, T const &right, T const &bottom, T const &top);

  //! \brief specifies a perspective projection matrix
  //! \param fov[in] the vertical (Y direction).
  //! \param aspect[in] the aspect ratio (width/height)
  //! \param zNear[in] the distance from the viewer to the near clipping plane
  //! \param zFar[in] the distance from the viewer to the far clipping plane
    template <typename T>
    __detail::mat4x4<T> perspective (T const &fov, T const &aspect, T const &zNear, T const &zFar);

  //! specifies a viewing frustum into the world coordinate system
    template <typename T>
    __detail::mat4x4<T> frustum (T const &left, T const &right, T const &bottom, T const &top, T const &nearVal, T const &farVal);

} /* namespace cs237 */


#endif /* !_CS237_TRANSFORM_HXX_ */
