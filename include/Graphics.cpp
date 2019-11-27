#include "Graphics.hpp"

#include <cmath>

#include "Camera.hpp"

Graphics::Graphics(Camera &camera)
	: m_window(sf::VideoMode(Graphics::ScreenWidth, Graphics::ScreenHeight), "Application"),
	  m_transform(sf::Transform::Identity),
	  m_camera(camera)
{
	m_window.setVerticalSyncEnabled(true);

	allTextures.reserve(EnumSize);

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

void Graphics::Render(sf::Vector2f point1, sf::Vector2f point2, sf::Color color)
{
	sf::Vertex line[] =
		{
			sf::Vertex(point1),
			sf::Vertex(point2)};
	for (auto &vertex : line)
	{
		vertex.color = color;
	}
	m_window.draw(line, 2, sf::Lines, m_transform);
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
	m_camera.PushChain(point1, point2, color);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2)
{
	DrawLine(sf::Vector2f(x1, y1), sf::Vector2f(x2, y2));
}

void Graphics::ApplyTransformation(sf::Transform add)
{
	m_transform = m_transform * add;
}
