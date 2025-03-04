//
// Created by saumonbro on 2/19/25.
//

#include <Compiler.h>
#include <Parser/Parser.h>
#include <fstream>
#include <iostream>
#include <Logger.h>

Compiler::Compiler(const std::string &path)
{

    this->path = path;
    std::ifstream in(path);
    Parser parser(in);

    try
    {
        m_program = std::move(parser.Parse());
    }
    catch (const std::exception &e)
    {
        Logger::Log(path + ":" + std::to_string(parser.GetLineNumber()) + ":\n" + std::string{e.what()} + '\n' + parser.GetCurrentLine(), ERROR);
        throw;
    }

    m_program->Optimize();

    /*
    offsets = parser.GetOffsets();

    auto opt = (root->Optimize());
    if (opt != nullptr) {
        root = std::move(opt);
    }
    std::cout << root->Dump() << std::endl;
    std::cout << root->Evaluate() << std::endl;*/
}

int Compiler::GenerateExecutable(const std::string &filepath)
{
    std::string outputFile = "output";
    std::string command = "cc " + filepath + " -o " + outputFile;

    return std::system(command.c_str());
}

int Compiler::Compile(const std::string &out)
{
    // Do stuff
    std::ofstream outFile(out);

    std::string res = m_program->Compile();
    outFile << res << std::endl;

    outFile.close();
    // GenerateExecutable(out);
    return 0;
}
