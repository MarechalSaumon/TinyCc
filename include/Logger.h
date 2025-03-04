//
// Created by saumonbro on 3/4/25.
//

#ifndef LOGGER_H
#define LOGGER_H
#include <iostream>
#include <ostream>

enum class background : unsigned int
    {
        black = 40,
        red = 41,
        green = 42,
        yellow = 43,
        blue = 44,
        magenta = 45,
        cyan = 46,
        white = 47
    };

enum class foreground : unsigned int
{
    black = 30,
    red = 31,
    green = 32,
    yellow = 33,
    blue = 34,
    magenta = 35,
    cyan = 36,
    white = 37
};


enum LoggingLevel{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
};

class Logger {

    public:

        Logger& operator=(const Logger&) = delete;
        Logger(const Logger&) = delete;

        static void SetLogLevel(LoggingLevel level);
        template<typename T>
        static void Log(const T&, LoggingLevel level = DEBUG);

       private:
            Logger() : m_stream(std::cout), m_level(DEBUG)
            {
            }

           template<typename T>
           void Send(const T&, LoggingLevel level = DEBUG);
           void SetLevel(LoggingLevel level);

           std::ostream& m_stream;
           LoggingLevel m_level;
           static Logger m_instance;
};


#include "Logger.hxx"
#endif //LOGGER_H
