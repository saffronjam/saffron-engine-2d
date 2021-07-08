#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/System/Vector2.hpp>

namespace Se
{
template <typename T>
class Transform
{
public:
	Transform();
	Transform(T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22);

	explicit operator sf::Transform();

	auto Matrix() const -> const T*;
	auto Inverse() const -> Transform;

	auto TransformPoint(T x, T y) const -> sf::Vector2<T>;
	auto TransformPoint(const sf::Vector2<T>& point) const -> sf::Vector2<T>;
	auto TransformRect(const sf::Rect<T>& rectangle) const -> sf::Rect<T>;

	auto Combine(const Transform& transform) -> Transform&;

	auto Translate(T x, T y) -> Transform&;
	auto Translate(const sf::Vector2<T>& offset) -> Transform&;

	auto Rotate(T angle) -> Transform&;
	auto Rotate(T angle, T centerX, T centerY) -> Transform&;
	auto Rotate(T angle, const sf::Vector2<T>& center) -> Transform&;

	auto Scale(T scaleX, T scaleY) -> Transform&;
	auto Scale(T scaleX, T scaleY, T centerX, T centerY) -> Transform&;
	auto Scale(const sf::Vector2<T>& factors) -> Transform&;
	auto Scale(const sf::Vector2<T>& factors, const sf::Vector2<T>& center) -> Transform&;

	static const Transform Identity;

private:
	T m_matrix[16]; //!< 4x4 matrix defining the transformation
};


template <typename T>
Transform<T> operator *(const Transform<T>& left, const Transform<T>& right);

template <typename T>
Transform<T>& operator *=(Transform<T>& left, const Transform<T>& right);

template <typename T>
sf::Vector2<T> operator *(const Transform<T>& left, const sf::Vector2<T>& right);

template <typename T>
bool operator ==(const Transform<T>& left, const Transform<T>& right);

template <typename T>
bool operator !=(const Transform<T>& left, const Transform<T>& right);

template <typename T>
const Transform<T> Transform<T>::Identity;


template <typename T>
Transform<T>::Transform()
{
	// Identity matrix
	m_matrix[0] = 1.f;
	m_matrix[4] = 0.f;
	m_matrix[8] = 0.f;
	m_matrix[12] = 0.f;
	m_matrix[1] = 0.f;
	m_matrix[5] = 1.f;
	m_matrix[9] = 0.f;
	m_matrix[13] = 0.f;
	m_matrix[2] = 0.f;
	m_matrix[6] = 0.f;
	m_matrix[10] = 1.f;
	m_matrix[14] = 0.f;
	m_matrix[3] = 0.f;
	m_matrix[7] = 0.f;
	m_matrix[11] = 0.f;
	m_matrix[15] = 1.f;
}


template <typename T>
Transform<T>::Transform(T a00, T a01, T a02, T a10, T a11, T a12, T a20, T a21, T a22)
{
	m_matrix[0] = a00;
	m_matrix[4] = a01;
	m_matrix[8] = 0.f;
	m_matrix[12] = a02;
	m_matrix[1] = a10;
	m_matrix[5] = a11;
	m_matrix[9] = 0.f;
	m_matrix[13] = a12;
	m_matrix[2] = 0.f;
	m_matrix[6] = 0.f;
	m_matrix[10] = 1.f;
	m_matrix[14] = 0.f;
	m_matrix[3] = a20;
	m_matrix[7] = a21;
	m_matrix[11] = 0.f;
	m_matrix[15] = a22;
}

template <typename T>
Transform<T>::operator sf::Transform()
{
	float d[16] = {};
	for (int i = 0; i < 16; i++)
	{
		d[i] = static_cast<float>(m_matrix[i]);
	}
	sf::Transform transform;
	std::memcpy(&transform, d, sizeof(float) * 16);
	return transform;
}

template <typename T>
const T* Transform<T>::Matrix() const
{
	return m_matrix;
}


template <typename T>
auto Transform<T>::Inverse() const -> Transform<T>
{
	// Compute the determinant
	T det = m_matrix[0] * (m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) - m_matrix[1] * (m_matrix[15] *
		m_matrix[4] - m_matrix[7] * m_matrix[12]) + m_matrix[3] * (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[
		12]);

	// Compute the inverse if the determinant is not zero
	// (don't use an epsilon because the determinant may *really* be tiny)
	if (det != 0.f)
	{
		return Transform((m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) / det,
		                 -(m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12]) / det,
		                 (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12]) / det,
		                 -(m_matrix[15] * m_matrix[1] - m_matrix[3] * m_matrix[13]) / det,
		                 (m_matrix[15] * m_matrix[0] - m_matrix[3] * m_matrix[12]) / det,
		                 -(m_matrix[13] * m_matrix[0] - m_matrix[1] * m_matrix[12]) / det,
		                 (m_matrix[7] * m_matrix[1] - m_matrix[3] * m_matrix[5]) / det,
		                 -(m_matrix[7] * m_matrix[0] - m_matrix[3] * m_matrix[4]) / det,
		                 (m_matrix[5] * m_matrix[0] - m_matrix[1] * m_matrix[4]) / det);
	}
	else
	{
		return Identity;
	}
}


template <typename T>
auto Transform<T>::TransformPoint(T x, T y) const -> sf::Vector2<T>
{
	return sf::Vector2<T>(m_matrix[0] * x + m_matrix[4] * y + m_matrix[12],
	                      m_matrix[1] * x + m_matrix[5] * y + m_matrix[13]);
}


