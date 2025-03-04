#include <Compiler.h>
#include <Logger.h>
#include <Lexer/Lexer.h>
#include <Parser/Parser.h>
#include <fstream>
#include <iostream>

int compile(const std::string &def, unsigned long opts)
{
    (void) opts;
    Logger::SetLogLevel(DEBUG);
    Compiler compiler(
        def
    );

    return compiler.Compile();
}

std::string Usage(const std::string &name)
{
    return "Usage: " + name + " <file>";
}

void displayHelp()
{
    std::cout << "Usage: TinyCc [options] file..." << '\n' << "\nOptions:\n";
    std::cout << "\t" << "-h:        Display this message an exit.\n";
    std::cout << "\t" << "-v:        Sets the logging level to debug (all).\n";
    std::cout << "\t" << "-c:        Compile and assemble, but do not link.\n";
    std::cout << "\t" << "-S:        Compile, but do not assemble or link.\n";
    std::cout << "\t" << "-o <file>: Place the output into <file>." << std::endl;
}

int main(int argc, char *argv[])
{
    std::string def = "/home/saumonbro/Desktop/CodeForFun/C++/TinyCC2/TinyCc/test.txt";

    unsigned long opts = 0;

    LoggingLevel level = INFO;

    std::vector<std::string> files;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-v")
        {
            level = DEBUG;
        } else if (arg == "-h")
        {
            displayHelp();
            return 0;
        } else if (arg == "-c") // compile, no link
        {
        } else if (arg == "-o")
        {
        } else if (arg == "-S")
        {
        } else
        {
            files.push_back(arg);
        }
    }
    Logger::SetLogLevel(level);

    if (files.empty())
    {
        Logger::Log("No input files.\n" + Usage(argv[0]), ERROR);
        return 1;
    }

    try
    {
        int res = compile(def, opts);
        if (res == 0)
        {
            Logger::Log("Compilation successful.", INFO);
        }
    } catch (const std::exception &e)
    {
        return 1;
    }
}
