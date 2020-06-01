#include "Camera.h"

Camera::Camera(const sf::Vector2f &position)
    : m_view(Window::GetDefaultView()),
      m_viewport(m_view.getViewport()),
      m_follow(nullptr),
      m_zoom(1.0f)
{
    SetViewport(sf::FloatRect(0.1f, 0.1f, 0.5f, 0.5f));
}

Camera::~Camera()
{
}

Camera::Camera(const Camera &camera)
    : m_view(camera.m_view),
      m_follow(camera.m_follow),
      m_zoom(camera.m_zoom)
{
}

const Camera &Camera::operator()(const Camera &camera)
{
    m_view = camera.m_view;
    m_follow = camera.m_follow;
    m_zoom = camera.m_zoom;
    return *this;
}

void Camera::Update()
{
    if (m_follow.has_value())
    {
        m_view.setCenter(*m_follow.value());
    }
}

void Camera::Draw(const sf::Drawable &drawable, sf::RenderStates renderStates) const noexcept
{
    renderStates.transform.combine(Window::GetNdcTransform());
    Window::SetView(m_view);
    Window::Render(drawable, renderStates);
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
    m_viewport = viewport;
    auto vp = viewport;
    vp.top = 1.0f - vp.top - vp.height;

    m_view.setViewport(vp);
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
    m_rotation = angle;
    m_view.setRotation(angle);
}

sf::Vector2f Camera::ScreenToWorld(const sf::Vector2f &point) const noexcept
{
    sf::FloatRect ndcRect = Window::GetNdcRect();
    sf::FloatRect vp = m_viewport;
    vp.left *= ndcRect.width;
    vp.top *= ndcRect.height;
    vp.width *= ndcRect.width;
    vp.height *= ndcRect.height;
    vp.left += ndcRect.left;
    vp.top += ndcRect.top;

    sf::Vector2f rectMid = Lib::Mid(vp);

    sf::Transform transform = sf::Transform::Identity;
    transform.translate(rectMid.x, rectMid.y);
    transform.scale(m_zoom, m_zoom);
    // transform.rotate(m_rotation);
    transform.translate(-rectMid.x, -rectMid.y);
    sf::Vector2f transformedPoint = transform.transformPoint(point);

    transformedPoint = Lib::MapPoint(transformedPoint, vp, ndcRect);

    return transformedPoint;
}

sf::Vector2f Camera::WorldToScreen(const sf::Vector2f &point) const noexcept
{
}