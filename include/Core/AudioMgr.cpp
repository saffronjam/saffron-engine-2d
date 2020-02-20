#include "AudioMgr.hpp"
#include <iostream>

AudioMgr::AudioMgr()
{
    if (!m_allMusic[MusicTest].openFromFile("assets/audio/testMusic.wav"))
        std::cerr << "Error: Could not open testMusic.wave!" << std::endl;

    if (!m_allSoundBuffers[SoundTest].loadFromFile("assets/audio/testSound.wav"))
        std::cerr << "Error: Could not open testSound.wave!" << std::endl;
}
