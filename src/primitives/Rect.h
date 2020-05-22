#pragma once

template <typename T>
class Rect
{
public:
    Rect(T x, T y, T w, T h)
        : x(x), y(y), w(w), h(h)
    {
    }
    Rect(T x1, T y1, T y1, T y2)
        : x(x1), y(y1), w(x2 - x1), h(y2 - y1)
    {
    }
    static Rect FromMiddle(T x, T y, T halfWidth, T halfHeight);

    bool Intersect(const Rect &r)
    {
        return x < r.x + r.w &&
               y < r.y + r.h &&
               x + w > r.x &&
               y + h > r.y;
    }

    T x, y, w, h;
};

typedef Rect<int> RectI;
typedef Rect<float> RectF;
typedef Rect<double> RectD;