#pragma once

#include <vector>
#include <array>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

class Funclib
{
public:
    static std::vector<sf::Vector2f> WrapPoints(std::vector<sf::Vector2f> *points);

private:
    static void ClearPointsRecursively(std::pair<sf::Vector2f, sf::Vector2f> line, std::vector<sf::Vector2f *> *points, std::vector<sf::Vector2f> *finalPoints);
};

class vf
{
public:
    static sf::Vector2f GetLineMiddlePoint(sf::Vector2f point1, sf::Vector2f point2);
    static sf::Vector2f GetDirectionVector(sf::Vector2f point1, sf::Vector2f point2);
    static sf::Vector2f GetUnitVector(sf::Vector2f vector);
    static sf::Vector2f NullVector();
    static sf::Vector2f GetPerpendicularVector(sf::Vector2f vector);
    static sf::Vector2f InDefIntersection(std::pair<sf::Vector2f, sf::Vector2f> line1, std::pair<sf::Vector2f, sf::Vector2f> line2);
    static float GetVectorLength(sf::Vector2f vector);
    static float GetVectorLengthSq(sf::Vector2f vector);
    static void NormalizeVector(sf::Vector2f &vector);
    static float GetSlope(sf::Vector2f point1, sf::Vector2f point2);
    static bool isLeft(sf::Vector2f line_point1, sf::Vector2f line_point2, sf::Vector2f point);
    static float GetDistanceFromLine(sf::Vector2f line_point1, sf::Vector2f line_point2, sf::Vector2f point);
};

class gf
{
public:
    static bool IsInBetween(float value, float lower_bound, float upper_bound);
};

class sfmlext
{
public:
    static bool UniqueInConvexShape(sf::ConvexShape &convexShape, sf::Vector2f &point);
    static sf::ConvexShape CreateConvexShapeFromPointList(std::vector<sf::Vector2f> unsorted_list);
    static sf::Vector2f GetCentroidOfPolygon(sf::ConvexShape polygon);
    static sf::Rect<float> RectFromCenter(sf::Vector2f mid, float half_width, float half_height);
    static sf::Vector2f ClosestPolygonVertex(sf::ConvexShape polygon, sf::Vector2f point);
};