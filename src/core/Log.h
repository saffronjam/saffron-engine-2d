#pragma once
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <map>

#define Log_info(args...) Log::info(__LINE__, __FILE__, args)
#define Log_warning(args...) Log::warning(__LINE__, __FILE__, args)
#define Log_error(args...) Log::error(__LINE__, __FILE__, args)
#define Log_fatal(args...) Log::fatal(__LINE__, __FILE__, args)

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
        White,
        DarkGrey,
        Count
    };

public:
    static void Init();

    template <typename T, typename... Types>
    static void info(int line, const char *file, T var1, Types... var2)
    {
        custom(line, file, "INFO", Color::Green, var1, var2...);
    }
    template <typename T, typename... Types>
    static void warning(int line, const char *file, T var1, Types... var2)
    {
        custom(line, file, "WARNING", Color::Yellow, var1, var2...);
    }
    template <typename T, typename... Types>
    static void error(int line, const char *file, T var1, Types... var2)
    {
        custom(line, file, "ERROR", Color::Red, var1, var2...);
    }
    template <typename T, typename... Types>
    static void fatal(int line, const char *file, T var1, Types... var2)
    {
        custom(line, file, "FATAL", Color::Magenta, var1, var2...);
    }
    template <typename T, typename... Types>
    static void custom(int line, const char *file, std::string type, Color color, T var1, Types... var2)
    {
        std::scoped_lock scoped(m_printLock);
        if (m_colorsEnabled)
        {
            std::cout << "[\033[1;" + m_colors[color] + "m" << type << "\033[0m] \033[2m" << file << "(" << line << ")\033[0m:";
        }
        else
        {
            std::cout << "[" + type + "] " << file << "(" << line << "):";
        }

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