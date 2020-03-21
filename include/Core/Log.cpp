#include "Log.hpp"

std::mutex Log::m_printLock = std::mutex();
bool Log::m_colorsEnabled = true;
std::string Log::m_prefix = "";
std::map<Log::Color, std::string> Log::m_colors = {
    {Log::Color::Red, "31"},
    {Log::Color::Green, "32"},
    {Log::Color::Yellow, "33"},
    {Log::Color::Blue, "34"},
    {Log::Color::Magenta, "35"},
    {Log::Color::Cyan, "36"},
    {Log::Color::White, "37"},
};