#include "Camera.h"

Camera::Camera(const sf::Vector2f &position)
    : m_view(Window::GetDefaultView()),
      m_position(position),
      m_follow(nullptr),
      m_zoom(1.0f)
{
}

Camera::~Camera()
{
}

Camera::Camera(const Camera &camera)
    : m_view(camera.m_view),
      m_position(camera.m_position),
      m_follow(camera.m_follow),
      m_zoom(camera.m_zoom)
{
}

const Camera &Camera::operator()(const Camera &camera)
{
    m_view = camera.m_view;
    m_position = camera.m_position;
    m_follow = camera.m_follow;
    m_zoom = camera.m_zoom;
    return *this;
}

void Camera::Draw(const sf::Drawable &drawable, sf::RenderStates renderStates) const noexcept
{
    Window::SetView(m_view);
    Window::Draw(drawable, renderStates);
}

void Camera::Move(const sf::Vector2f &offset) noexcept
{
    m_view.move(offset);
}

void Camera::Zoom(float factor) noexcept
{
    m_view.zoom(factor);
    m_zoom += (factor - 1.0f);
}

void Camera::Rotate(float angle) noexcept
{
    m_view.rotate(angle);
}

void Camera::SetViewport(const sf::FloatRect &viewport) noexcept
{
    m_view.setViewport(viewport);
}

void Camera::SetCenter(const sf::Vector2f &center) noexcept
{
    m_view.setCenter(center);
}

void Camera::SetZoom(float factor) noexcept
{
    if (factor == 0.0f)
        return;
    m_view.zoom(1.0f / m_zoom);
    m_zoom = factor;
    m_view.zoom(m_zoom);
}

void Camera::SetRotation(float angle) noexcept
{
    m_view.setRotation(angle);
}

void Camera::SetFollow(sf::Vector2f *follow) noexcept
{
    m_follow = follow;
}

sf::Vector2f Camera::ScreenToView(const sf::Vector2f &point) const noexcept
{
    return m_view.getTransform().transformPoint(point);
}

sf::Vector2f Camera::ScreenToWorld(const sf::Vector2f &point) const noexcept
{
    // sf::FloatRect screenRect = Lib::ConvertTo<float>(Window::GetScreenRect());
    // sf::FloatRect vpNorm = m_view.getViewport();
    // sf::FloatRect vp = vpNorm;
    // vp.left *= screenRect.width;
    // vp.top *= screenRect.height;
    // vp.width *= screenRect.width;
    // vp.height *= screenRect.height;

    // // Log_info("Left: ", viewportInScreenSpace.left, " Top: ", viewportInScreenSpace.top, " Width: ", viewportInScreenSpace.width, " Height: ", viewportInScreenSpace.height);

    // sf::Vector2f point_trans = point;

    // sf::Transform transform = sf::Transform::Identity;

    // transform.translate(-Lib::Mid(vp));
    // point_trans = transform.transformPoint(point);
    // Log_info("1: ", point_trans.x, ", ", point_trans.y);

    // transform.scale(1.0f / vp.width, 1.0f / vp.height);
    // point_trans = transform.transformPoint(point);
    // Log_info("2: ", point_trans.x, ", ", point_trans.y);

    // transform.scale(vp.width / m_zoom, vp.height / m_zoom);
    // point_trans = transform.transformPoint(point);
    // Log_info("3: ", point_trans.x, ", ", point_trans.y);

    // sf::Vector2f halfSize(m_view.getSize().x / 2.0f, m_view.getSize().y / 2.0f);
    // transform.translate(Lib::Mid(screenRect));
    // point_trans = transform.transformPoint(point);
    // Log_info("4: ", point_trans.x, ", ", point_trans.y);

    // // sf::Vector2f _point = Lib::MapPoint(point, viewportInScreenSpace, screenRect);

    // // // transform.translate(-sf::Vector2f(screenRect.width / 2.0f, screenRect.height / 2.0f));

    // // // transform.rotate(m_view.getRotation());
    // // sf::Vector2f halfSize(m_view.getSize().x / 2.0f, m_view.getSize().y / 2.0f);
    // // sf::Vector2f translate = m_view.getCenter() - halfSize;
    // // transform.translate(translate);

    // // _point = transform.transformPoint(_point);

    return point;
}