template <typename T>
auto Transform<T>::TransformPoint(const sf::Vector2<T>& point) const -> sf::Vector2<T>
{
	return TransformPoint(point.x, point.y);
}


template <typename T>
auto Transform<T>::TransformRect(const sf::Rect<T>& rectangle) const -> sf::Rect<T>
{
	// HighPrecTransform the 4 corners of the rectangle
	const sf::Vector2<T> points[] = {
		TransformPoint(rectangle.left, rectangle.top), TransformPoint(rectangle.left, rectangle.top + rectangle.height),
		TransformPoint(rectangle.left + rectangle.width, rectangle.top),
		TransformPoint(rectangle.left + rectangle.width, rectangle.top + rectangle.height)
	};

	// Compute the bounding rectangle of the transformed points
	T left = points[0].x;
	T top = points[0].y;
	T right = points[0].x;
	T bottom = points[0].y;
	for (int i = 1; i < 4; ++i)
	{
		if (points[i].x < left) left = points[i].x;
		else if (points[i].x > right) right = points[i].x;
		if (points[i].y < top) top = points[i].y;
		else if (points[i].y > bottom) bottom = points[i].y;
	}

	return sf::Rect<T>(left, top, right - left, bottom - top);
}


template <typename T>
auto Transform<T>::Combine(const Transform<T>& transform) -> Transform<T>&
{
	const T* a = m_matrix;
	const T* b = transform.m_matrix;

	*this = Transform(a[0] * b[0] + a[4] * b[1] + a[12] * b[3], a[0] * b[4] + a[4] * b[5] + a[12] * b[7],
	                  a[0] * b[12] + a[4] * b[13] + a[12] * b[15], a[1] * b[0] + a[5] * b[1] + a[13] * b[3],
	                  a[1] * b[4] + a[5] * b[5] + a[13] * b[7], a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
	                  a[3] * b[0] + a[7] * b[1] + a[15] * b[3], a[3] * b[4] + a[7] * b[5] + a[15] * b[7],
	                  a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);

	return *this;
}


template <typename T>
auto Transform<T>::Translate(T x, T y) -> Transform<T>&
{
	Transform translation(1, 0, x, 0, 1, y, 0, 0, 1);

	return Combine(translation);
}


template <typename T>
auto Transform<T>::Translate(const sf::Vector2<T>& offset) -> Transform<T>&
{
	return Translate(offset.x, offset.y);
}


template <typename T>
auto Transform<T>::Rotate(T angle) -> Transform<T>&
{
	T rad = angle * 3.141592654f / 180.f;
	T cos = std::cos(rad);
	T sin = std::sin(rad);

	Transform rotation(cos, -sin, 0, sin, cos, 0, 0, 0, 1);

	return Combine(rotation);
}


template <typename T>
auto Transform<T>::Rotate(T angle, T centerX, T centerY) -> Transform<T>&
{
	T rad = angle * 3.141592654f / 180.f;
	T cos = std::cos(rad);
	T sin = std::sin(rad);

	Transform rotation(cos, -sin, centerX * (1 - cos) + centerY * sin, sin, cos, centerY * (1 - cos) - centerX * sin, 0,
	                   0, 1);

	return Combine(rotation);
}


template <typename T>
auto Transform<T>::Rotate(T angle, const sf::Vector2<T>& center) -> Transform<T>&
{
	return Rotate(angle, center.x, center.y);
}


template <typename T>
auto Transform<T>::Scale(T scaleX, T scaleY) -> Transform<T>&
{
	Transform scaling(scaleX, 0, 0, 0, scaleY, 0, 0, 0, 1);

	return Combine(scaling);
}


template <typename T>
auto Transform<T>::Scale(T scaleX, T scaleY, T centerX, T centerY) -> Transform<T>&
{
	Transform scaling(scaleX, 0, centerX * (1 - scaleX), 0, scaleY, centerY * (1 - scaleY), 0, 0, 1);

	return Combine(scaling);
}


template <typename T>
auto Transform<T>::Scale(const sf::Vector2<T>& factors) -> Transform<T>&
{
	return Scale(factors.x, factors.y);
}


template <typename T>
auto Transform<T>::Scale(const sf::Vector2<T>& factors, const sf::Vector2<T>& center) -> Transform<T>&
{
	return Scale(factors.x, factors.y, center.x, center.y);
}


template <typename T>
auto operator *(const Transform<T>& left, const Transform<T>& right) -> Transform<T>
{
	return Transform(left).Combine(right);
}


template <typename T>
auto operator *=(Transform<T>& left, const Transform<T>& right) -> Transform<T>&
{
	return left.Combine(right);
}


template <typename T>
auto operator *(const Transform<T>& left, const sf::Vector2<T>& right) -> sf::Vector2<T>
{
	return left.TransformPoint(right);
}


template <typename T>
auto operator ==(const Transform<T>& left, const Transform<T>& right) -> bool
{
	const T* a = left.Matrix();
	const T* b = right.Matrix();

	return ((a[0] == b[0]) && (a[1] == b[1]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[7] == b[7]) &&
		(a[12] == b[12]) && (a[13] == b[13]) && (a[15] == b[15]));
}


template <typename T>
auto operator !=(const Transform<T>& left, const Transform<T>& right) -> bool
{
	return !(left == right);
}
}
