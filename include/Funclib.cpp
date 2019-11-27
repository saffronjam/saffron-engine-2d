#include "Funclib.hpp"

#include <cmath>

std::vector<sf::Vector2f> Funclib::WrapPoints(std::vector<sf::Vector2f> *points)
{
    std::vector<sf::Vector2f> finalPoints;

    std::vector<sf::Vector2f *> topPoints;
    std::vector<sf::Vector2f *> bottomPoints;
    std::pair<sf::Vector2f, sf::Vector2f> startLine;

    sf::Vector2f biggestX = {-10000, -10000};
    sf::Vector2f smallestX = {10000, 10000};
    for (auto &point : *points)
    {
        if (point.x > biggestX.x)
        {
            biggestX = point;
        }
        if (point.x < smallestX.x)
        {
            smallestX = point;
        }
    }
    startLine = {smallestX, biggestX};

    //Inital removal of center-points
    for (auto &point : *points)
    {
        if (!vf::isLeft(startLine.first, startLine.second, point))
        {
            topPoints.emplace_back(&point);
        }
        else
        {
            bottomPoints.emplace_back(&point);
        }
    }

    ClearPointsRecursively(startLine, &topPoints, &finalPoints);
    ClearPointsRecursively({startLine.second, startLine.first}, &bottomPoints, &finalPoints);

    return finalPoints;
}

void Funclib::ClearPointsRecursively(std::pair<sf::Vector2f, sf::Vector2f> line, std::vector<sf::Vector2f *> *points, std::vector<sf::Vector2f> *finalPoints)
{
    //Find the point which is the furthest away
    float biggestDistance = 0.0f;
    int biggestIndex = -1;
    for (size_t i = 0; i < points->size(); i++)
    {
        float currentCheck = vf::GetDistanceFromLine(line.first, line.second, *(*points)[i]);
        if (currentCheck > biggestDistance)
        {
            biggestDistance = currentCheck;
            biggestIndex = i;
        }
    }

    //Continues only if it can find a new point
    if (biggestIndex != -1)
    {
        sf::Vector2f furthest = *(*points)[biggestIndex];
        std::pair<sf::Vector2f, sf::Vector2f> newLine = {line.first, furthest};
        line.first = furthest;

        std::vector<sf::Vector2f *> consideredPoints1;
        std::vector<sf::Vector2f *> consideredPoints2;
        for (auto &point : *points)
        {
            if (!vf::isLeft(newLine.first, newLine.second, *point))
            {
                consideredPoints1.emplace_back(point);
            }
            else if (!vf::isLeft(line.first, line.second, *point))
            {
                consideredPoints2.emplace_back(point);
            }
        }

        ClearPointsRecursively(newLine, &consideredPoints1, finalPoints);
        ClearPointsRecursively(line, &consideredPoints2, finalPoints);
    }
    else
    {
        finalPoints->emplace_back(line.first.x, line.first.y);
        finalPoints->emplace_back(line.second.x, line.second.y);
    }
}

//Vector functions
sf::Vector2f vf::GetLineMiddlePoint(sf::Vector2f point1, sf::Vector2f point2)
{
    sf::Vector2f delta = point2 - point1;
    delta.x /= 2.0f;
    delta.y /= 2.0f;
    return point1 + delta;
}

sf::Vector2f vf::GetDirectionVector(sf::Vector2f point1, sf::Vector2f point2)
{
    return vf::GetUnitVector(point2 - point1);
}

sf::Vector2f vf::GetUnitVector(sf::Vector2f vector)
{
    return vector / vf::GetVectorLength(vector);
}

sf::Vector2f vf::NullVector()
{
    return sf::Vector2f(0, 0);
}

sf::Vector2f vf::GetPerpendicularVector(sf::Vector2f vector)
{
    return sf::Vector2f(-vector.y, vector.x);
}

sf::Vector2f vf::InDefIntersection(std::pair<sf::Vector2f, sf::Vector2f> line1, std::pair<sf::Vector2f, sf::Vector2f> line2)
{
    float line1_k = GetSlope(line1.first, line1.second);
    float line2_k = GetSlope(line2.first, line2.second);
    if (line1_k == line2_k)
    {
        //Parallell lines have none or unlimited intersections
        return sf::Vector2f(-1, -1);
    }

    float line1_m = line1.first.y - line1_k * line1.first.x;
    float line2_m = line2.first.y - line2_k * line2.first.x;

    sf::Vector2f intersection;
    intersection.x = (line2_m - line1_m) / (line1_k - line2_k);
    intersection.y = line1_k * intersection.x;

    return intersection;
}

