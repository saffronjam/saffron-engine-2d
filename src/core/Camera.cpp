#include "Camera.h"

Camera::Camera()
    : m_transform(sf::Transform::Identity),
      m_follow(std::nullopt),
      m_position(0.0f, 0.0f),
      m_rotation(0.0f),
      m_rps(0.2f),
      m_zoom(sf::Vector2f(1.0f, 1.0f))
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
    if (m_follow.has_value())
    {
        SetCenter(*m_follow.value());
    }
    else
    {
        const sf::Vector2f cur_pos = Mouse::GetPos();
        if (Mouse::IsDown(sf::Mouse::Button::Left) && Mouse::IsDown(sf::Mouse::Button::Right))
        {
            m_engaged = true;
            sf::Vector2f delta = cur_pos - m_lastPos;
            if (vl::LengthSq(delta) > 0.0f)
            {
                m_lastPos = cur_pos;
                delta = m_rotationTransform.getInverse().transformPoint(delta);
                delta = m_zoomTransform.getInverse().transformPoint(delta);
                m_position += delta;
            }
        }
        else
        {
            m_lastPos = cur_pos;
            m_engaged = false;
        }
    }

    Zoom((Mouse::GetVerticalScroll() / 100.0f) + 1.0f);

    float angle = 0.0f;

    if (Keyboard::IsDown(sf::Keyboard::Q))
    {
        angle += m_rps * 360.0f * Clock::Delta().asSeconds();
    }
    if (Keyboard::IsDown(sf::Keyboard::E))
    {
        angle -= m_rps * 360.0f * Clock::Delta().asSeconds();
    }
    Rotate(angle);

    if (Keyboard::IsPressed(sf::Keyboard::R))
    {
        ResetTransformation();
    }

    CapZoomLevel();
}

void Camera::Draw(const sf::Drawable &drawable, sf::RenderStates renderStates) const noexcept
{
    renderStates.transform.combine(Window::GetNdcTransform());
    renderStates.transform.combine(m_transform);
    Window::Render(drawable, renderStates);
}

void Camera::Move(const sf::Vector2f &offset) noexcept
{
    m_position += offset;
    m_positionTransform.translate(offset);
    UpdateTransform();
}

void Camera::Zoom(float factor) noexcept
{
    m_zoom *= factor;
    m_zoomTransform.scale(factor, factor);
    UpdateTransform();
}

void Camera::Rotate(float angle) noexcept
{
    m_rotation += angle;
    m_rotationTransform = sf::Transform().rotate(m_rotation);
    UpdateTransform();
}

void Camera::SetCenter(const sf::Vector2f &center) noexcept
{
    m_position = center - sf::Vector2f(1.0f, 1.0f);
    m_positionTransform = sf::Transform().translate(m_position);
    UpdateTransform();
}

void Camera::SetZoom(float factor) noexcept
{
    if (factor != 0.0f)
    {
        m_zoom = sf::Vector2f(factor, factor);
        m_zoomTransform = sf::Transform().scale(m_zoom, m_zoom);
        UpdateTransform();
    }
}

void Camera::SetRotation(float angle) noexcept
{
    m_rotation = angle;
    m_rotationTransform = sf::Transform().rotate(m_rotation);
    UpdateTransform();
}

sf::Vector2f Camera::ScreenToWorld(const sf::Vector2f &point) const noexcept
{
    // sf::FloatRect ndcRect = Window::GetNdcRect();
    // sf::FloatRect vp = m_viewport;
    // vp.left *= ndcRect.width;
    // vp.top *= ndcRect.height;
    // vp.width *= ndcRect.width;
    // vp.height *= ndcRect.height;
    // vp.left += ndcRect.left;
    // vp.top += ndcRect.top;

    // sf::Vector2f rectMid = Lib::Mid(vp);

    // sf::Transform transform = sf::Transform::Identity;
    // transform.translate(rectMid.x, rectMid.y);
    // transform.scale(m_zoom, m_zoom);
    // transform.rotate(m_rotation);
    // transform.translate(-rectMid.x, -rectMid.y);
    // sf::Vector2f transformedPoint = transform.transformPoint(point);

    // transformedPoint = Lib::MapPoint(transformedPoint, vp, ndcRect);

    return vl::Null<float>();
}

sf::Vector2f Camera::WorldToScreen(const sf::Vector2f &point) const noexcept
{
}

void Camera::UpdateTransform() noexcept
{
    m_transform = sf::Transform::Identity;
    m_transform.scale(m_zoom);
    m_transform.rotate(m_rotation);
    m_transform.translate(m_position);
}

void Camera::CapZoomLevel() noexcept
{
    Lib::Constrain(m_zoom.x, 0.9f, 3.0f);
    Lib::Constrain(m_zoom.y, 0.9f, 3.0f);
}

void Camera::ResetTransformation() noexcept
{
    m_position = sf::Vector2f(0.0f, 0.0f);
    m_rotation = 0.0f;
    m_zoom = sf::Vector2f(1.0f, 1.0f);

    m_positionTransform = sf::Transform::Identity;
    m_rotationTransform = sf::Transform::Identity;
    m_zoomTransform = sf::Transform::Identity;
}