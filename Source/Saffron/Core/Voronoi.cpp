#include "SaffronPCH.h"

#define JC_VORONOI_IMPLEMENTATION

#include "Saffron/Core/Voronoi.h"

namespace Se
{
Voronoi::Polygon::Polygon(Voronoi& parent, int lineVAIndex, int filledVAIndex, const sf::Vector2f& voronoiPoint,
                          List<sf::Vector2f> points) :
	_parent(parent),
	_lineVAIndex(lineVAIndex),
	_filledVAIndex(filledVAIndex),
	_points(Move(points)),
	_voronoiPoint(voronoiPoint)
{
}

auto Voronoi::Polygon::operator==(const Polygon& other) const -> bool { return _uuid == other._uuid; }

auto Voronoi::Polygon::LineVAIndex() const -> int { return _lineVAIndex; }

auto Voronoi::Polygon::FilledVAIndex() const -> int { return _filledVAIndex; }

auto Voronoi::Polygon::Points() const -> const List<sf::Vector2f>& { return _points; }

auto Voronoi::Polygon::FillColor() const -> sf::Color { return _fillColor; }

auto Voronoi::Polygon::Neighbors() const -> const TreeSet<Polygon*>& { return _neighbors; }

void Voronoi::Polygon::AddNeighbor(Polygon* neighbor)
{
	_neighbors.emplace(neighbor);
}

auto Voronoi::Polygon::VoronoiPoint() const -> const sf::Vector2f& { return _voronoiPoint; }

void Voronoi::Polygon::SetVoronoiPoint(const sf::Vector2f& voronoiPoint)
{
	_voronoiPoint = voronoiPoint;
}

void Voronoi::ShowGrid()
{
	_shouldDrawGrid = true;
}

void Voronoi::HideGrid()
{
	_shouldDrawGrid = false;
}

void Voronoi::ShowFilled()
{
	_shouldDrawFilledPolygons = true;
}

void Voronoi::HideFilled()
{
	_shouldDrawFilledPolygons = false;
}

auto Voronoi::Polygons() const -> const List<Polygon>& { return _polygons; }

void Voronoi::EnableAutomaticGeneration()
{
	_automaticGeneration = true;
}

void Voronoi::DisableAutomaticGeneration()
{
	_automaticGeneration = false;
}

void Voronoi::Polygon::SetFillColor(sf::Color color) const
{
	_parent.SetFillColor(*this, color);
}

void Voronoi::Polygon::ClearFillColor(sf::Color color) const
{
	SetFillColor(sf::Color::Transparent);
}

auto Voronoi::Polygon::ClosestEdge(const sf::Vector2f& position) const -> Pair<sf::Vector2f, sf::Vector2f>
{
	constexpr float inf = std::numeric_limits<float>::infinity();
	Pair<float, float> distCache{inf, inf};
	Pair<int, int> closestIndices{-1, -1};

	for (int i = 0; i < _points.size(); i++)
	{
		const float firstCandidate = VecUtils::LengthSq(position - _points[i]);
		if (closestIndices.first == -1 || firstCandidate < distCache.first)
		{
			closestIndices.first = i;
			distCache.first = firstCandidate;
		}
	}

	for (int i = 0; i < _points.size(); i++)
	{
		if (i != closestIndices.first)
		{
			const float secondCandidate = VecUtils::LengthSq(position - _points[i]);
			if (closestIndices.second == -1 || secondCandidate < distCache.second)
			{
				closestIndices.second = i;
				distCache.second = secondCandidate;
			}
		}
	}

	return {_points[closestIndices.first], _points[closestIndices.second]};
}

Voronoi::Voronoi() :
	_boundingBox(0.0f, 0.0f, 0.0f, 0.0f)
{
	SetupUpdateCallback();
}

Voronoi::Voronoi(const sf::FloatRect& boundingBox) :
	_boundingBox(boundingBox)
{
	SetupUpdateCallback();
}

Voronoi::Voronoi(const sf::FloatRect& boundingBox, List<sf::Vector2f> points) :
	_boundingBox(boundingBox),
	_points(Move(points))
{
	SetupUpdateCallback();
	ForceGenerate();
}

Voronoi::Voronoi(const sf::FloatRect& boundingBox, int noRandomPoints) :
	_boundingBox(boundingBox)
{
	SetPoints(noRandomPoints);
	SetupUpdateCallback();
	ForceGenerate();
}

Voronoi::~Voronoi()
{
	if (_diagram.has_value())
	{
		jcv_diagram_free(&_diagram.value());
	}

	Run::Remove(_updateHandle);
}

void Voronoi::SetPoints(List<sf::Vector2f> points)
{
	_points = Move(points);
	MarkForGeneration();
}

void Voronoi::SetPoints(int noRandomPoints)
{
	GeneratePoints(noRandomPoints);
	MarkForGeneration();
}

void Voronoi::SetBoundingBox(const sf::FloatRect& boundingBox)
{
	_boundingBox = boundingBox;
	MarkForGeneration();
}

void Voronoi::SetOutlineColor(const sf::Color& color)
{
	for (int i = 0; i < _polygonsVA.getVertexCount(); i++)
	{
		_polygonsVA[i].color = color;
	}
}

void Voronoi::SetFillColor(const Polygon& polygon, sf::Color color)
{
	const auto result = std::ranges::find(_polygons, polygon);
	if (result != _polygons.end())
	{
		const size_t noPoints = polygon.Points().size();
		const int VAIndex = polygon.FilledVAIndex();
		for (int i = 0; i < noPoints * 3; i++)
		{
			_filledPolygonsVA[VAIndex + i].color = color;
		}
	}
}

void Voronoi::Relax(int iterations)
{
	Debug::Assert(_diagram.has_value(), "Voronoi was not created. _diagram was not instantiated");
	for (int i = 0; i < iterations; i++)
	{
		const jcv_site* sites = jcv_diagram_get_sites(&_diagram.value());
		for (int j = 0; j < _diagram.value().numsites; ++j)
		{
			const jcv_site* site = &sites[j];
			jcv_point sum = site->p;
			int count = 1;
			for (const jcv_graphedge* edge = site->edges; edge != nullptr; edge = edge->next)
			{
				sum.x += edge->pos[0].x;
				sum.y += edge->pos[0].y;
				++count;
			}
			_points[site->index].x = sum.x / count;
			_points[site->index].y = sum.y / count;
		}
		ForceGenerate();
	}
}

auto Voronoi::PolygonAt(const sf::Vector2f& position) -> Voronoi::Polygon&
{
	float minDistance = std::numeric_limits<float>::infinity();
	Polygon* closest = nullptr;
	for (auto& polygon : _polygons)
	{
		const float distance = VecUtils::LengthSq(polygon.VoronoiPoint() - position);
		if (distance < minDistance)
		{
			minDistance = distance;
			closest = &polygon;
		}
	}

	Debug::Assert(closest != nullptr,
	              "Failed to find any polygons closer than std::numeric_limits<float>::infinity() units from given position");
	return *closest;
}

void Voronoi::ForceGenerate()
{
	_wantNewPolygonVA = false;
	Generate();
}

auto Voronoi::ConvertBoundingBox(const sf::FloatRect& boundingBox) -> jcv_rect
{
	return jcv_rect{
		jcv_point{boundingBox.left, boundingBox.top},
		jcv_point{boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height}
	};
}

void Voronoi::Generate()
{
	Debug::Assert(_boundingBox.width > 0.0f && _boundingBox.height > 0.0f, "Invalid bounding box dimensions.");
	Debug::Assert(!_points.empty(), "No points available");

	jcv_rect rect = ConvertBoundingBox(_boundingBox);

	if (_diagram.has_value())
	{
		jcv_diagram_free(&_diagram.value());
		std::memset(&_diagram.value(), 0, sizeof(jcv_diagram));
	}
	_diagram = jcv_diagram();
	jcv_diagram_generate(_points.size(), reinterpret_cast<const jcv_point*>(_points.data()), &rect, nullptr,
	                     &_diagram.value());

	// Generate polygon list from sites
	const jcv_site* sites = jcv_diagram_get_sites(&_diagram.value());

	_polygonsVA.clear();
	_filledPolygonsVA.clear();
	_polygons.clear();
	_polygons.reserve(_diagram.value().numsites);

	const auto setupPolygon = [this](Polygon& polygon, int& currentLineVAIndex, int& currentFilledVAIndex)
	{
		const auto& points = polygon.Points();
		for (int i = 0; i < points.size() - 1; i++)
		{
			_polygonsVA.append({points[i], _defaultGridColor});
			_polygonsVA.append({points[i + 1], _defaultGridColor});
		}
		_polygonsVA.append({points.back(), _defaultGridColor});
		_polygonsVA.append({points.front(), _defaultGridColor});

		const auto& voronoiPoint = polygon.VoronoiPoint();
		const sf::Color fillColor = sf::Color::Transparent; // = polygon.GetFillColor();
		for (int i = 0; i < points.size() - 1; i++)
		{
			_filledPolygonsVA.append({voronoiPoint, fillColor});
			_filledPolygonsVA.append({points[i], fillColor});
			_filledPolygonsVA.append({points[i + 1], fillColor});
		}
		_filledPolygonsVA.append({voronoiPoint, fillColor});
		_filledPolygonsVA.append({points.front(), fillColor});
		_filledPolygonsVA.append({points.back(), fillColor});

		currentLineVAIndex += points.size() * 2;
		currentFilledVAIndex += points.size() * 3;
	};


	for (int i = 0, currentLineVAIndex = 0, currentFilledVAIndex = 0; i < _diagram.value().numsites; ++i)
	{
		const jcv_site* site = &sites[i];

		List<sf::Vector2f> polygonPoints;
		for (jcv_graphedge* edge = site->edges; edge != nullptr; edge = edge->next)
		{
			polygonPoints.emplace_back(edge->pos[0].x, edge->pos[0].y);
		}

		// Setup Polygon
		Polygon polygon(*this, currentLineVAIndex, currentFilledVAIndex, sf::Vector2f(site->p.x, site->p.y),
		                polygonPoints);
		setupPolygon(polygon, currentLineVAIndex, currentFilledVAIndex);
		_polygons.push_back(polygon);
	}

	for (int i = 0; i < _diagram.value().numsites; ++i)
	{
		const jcv_site* site = &sites[i];

		for (jcv_graphedge* edge = site->edges; edge != nullptr; edge = edge->next)
		{
			if (edge->neighbor)
			{
				Polygon& neighbor = PolygonAt(sf::Vector2f(edge->neighbor->p.x, edge->neighbor->p.y));
				_polygons[i].AddNeighbor(&neighbor);
			}
		}
	}
}

void Voronoi::GeneratePoints(int noPoints)
{
	_points.clear();
	_points.reserve(noPoints);

	RandomGenerator<float> generatorX(_boundingBox.left, _boundingBox.left + _boundingBox.width);
	RandomGenerator<float> generatorY(_boundingBox.top, _boundingBox.top + _boundingBox.height);
	for (int i = 0; i < noPoints; i++)
	{
		_points.emplace_back(generatorX.Generate(), generatorY.Generate());
	}
}

void Voronoi::MarkForGeneration()
{
	if (_automaticGeneration)
	{
		_wantNewPolygonVA = true;
	}
}

void Voronoi::SetupUpdateCallback()
{
	_updateHandle = Run::EveryFrame([this]
	{
		if (_wantNewPolygonVA)
		{
			Generate();
			_wantNewPolygonVA = false;
		}
	});
}

void Voronoi::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (_shouldDrawFilledPolygons)
	{
		target.draw(_filledPolygonsVA, states);
	}
	if (_shouldDrawGrid)
	{
		target.draw(_polygonsVA, states);
	}
}
}
