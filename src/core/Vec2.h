#pragma once

#include <cmath>

template <class T>
class Vec2
{
public:
    T x, y;

    Vec2() : x((T)0), y((T)0) {}
    Vec2(T x, T y) : x(x), y(y) {}
    Vec2(const Vec2 &v) : x(v.x), y(v.y) {}

    Vec2 &operator=(const Vec2 &v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }

    Vec2 operator+(Vec2 &v)
    {
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator-(Vec2 &v)
    {
        return Vec2(x - v.x, y - v.y);
    }

    Vec2 &operator+=(Vec2 &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec2 &operator-=(Vec2 &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vec2 operator+(double s)
    {
        return Vec2(x + s, y + s);
    }
    Vec2 operator-(double s)
    {
        return Vec2(x - s, y - s);
    }
    Vec2 operator*(double s)
    {
        return Vec2(x * s, y * s);
    }
    Vec2 operator/(double s)
    {
        return Vec2(x / s, y / s);
    }

    Vec2 &operator+=(double s)
    {
        x += s;
        y += s;
        return *this;
    }
    Vec2 &operator-=(double s)
    {
        x -= s;
        y -= s;
        return *this;
    }
    Vec2 &operator*=(double s)
    {
        x *= s;
        y *= s;
        return *this;
    }
    Vec2 &operator/=(double s)
    {
        x /= s;
        y /= s;
        return *this;
    }

    void Set(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    void Rotate(double deg)
    {
        double theta = deg / 180.0 * M_PI;
        double c = cos(theta);
        double s = sin(theta);
        double tx = x * c - y * s;
        double ty = x * s + y * c;
        x = tx;
        y = ty;
    }

    Vec2 &Normalize()
    {
        if (Length() == 0)
            return *this;
        *this *= (1.0 / Length());
        return *this;
    }

    float Dist(Vec2 v) const
    {
        Vec2 d(v.x - x, v.y - y);
        return d.length();
    }
    float Length() const
    {
        return std::sqrt(LengthSq());
    }
    float LengthSq() const
    {
        return x * x + y * y;
    }
    void Truncate(double length)
    {
        double angle = atan2f(y, x);
        x = length * cos(angle);
        y = length * sin(angle);
    }

    Vec2 Ortho() const
    {
        return Vec2(y, -x);
    }

    static float dot(Vec2 v1, Vec2 v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }
    static float cross(Vec2 v1, Vec2 v2)
    {
        return (v1.x * v2.y) - (v1.y * v2.x);
    }
};

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;