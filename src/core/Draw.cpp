#include "Draw.h"

void Draw::Static(const sf::Drawable &drawable)
{
    Window::m_sfWindow->draw(drawable);
}

void Draw::Transformed(const sf::Drawable &drawable, const sf::Transform &transform)
{
    Window::m_sfWindow->draw(drawable, sf::RenderStates(transform));
}

void Draw::View(const sf::Drawable &drawable, const sf::View &view)
{
    Window::SetView(view);
    Window::m_sfWindow->draw(drawable);
    Window::SetView(sf::View());
}