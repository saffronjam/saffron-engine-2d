#pragma once

#include <utility>
#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Color.hpp>

namespace Se
{
class VecUtils
{
public:
	template <typename T>
	static auto LineMiddlePoint(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2) -> sf::Vector2<T>;

	template <typename T>
	static auto Direction(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2) -> sf::Vector2<T>;

	template <typename T>
	static auto Unit(const sf::Vector2<T>& vector) -> sf::Vector2<T>;

	template <typename T = float>
	static auto Null() -> sf::Vector2<T>;

	template <typename T>
	static auto Perpendicular(const sf::Vector2<T>& vector) -> sf::Vector2<T>;

	template <typename T>
	static auto Rotate(const sf::Vector2<T>& vector, float angle, const sf::Vector2<T>& around) -> sf::Vector2<T>;

	template <typename T>
	static auto Rotate(const sf::Vector2<T>& vector, const sf::Vector2f& direction,
	                   const sf::Vector2<T>& around) -> sf::Vector2<T>;

	template <typename T>
	static auto Length(const sf::Vector2<T>& vector) -> float;

	template <typename T>
	static auto LengthSq(const sf::Vector2<T>& vector) -> float;

	template <typename T>
	static auto Distance(const sf::Vector2<T>& u, const sf::Vector2<T>& v) -> float;

	template <typename T>
	static void Normalize(sf::Vector2<T>& vector);

	template <typename T>
	static auto SetLength(const sf::Vector2<T>& vector, float length) -> sf::Vector2<T>;

	template <typename T>
	static auto Angle(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) -> float;

	template <typename T>
	static auto Slope(sf::Vector2<T> point1, sf::Vector2<T> point2) -> float;

	template <typename T>
	static auto DistanceFromLine(sf::Vector2<T> linePoint1, sf::Vector2<T> linePoint2, sf::Vector2<T> point) -> float;

	template <typename T>
	static auto Dot(const sf::Vector2<T>& u, const sf::Vector2<T>& v) -> T;

	template <typename T>
	static auto MapRange(const sf::Vector2<T>& v, T a1, T b1, T a2, T b2) -> sf::Vector2<T>;

	template <typename T>
	static auto MapRange(const sf::Vector3<T>& v, T a1, T b1, T a2, T b2) -> sf::Vector3<T>;

	template <typename T = float>
	static auto ByColor(const sf::Color& color) -> sf::Vector3<T>;

	template <typename T, typename U>
	static auto ConvertTo(const U& in) -> T;

	template <typename T>
	static auto IsLeft(const sf::Vector2<T>& a, const sf::Vector2<T>& b, const sf::Vector2<T>& point) -> bool;

