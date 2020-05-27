#pragma once

#include <exception>
#include <sstream>

#define LogCatch                                   \
    catch (const IException &e)                    \
    {                                              \
        log_fatal("\n%s", e.what());               \
    }                                              \
    catch (const std::exception &e)                \
    {                                              \
        log_fatal("\n%s", e.what());               \
    }                                              \
    catch (...)                                    \
    {                                              \
        log_fatal("\n%s", "No details available"); \
    }

class IException : public std::exception
{
public:
    IException(int line, const char *file) noexcept;
    const char *what() const noexcept override;
    virtual const char *GetType() const noexcept;
    int GetLine() const noexcept { return m_line; }
    const std::string GetFile() const noexcept { return m_file; }
    std::string GetOriginString() const noexcept;

private:
    int m_line;
    std::string m_file;

protected:
    mutable std::string whatBuffer;
};
