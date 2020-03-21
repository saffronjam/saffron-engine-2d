#pragma once
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <map>

class Log
{
public:
    enum Color
    {
        Red = 0,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White
    };

public:
    static void Init();

    template <typename T, typename... Types>
    static void info(T var1, Types... var2)
    {
        custom("INFO", Color::Green, var1, var2...);
    }
    template <typename T, typename... Types>
    static void warning(T var1, Types... var2)
    {
        custom("WARNING", Color::Yellow, var1, var2...);
    }
    template <typename T, typename... Types>
    static void error(T var1, Types... var2)
    {
        custom("ERROR", Color::Red, var1, var2...);
    }
    template <typename T, typename... Types>
    static void fatal(T var1, Types... var2)
    {
        custom("FATAL", Color::Magenta, var1, var2...);
    }
    template <typename T, typename... Types>
    static void custom(std::string type, Color color, T var1, Types... var2)
    {
        std::scoped_lock scoped(m_printLock);
        m_colorsEnabled ? std::cout << "[\033[1;" + m_colors[color] + "m" + type + "\033[0m] " : std::cout << "[" + type + "] ";
        if (m_prefix.length())
            std::cout << m_prefix << " ";
        rec(var1, var2...);
    }

    static void EnableColors() { m_colorsEnabled = true; }
    static void DisableColors() { m_colorsEnabled = false; }
    static void AddPrefix(std::string prefix) { m_prefix += prefix; }
    static void ClearPrefix() { m_prefix.clear(); }

private:
    template <typename T, typename... Types>
    static void rec(T var1, Types... var2)
    {
        std::cout << var1 << " ";
        rec(var2...);
    }
    static void rec()
    {
        std::cout << std::endl;
    }

private:
    static std::mutex m_printLock;
    static bool m_colorsEnabled;
    static std::string m_prefix;
    static std::map<Color, std::string> m_colors;
};