	template <typename T>
	static auto Constrain(const sf::Vector2<T>& value, T from, T to) -> sf::Vector2<T>;
};

template <typename T>
auto VecUtils::LineMiddlePoint(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2) -> sf::Vector2<T>
{
	sf::Vector2<T> delta = point2 - point1;
	delta.x /= 2.0f;
	delta.y /= 2.0f;
	return point1 + delta;
}

template <typename T>
auto VecUtils::Direction(const sf::Vector2<T>& point1, const sf::Vector2<T>& point2) -> sf::Vector2<T>
{
	return VecUtils::Unit(point2 - point1);
}

template <typename T>
auto VecUtils::Unit(const sf::Vector2<T>& vector) -> sf::Vector2<T>
{
	if (vector != VecUtils::Null<T>()) return vector / VecUtils::Length(vector);
	return VecUtils::Null<T>();
}

template <typename T>
auto VecUtils::Null() -> sf::Vector2<T>
{
	return sf::Vector2<T>(0, 0);
}

template <typename T>
auto VecUtils::Perpendicular(const sf::Vector2<T>& vector) -> sf::Vector2<T>
{
	return sf::Vector2<T>(-vector.y, vector.x);
}

template <typename T>
auto VecUtils::Rotate(const sf::Vector2<T>& vector, float angle, const sf::Vector2<T>& around) -> sf::Vector2<T>
{
	auto anchor = vector - around;

	const float cosTheta = cos(angle);
	const float sinTheta = sin(angle);
	const float new_x = anchor.x * cosTheta - anchor.y * sinTheta;
	anchor.y = anchor.x * sinTheta + anchor.y * cosTheta;
	anchor.x = new_x;

	return anchor + around;
}

template <typename T>
auto VecUtils::Rotate(const sf::Vector2<T>& vector, const sf::Vector2f& direction,
                      const sf::Vector2<T>& around) -> sf::Vector2<T>
{
	const sf::Vector2f right(1.0f, 0.0f);

	float angle = Angle(right, direction);
	if (direction.y > 0.0f)
	{
		angle = 360.0f - angle;
	}
	return VecUtils::Rotate(vector, angle, around);
}

template <typename T>
auto VecUtils::Length(const sf::Vector2<T>& vector) -> float
{
	return sqrt(VecUtils::LengthSq(vector));
}

template <typename T>
auto VecUtils::LengthSq(const sf::Vector2<T>& vector) -> float
{
	return vector.x * vector.x + vector.y * vector.y;
}

template <typename T>
auto VecUtils::Distance(const sf::Vector2<T>& u, const sf::Vector2<T>& v) -> float
{
	return sqrt(pow(u.x - v.x, 2) + pow(u.y - v.y, 2));
}

template <typename T>
void VecUtils::Normalize(sf::Vector2<T>& vector)
{
	float length = VecUtils::Length(vector);
	if (length != 0.0f)
	{
		vector.x /= length;
		vector.y /= length;
	}
}

template <typename T>
auto VecUtils::SetLength(const sf::Vector2<T>& vector, float length) -> sf::Vector2<T>
{
	return VecUtils::Unit(vector) * length;
}

template <typename T>
auto VecUtils::Angle(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2) -> float
{
	return std::acos(VecUtils::Dot(v1, v2) / (VecUtils::Length(v1) * VecUtils::Length(v2)));
}

template <typename T>
auto VecUtils::Slope(sf::Vector2<T> point1, sf::Vector2<T> point2) -> float
{
	if (point1.x > point2.x)
	{
		std::swap(point1, point2);
	}
	return (point2.y - point1.y) / (point2.x - point1.x);
}

template <typename T>
auto VecUtils::DistanceFromLine(sf::Vector2<T> linePoint1, sf::Vector2<T> linePoint2, sf::Vector2<T> point) -> float
{
	return abs(
		((linePoint2.x - linePoint1.x) * (point.y - linePoint1.y) - (linePoint2.y - linePoint1.y) * (point.x -
			linePoint1.x)) / VecUtils::Length(linePoint2 - linePoint1));
}

template <typename T>
auto VecUtils::Dot(const sf::Vector2<T>& u, const sf::Vector2<T>& v) -> T
{
	return u.x * v.x + u.y * v.y;
}

template <typename T>
auto VecUtils::MapRange(const sf::Vector2<T>& v, T a1, T b1, T a2, T b2) -> sf::Vector2<T>
{
	T x = (v.x - a1) * (b2 - a2) / (b1 - a1) + a2;
	T y = (v.y - a1) * (b2 - a2) / (b1 - a1) + a2;
	return sf::Vector2<T>(x, y);
}

template <typename T>
auto VecUtils::MapRange(const sf::Vector3<T>& v, T a1, T b1, T a2, T b2) -> sf::Vector3<T>
{
	T x = (v.x - a1) * (b2 - a2) / (b1 - a1) + a2;
	T y = (v.y - a1) * (b2 - a2) / (b1 - a1) + a2;
	T z = (v.z - a1) * (b2 - a2) / (b1 - a1) + a2;
	return sf::Vector3<T>(x, y, z);
}

template <typename T>
auto VecUtils::ByColor(const sf::Color& color) -> sf::Vector3<T>
{
	return sf::Vector3<T>(static_cast<T>(color.r), static_cast<T>(color.g), static_cast<T>(color.b));
}

template <typename OutputVector, typename InputVector>
auto VecUtils::ConvertTo(const InputVector& in) -> OutputVector
{
	return OutputVector(in.x, in.y);
}

template <typename T>
auto VecUtils::IsLeft(const sf::Vector2<T>& a, const sf::Vector2<T>& b, const sf::Vector2<T>& point) -> bool
{
	return (b.x - a.x) * (point.y - a.y) - (b.y - a.y) * (point.x - a.x) < 0.0f;
}

template <typename T>
auto VecUtils::Constrain(const sf::Vector2<T>& vector, T from, T to) -> sf::Vector2<T>
{
	if (VecUtils::Length(vector) < from)
	{
		return VecUtils::Unit(vector) * from;
	}
	if (VecUtils::Length(vector) > to)
	{
		return VecUtils::Unit(vector) * to;
	}
	return vector;
}
}
