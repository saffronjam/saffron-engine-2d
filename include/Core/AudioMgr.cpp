#include "AudioMgr.hpp"
#include "Log.hpp"

AudioMgr::AudioMgr()
{
    for (auto &music : m_allMusic)
        music = new sf::Music;

    for (auto &soundBuffer : m_allSoundBuffers)
        soundBuffer = new sf::SoundBuffer;

    if (!m_allMusic[MusicTest]->openFromFile("assets/audio/testMusic.wav"))
        Log::warning("Could not open testMusic.wav");

    if (!m_allSoundBuffers[SoundTest]->loadFromFile("assets/audio/testSound.wav"))
        Log::warning("Could not open testSound.wave");
}

AudioMgr::~AudioMgr()
{
    for (auto &music : m_allMusic)
        delete music;

    for (auto &soundBuffer : m_allSoundBuffers)
        delete soundBuffer;
}