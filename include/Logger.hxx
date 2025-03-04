//
// Created by saumonbro on 3/4/25.
//
#pragma once

#include <Logger.h>
#include <chrono>
#include <iterator>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>


inline std::string LoggingLevelToString(LoggingLevel level)
{
    switch (level)
    {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        case CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
inline std::string GetDateTime() {
    time_t     now = time(nullptr);
    tm  tstruct{};
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return "[" + std::string{buf};
}

template<typename T>
void Logger::Log(const T& msg, LoggingLevel level)
{
    static Logger m_instance;
    m_instance.Send(msg, level);
}

template<>
inline void Logger::Log(const std::string& msg, LoggingLevel level)
{
    static Logger m_instance;
    m_instance.Send(msg, level);
}


inline void Logger::SetLogLevel(LoggingLevel level)
{
    static Logger m_instance;
    m_instance.SetLevel(level);
}

inline foreground GetColor(LoggingLevel level)
{
    switch (level)
    {
        case DEBUG:
            return foreground::green;
        case INFO:
            return foreground::blue;
        case WARNING:
            return foreground::yellow;
        case ERROR:
            return foreground::red;
        case CRITICAL:
            return foreground::red;
        default:
            return foreground::white;
    }

}



template<typename T>
          void Logger::Send(const T& msg, LoggingLevel level)
{
    if (m_level <= level)
    {
        std::string color = "\x1B[" + std::to_string(static_cast<int>(GetColor(level))) + "m";

        m_stream << /*GetDateTime() << " - "*/  "[" << color << LoggingLevelToString(level)  << "\x1B[0m" << "] " << color;
        std::istringstream ss{msg};
        std::string buf;
        getline(ss, buf, '\n');
        m_stream << buf << (ss.eof() ? "" : "\n");
        while (getline(ss, buf, '\n'))
        {
            m_stream << "\t| " << buf << (ss.eof() ? "" : "\n");

        }
        m_stream << "\x1B[0m" << std::endl;

    }
}

inline void Logger::SetLevel(LoggingLevel level)
{
    m_level = level;
}