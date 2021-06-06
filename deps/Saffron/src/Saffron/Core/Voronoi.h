#pragma once

#include <optional>
#include <vector>
#include <set>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <jcv/jc_voronoi.h>

#include <Saffron.h>

namespace Se
{
class Voronoi : public sf::Drawable
{
private:
	friend class Polygon;

public:
	class Polygon
	{
	public:
		Polygon(Voronoi& parent, int lineVAIndex, int filledVAIndex, const sf::Vector2f& voronoiPoint,
		        List<sf::Vector2f> points);

		auto operator==(const Polygon& other) const -> bool { return _uuid == other._uuid; }

		auto GetLineVAIndex() const -> int { return _lineVAIndex; }

		auto GetFilledVAIndex() const -> int { return _filledVAIndex; }

		auto GetPoints() const -> const List<sf::Vector2f>& { return _points; }

		auto GetFillColor() const -> sf::Color { return _fillColor; }

		void SetFillColor(sf::Color color) const;
		void ClearFillColor(sf::Color color);

		auto GetNeighbors() const -> const Set<Polygon*>& { return _neighbors; }

		void AddNeighbor(Polygon* neighbor) { _neighbors.emplace(neighbor); }

		auto GetVoronoiPoint() const -> const sf::Vector2f& { return _voronoiPoint; }

		void SetVoronoiPoint(const sf::Vector2f& voronoiPoint) { _voronoiPoint = voronoiPoint; }

		auto GetClosestEdge(const sf::Vector2f& position) const -> Pair<sf::Vector2f, sf::Vector2f>;

	private:
		UUID _uuid;

		Voronoi& _parent;
		int _lineVAIndex;
		int _filledVAIndex;

		sf::Color _fillColor = sf::Color::Transparent;
		List<sf::Vector2f> _points;
		std::set<Polygon*> _neighbors;
		sf::Vector2f _voronoiPoint;
	};

public:
	Voronoi();
	explicit Voronoi(const sf::FloatRect& boundingBox);
	Voronoi(const sf::FloatRect& boundingBox, List<sf::Vector2f> points);
	Voronoi(const sf::FloatRect& boundingBox, int noRandomPoints);
	~Voronoi() override;

	void SetPoints(List<sf::Vector2f> points);
	void SetPoints(int noRandomPoints);
	void SetBoundingBox(const sf::FloatRect& boundingBox);
	void SetOutlineColor(const sf::Color& color);
	void SetFillColor(const Polygon& polygon, sf::Color color);

	void ShowGrid() { _shouldDrawGrid = true; }

	void HideGrid() { _shouldDrawGrid = false; }

	void ShowFilled() { _shouldDrawFilledPolygons = true; }

	void HideFilled() { _shouldDrawFilledPolygons = false; }

	void Relax(int iterations = 1);

	auto GetPolygons() const -> const List<Polygon>& { return _polygons; }

	auto GetPolygon(const sf::Vector2f& position) -> Polygon&;

	// Manual controls
	void EnableAutomaticGeneration() { _automaticGeneration = true; }

	void DisableAutomaticGeneration() { _automaticGeneration = false; }

	void ForceGenerate();

private:
	static auto ConvertBoundingBox(const sf::FloatRect& boundingBox) -> jcv_rect;

	void Generate();
	void GeneratePoints(int noPoints);
	void MarkForGeneration();

	void SetupUpdateCallback();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	Run::Handle _updateHandle;
	bool _automaticGeneration = true;

	sf::Color _defaultGridColor = sf::Color::Blue;
	Optional<jcv_diagram> _diagram;
	sf::FloatRect _boundingBox;
	List<sf::Vector2f> _points;

	List<Polygon> _polygons;
	sf::VertexArray _polygonsVA{sf::PrimitiveType::Lines};
	sf::VertexArray _filledPolygonsVA{sf::PrimitiveType::Triangles};
	bool _wantNewPolygonVA = false;

	bool _shouldDrawFilledPolygons = true;
	bool _shouldDrawGrid = true;
};
}
