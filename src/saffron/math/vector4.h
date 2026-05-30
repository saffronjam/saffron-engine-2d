////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2020 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered src versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 4. This notice may not be removed or altered from any src distribution.
//
////////////////////////////////////////////////////////////

#ifndef SFML_VECTOR4_HPP
#define SFML_VECTOR4_HPP


namespace sf
{
////////////////////////////////////////////////////////////
/// \brief Utility template class for manipulating
///        4-dimensional vectors
///
////////////////////////////////////////////////////////////
template <typename T>
class Vector4
{
public:

	////////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates a Vector4(0, 0, 0).
	///
	////////////////////////////////////////////////////////////
	Vector4() :
		x(0),
		y(0),
		z(0),
		w(0)
	{
	}

	////////////////////////////////////////////////////////////
	/// \brief Construct the vector from its coordinates
	///
	/// \param X X coordinate
	/// \param Y Y coordinate
	/// \param Z Z coordinate
	/// \param W W coordinate
	///
	////////////////////////////////////////////////////////////
	Vector4(T X, T Y, T Z, T W) :
		x(X),
		y(Y),
		z(Z),
		w(W)
	{
	}

	Vector4(Vector3<T> vec, T W) :
		x(vec.x),
		y(vec.y),
		z(vec.z),
		w(W)
	{
	}

	////////////////////////////////////////////////////////////
	/// \brief Construct the vector from another type of vector
	///
	/// This constructor doesn't replace the copy constructor,
	/// it's called only when U != T.
	/// A call to this constructor will fail to compile if U
	/// is not convertible to T.
	///
	/// \param vector Vector to convert
	///
	////////////////////////////////////////////////////////////
	template <typename U>
	explicit Vector4(const Vector4<U>& vector) :
		x(static_cast<T>(vector.x)),
		y(static_cast<T>(vector.y)),
		z(static_cast<T>(vector.z)),
		w(static_cast<T>(vector.w))
	{
	}

	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
	T x; //!< X coordinate of the vector
	T y; //!< Y coordinate of the vector
	T z; //!< Z coordinate of the vector
	T w; //!< W coordinate of the vector
};

// Define the most common types
typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;
} // namespace sf


#endif // SFML_VECTOR4_HPP
