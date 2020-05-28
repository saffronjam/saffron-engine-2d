
#include "IApp.h"

IApp::IApp()
    : m_screenList(std::make_unique<ScreenList>(this)),
      m_currentScreen(nullptr),
      m_isRunning(true)
{
    EventMgr::AddOnEventCallback(this);
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    Window::Create("V-2DFramework", mode.width * 0.7f, mode.height * 0.7f);
}

IApp::~IApp()
{
}

void IApp::Run()
{
    try
    {
        Init();
    }
    LogCatch;

    while (m_isRunning)
    {
        FPSLimiter::Start();
        PacketMgr::HandleAllPackets();
        Keyboard::Update();
        Mouse::Update();
        EventMgr::PollAll();
        Window::Clear();
        try
        {
            Update();
            Draw();
        }
        LogCatch;

        Window::Present();
        Clock::Mark();
        Clock::Reset();
        FPSLimiter::Adjust();
    }
}
void IApp::Exit()
{
    if (m_currentScreen)
        m_currentScreen->OnExit();
    if (m_screenList)
        m_screenList.reset();

    m_isRunning = false;
}

void IApp::Update()
{
    if (m_currentScreen)
    {
        switch (m_currentScreen->GetState())
        {
        case ScreenState::Running:
            m_currentScreen->Update();
            break;
        case ScreenState::ChangeNext:
            m_currentScreen->OnExit();
            m_currentScreen = m_screenList->MoveNext();
            if (m_currentScreen)
            {
                m_currentScreen->SetRunning();
                m_currentScreen->OnEntry();
            }
            break;
        case ScreenState::ChangePrevious:
            m_currentScreen->OnExit();
            m_currentScreen = m_screenList->MovePrevious();
            if (m_currentScreen)
            {
                m_currentScreen->SetRunning();
                m_currentScreen->OnEntry();
            }
            break;
        case ScreenState::ExitApp:
            Exit();
            break;
        default:
            break;
        }
    }
}
void IApp::Draw()
{
    if (m_currentScreen && m_currentScreen->GetState() == ScreenState::Running)
    {
        m_currentScreen->Draw();
    }
}

bool IApp::Init()
{
    OnInit();
    AddScreens();

    m_currentScreen = m_screenList->GetCurrent();
    m_currentScreen->OnEntry();
    m_currentScreen->SetRunning();

    return true;
}

void IApp::OnEvent(const sf::Event &event)
{
    switch (event.type)
    {
    case sf::Event::EventType::Closed:
        Exit();
        break;
    }
}

IApp::Exception::Exception(int line, const char *file, const char *errorString) noexcept
    : IException(line, file),
      errorString(errorString)
{
}

const char *IApp::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << "[Type] " << GetType() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *IApp::Exception::GetType() const noexcept
{
    return "V-2DEngine App Exception";
}

const char *IApp::Exception::GetErrorString() const noexcept
{
    return errorString.c_str();
}