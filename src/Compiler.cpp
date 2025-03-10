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

    rodata_ = parser.GetRodata();

    m_program->Optimize();


    if (!m_program->Returns())
    {
        Logger::Log("Some functions do not comply to their specified return type (Either a procedure attempted to return a value, or a function does not return a value on all execution path).", ERROR);
        throw std::runtime_error("");
    }

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

std::string Compiler::GetBuiltins()
{
    return "";
}

std::string Compiler::GetRodata()
{
    std::string res = ".section .rodata\n";
    for (const auto & [label, value] : rodata_)
    {
        res += (label + ":\n.asciz \"" + value + "\"\n");
    }
    return res;
}

int Compiler::Compile(const std::string &out)
{
    // Do stuff
    std::ofstream outFile(out);

    std::string res = m_program->Compile();
    outFile << res << std::endl;
    outFile << GetRodata() << std::endl;

    outFile.close();
    // GenerateExecutable(out);
    return 0;
}
