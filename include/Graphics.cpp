#include "Graphics.hpp"

#include "Math.hpp"

#include "Camera.hpp"

Graphics::Graphics(Camera &camera)
	: m_window(sf::VideoMode(Graphics::ScreenWidth, Graphics::ScreenHeight), "Application"),
	  m_transform(sf::Transform::Identity),
	  m_camera(camera)
{
	m_window.setVerticalSyncEnabled(true);

	allTextures.reserve(TextureMapping::TextureCount);

	// sf::Texture *GrassStandard = new sf::Texture;

	// GrassStandard->loadFromFile("assets/Grass.png");

	// allTextures.push_back(GrassStandard);
}

Graphics::~Graphics()
{
	for (auto texture : allTextures)
	{
		delete texture;
	}
}

void Graphics::ClearFrame()
{
	m_window.clear();
}

void Graphics::EndFrame()
{
	m_window.display();
}

void Graphics::Render(const sf::Drawable &drawable)
{
	m_window.draw(drawable, m_transform);
	m_transform = sf::Transform::Identity;
}

void Graphics::Draw(const sf::Drawable &drawable)
{
	m_camera.PushChain(drawable);
}

void Graphics::Draw(sf::Vector2f point, float radius, sf::Color color)
{
	sf::CircleShape dot(radius);
	dot.setFillColor(color);
	dot.setPosition({point.x - radius, point.y - radius});
	m_camera.PushChain(dot);
}

void Graphics::DrawLine(sf::Vector2f point1, sf::Vector2f point2, sf::Color color)
{
	sf::VertexArray lines(sf::Lines, 2);
	lines[0].position = point1;
	lines[0].color = color;
	lines[1].position = point2;
	lines[1].color = color;
	m_camera.PushChain(lines);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2)
{
	DrawLine(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2));
}

void Graphics::ApplyTransformation(sf::Transform add)
{
	m_transform = m_transform * add;
}

sf::Vector2i Graphics::GetMousePosition()
{
	sf::Vector2i final = sf::Mouse::getPosition(GetRenderWindow());

	final -= m_camera.GetOffset();

	const float cosTheta = cos((-m_camera.GetAngle() * Math::Constants::PI) / 180);
	const float sinTheta = sin((-m_camera.GetAngle() * Math::Constants::PI) / 180);
	const float new_x = final.x * cosTheta - final.y * sinTheta;
	final.y = final.x * sinTheta + final.y * cosTheta;
	final.x = new_x;

	final.x /= m_camera.GetZoom().x;
	final.y /= m_camera.GetZoom().y;

	final += (sf::Vector2i)m_camera.GetPos();

	return final;
}
