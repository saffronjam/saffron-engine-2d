#include "LightningMgr.h"

std::vector<LightningMgr::RenderLight> LightningMgr::m_renderLights;

sf::Shader LightningMgr::m_shadowMapShader;
sf::Shader LightningMgr::m_renderLightsShader;
sf::RectangleShape LightningMgr::m_ambientRect;

sf::RenderTexture LightningMgr::m_collectiveLightMap;

float LightningMgr::m_resolution = 4096;

LightningMgr::LightningMgr()
{
    m_shadowMapShader.loadFromFile("res/shaders/vertexShader.vert", "res/shaders/shadowMap.frag");
    m_renderLightsShader.loadFromFile("res/shaders/vertexShader.vert", "res/shaders/renderLights.frag");
    m_collectiveLightMap.create(Window::GetSize().x, Window::GetSize().y);
    m_ambientRect.setFillColor(sf::Color(0, 0, 0));
    m_ambientRect.setSize(Lib::ConvertTo<float>(Window::GetSize()));
}

LightningMgr::~LightningMgr()
{
    for (auto &renderLight : m_renderLights)
    {
        delete renderLight.occluders;
        delete renderLight.shadowMap;
        delete renderLight.lightMap;
        renderLight.occluders = nullptr;
        renderLight.shadowMap = nullptr;
        renderLight.lightMap = nullptr;
    }
}

void LightningMgr::Draw()
{
    Window::Draw(sf::Sprite(m_collectiveLightMap.getTexture()), sf::RenderStates(sf::BlendMultiply));
}

void LightningMgr::AddOccluder(const sf::Drawable &drawable, sf::RenderStates renderStates) noexcept
{
    sf::RenderStates copy = renderStates;
    for (auto &renderLight : m_renderLights)
    {
        float hypotenuse = std::sqrt(std::pow(renderLight.light->GetRadius(), 2.0f));
        float radius = renderLight.light->GetRadius();
        sf::Vector2f position = renderLight.light->GetPosition();
        copy.transform.combine(sf::Transform().translate(sf::Vector2f(radius, radius) / 2.0f - position)); // - sf::Vector2f(radius, radius)));
        renderLight.occluders->draw(drawable, copy);
        copy = renderStates;
    }
}

void LightningMgr::ClearOccluders() noexcept
{
    for (auto &renderLight : m_renderLights)
        renderLight.occluders->clear(sf::Color(0, 0, 0, 0));
}

void LightningMgr::DisplayOccluders() noexcept
{
    for (auto &renderLight : m_renderLights)
        renderLight.occluders->display();
}

void LightningMgr::RenderShadowMaps() noexcept
{
    for (auto &renderLight : m_renderLights)
    {
        renderLight.shadowMap->clear(sf::Color(0, 0, 0, 0));
        m_shadowMapShader.setUniform("resolution", m_resolution);
        renderLight.shadowMap->draw(sf::Sprite(renderLight.occluders->getTexture()), sf::RenderStates(&m_shadowMapShader));
    }
}

void LightningMgr::RenderLightMaps() noexcept
{
    for (auto &renderLight : m_renderLights)
    {
        m_renderLightsShader.setUniform("resolution", m_resolution);
        renderLight.lightMap->clear(sf::Color(0, 0, 0, 0));
        sf::Sprite shadowMapSprite(renderLight.shadowMap->getTexture());
        for (int i = 0; i < renderLight.light->GetRadius(); i++)
        {
            shadowMapSprite.setPosition(0.0, (float)i);
            renderLight.lightMap->draw(shadowMapSprite, sf::RenderStates(&m_renderLightsShader));
        }
        renderLight.lightMap->display();
    }
}

void LightningMgr::RenderCollectiveLightMap() noexcept
{
    m_collectiveLightMap.clear(sf::Color(0, 0, 0, 0));
    for (auto &renderLight : m_renderLights)
    {
        sf::Sprite lightSprite(renderLight.lightMap->getTexture());
        lightSprite.setScale(renderLight.light->GetRadius() / lightSprite.getLocalBounds().width,
                             renderLight.light->GetRadius() / lightSprite.getLocalBounds().height);
        lightSprite.setPosition(renderLight.light->GetPosition() -
                                sf::Vector2f(lightSprite.getLocalBounds().width, lightSprite.getLocalBounds().height) / 2.0f);
        lightSprite.setColor(renderLight.light->GetColor());
        Camera::Draw(lightSprite, m_collectiveLightMap, sf::RenderStates(sf::BlendAdd));
    }
    m_collectiveLightMap.draw(m_ambientRect, sf::RenderStates(sf::BlendAdd));
    m_collectiveLightMap.display();
}

void LightningMgr::AddLight(const Light::Ptr &light)
{
    m_renderLights.push_back(RenderLight(light));
}