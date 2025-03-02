//
// Created by saumonbro on 2/19/25.
//

#ifndef PARSER_H
#define PARSER_H
#include <AST/Ast.h>
#include <Function.h>
#include <Lexer/Lexer.h>
#include <Program.h>
#include <Variable.h>
#include <map>

class Parser
{
public:
    explicit Parser(std::istream &input);

    std::unique_ptr<Ast> FunctionCall(const std::string &func_name);

    std::unique_ptr<Ast> Factor();

    std::unique_ptr<Ast> Pow();

    std::unique_ptr<Ast> Mul();

    std::unique_ptr<Ast> Expr();

    std::unique_ptr<Ast> Comparison();

    std::unique_ptr<Ast> And();

    std::unique_ptr<Ast> Or();

    std::unique_ptr<Ast> Assignment();

    std::unique_ptr<Ast> If(const std::string &func);

    std::unique_ptr<Ast> Return(const std::string &func);

    std::unique_ptr<Ast> ParseWhile(const std::string &func);

    std::unique_ptr<Ast> ParseBody(const std::string &func);

    std::unique_ptr<Ast> Base(const std::string &func);

    std::shared_ptr<Function> ParseFunction();

    std::unique_ptr<Program> Parse();

    Token CurrentToken();

    std::unordered_map<std::string, int> GetOffsets();

private:
    std::string Eat(TokenType type);
    Lexer m_lexer;

    std::unordered_map<std::string, std::shared_ptr<Function>> m_functions;
    ContextMap m_context;
    int m_offset;
};

#endif // PARSER_H