float vf::GetVectorLength(sf::Vector2f vector)
{
    return sqrt(vf::GetVectorLengthSq(vector));
}

float vf::GetVectorLengthSq(sf::Vector2f vector)
{
    return vector.x * vector.x + vector.y * vector.y;
}

void vf::NormalizeVector(sf::Vector2f &vector)
{
    float length = vf::GetVectorLength(vector);
    if (length != 0.0f)
    {
        vector.x /= length;
        vector.y /= length;
    }
}

float vf::GetSlope(sf::Vector2f point1, sf::Vector2f point2)
{
    if (point1.x > point2.x)
    {
        std::swap(point1, point2);
    }
    return (point2.y - point1.y) / (point2.x - point1.x);
}

bool vf::isLeft(sf::Vector2f line_point1, sf::Vector2f line_point2, sf::Vector2f point)
{
    return ((line_point2.x - line_point1.x) * (point.y - line_point1.y) - (line_point2.y - line_point1.y) * (point.x - line_point1.x)) > 0.0f;
}

float vf::GetDistanceFromLine(sf::Vector2f line_point1, sf::Vector2f line_point2, sf::Vector2f point)
{
    return abs(((line_point2.x - line_point1.x) * (point.y - line_point1.y) - (line_point2.y - line_point1.y) * (point.x - line_point1.x)) / vf::GetVectorLength(line_point2 - line_point1));
}

bool gf::IsInBetween(float value, float lower_bound, float upper_bound)
{
    if (lower_bound > upper_bound)
    {
        std::swap(lower_bound, upper_bound);
    }
    return value >= lower_bound && value <= upper_bound;
}

bool sfmlext::UniqueInConvexShape(sf::ConvexShape &convexShape, sf::Vector2f &point)
{
    bool unique = true;
    for (size_t i = 0; i < convexShape.getPointCount(); i++)
    {
        if ((int)convexShape.getPoint(i).x == (int)point.x && (int)convexShape.getPoint(i).y == (int)point.y)
        {
            unique = false;
        }
    }
    return unique;
}

sf::ConvexShape sfmlext::CreateConvexShapeFromPointList(std::vector<sf::Vector2f> points)
{
    std::vector<sf::Vector2f> usable_points = Funclib::WrapPoints(&points);

    sf::ConvexShape finalShape;
    finalShape.setPointCount(usable_points.size() / 2);

    for (size_t i = 0, currentPoint = 0; i < usable_points.size() - 1; i += 2, currentPoint++)
    {
        finalShape.setPoint(currentPoint, usable_points[i]);
    }

    return finalShape;
}

sf::Vector2f sfmlext::GetCentroidOfPolygon(sf::ConvexShape polygon)
{
    sf::Vector2f sum_of_verticies(0.0f, 0.0f);
    unsigned int number_of_verticies = polygon.getPointCount();

    for (unsigned int i = 0; i < number_of_verticies; i++)
    {
        sum_of_verticies += polygon.getPoint(i);
    }

    sum_of_verticies.x /= number_of_verticies;
    sum_of_verticies.y /= number_of_verticies;

    return sum_of_verticies;
}

sf::Rect<float> sfmlext::RectFromCenter(sf::Vector2f mid, float half_width, float half_height)
{
    sf::Rect<float> final;
    final.left = mid.x - half_width;
    final.top = mid.y - half_height;
    final.width = half_width * 2.0f;
    final.height = half_height * 2.0f;
    return final;
}

sf::Vector2f sfmlext::ClosestPolygonVertex(sf::ConvexShape polygon, sf::Vector2f point)
{
    float smallest_dist_sq = -1;
    sf::Vector2f final(-1.0f, -1.0f);

    for (size_t i = 0; i < polygon.getPointCount(); i++)
    {
        float dist_sq = vf::GetVectorLengthSq(sf::Vector2f(point - polygon.getPoint(i)));

        if (smallest_dist_sq == -1 || dist_sq < smallest_dist_sq)
        {
            smallest_dist_sq = dist_sq;
            final = polygon.getPoint(i);
        }
    }
    return final;
}