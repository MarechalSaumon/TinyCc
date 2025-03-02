//
// Created by saumonbro on 2/19/25.
//

#include <AST/AstAssignment.h>
#include <AST/AstBinary.h>
#include <AST/AstBlock.h>
#include <AST/AstFunctionCall.h>
#include <AST/AstIdentifier.h>
#include <AST/AstIf.h>
#include <AST/AstLiteral.h>
#include <AST/AstReturn.h>
#include <AST/AstUnary.h>
#include <AST/AstWhile.h>
#include <Function.h>
#include <Parser/Parser.h>
#include <Program.h>
#include <Variable.h>
#include <complex>
#include <iostream>
#include <map>

Parser::Parser(std::istream &input)
    : m_lexer(input)
    , m_context(new std::map<std::string, std::shared_ptr<Variable>>())
    , m_offset(0)
{}

std::unique_ptr<Ast> Parser::FunctionCall(const std::string &func_name)
{
    Eat(TOKEN_LEFTPAR);
    std::vector<std::unique_ptr<Ast>> arguments;
    while (CurrentToken().Type != TOKEN_RIGHTPAR)
    {
        arguments.push_back(Assignment());
        if (CurrentToken().Type == TOKEN_RIGHTPAR)
        {
            break;
        }
        Eat(TOKEN_COMMA);
    }

    unsigned long argc = m_functions.at(func_name)->GetArgc();
    if (arguments.size() != argc)
    {
        throw std::runtime_error(
            "Function called with wrong number of arguments (expected "
            + std::to_string(argc) + " but got"
            + std::to_string(arguments.size()) + ")");
    }
    Eat(TOKEN_RIGHTPAR);

    return std::make_unique<AstFunctionCall>(m_functions[func_name],
                                             std::move(arguments));
}

std::unique_ptr<Ast> Parser::Factor()
{
    if (CurrentToken().Type == TOKEN_IDENTIFIER) // identifier OR function call
    {
        const std::string &tmp = CurrentToken().Data;
        Eat(TOKEN_IDENTIFIER);
        if (CurrentToken().Type == TOKEN_LEFTPAR) // Function call
        {
            return FunctionCall(tmp);
        }
        return std::make_unique<AstIdentifier>(tmp.data(), m_context);
    }
    if (CurrentToken().Type == TOKEN_NUMBER)
    {
        return std::make_unique<AstLiteral>(
            std::atol(Eat(TOKEN_NUMBER).data()));
    }
    if (CurrentToken().Type == TOKEN_LEFTPAR)
    {
        Eat(TOKEN_LEFTPAR);
        auto res = Assignment();
        Eat(TOKEN_RIGHTPAR);
        return res;
    }
    if (CurrentToken().Type == TOKEN_PLUS || CurrentToken().Type == TOKEN_MINUS
        || CurrentToken().Type == TOKEN_NOT)
    {
        auto type = CurrentToken().Type;
        Eat(type);
        return std::make_unique<AstUnary>(Factor(), type);
    }

    throw std::runtime_error("Unexpected token in factor : "
                             + Token::tokenToString(CurrentToken().Type));
}

std::unique_ptr<Ast> Parser::Pow()
{
    std::unique_ptr<Ast> res = Factor();
    if (CurrentToken().Type == TOKEN_POW)
    {
        TokenType op = CurrentToken().Type;
        Eat(op);
        res = std::make_unique<AstBinary>(std::move(res), std::move(Pow()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Mul()
{
    std::unique_ptr<Ast> res = Pow();
    while (CurrentToken().Type == TOKEN_DIV || CurrentToken().Type == TOKEN_MUL
           || CurrentToken().Type == TOKEN_MOD)
    {
        TokenType op = CurrentToken().Type;
        Eat(op);
        res = std::make_unique<AstBinary>(std::move(res), std::move(Pow()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Expr()
{
    std::unique_ptr<Ast> res = Mul();
    while (CurrentToken().Type == TOKEN_PLUS
           || CurrentToken().Type == TOKEN_MINUS)
    {
        TokenType op = CurrentToken().Type;
        Eat(op);
        res = std::make_unique<AstBinary>(std::move(res), std::move(Mul()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Comparison()
{
    std::unique_ptr<Ast> res = Expr();
    while (CurrentToken().Type == TOKEN_EQUAL
           || CurrentToken().Type == TOKEN_NOTEQUAL)
    {
        TokenType op = CurrentToken().Type;
        Eat(op);
        res =
            std::make_unique<AstBinary>(std::move(res), std::move(Expr()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::And()
{
    std::unique_ptr<Ast> res = Comparison();
    while (CurrentToken().Type == TOKEN_AND)
    {
        TokenType op = CurrentToken().Type;
        Eat(TOKEN_AND);
        res = std::make_unique<AstBinary>(std::move(res),
                                          std::move(Comparison()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Or()
{
    std::unique_ptr<Ast> res = And();
    while (CurrentToken().Type == TOKEN_OR)
    {
        TokenType op = CurrentToken().Type;
        Eat(TOKEN_OR);
        res = std::make_unique<AstBinary>(std::move(res), std::move(And()), op);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Assignment()
{
    if (CurrentToken().Type == TOKEN_SET)
    {
        Eat(TOKEN_SET);
        TokenType type = TOKEN_NONE;
        if (CurrentToken().Type
            != TOKEN_IDENTIFIER) // must be a type (initialization)
        {
            type = CurrentToken().Type;
            if (type != TOKEN_INT_TYPE && type != TOKEN_STRING_TYPE)
            {
                Eat(TOKEN_NONE);
            }
            Eat(type);
        }
        std::string id = CurrentToken().Data;
        for (auto it = m_context->begin(); it != m_context->end(); ++it)
        {
            std::cout << "dUIMP ME " << it->first << "\n";
        }
        if (m_context->find(id) == m_context->end())
        {
            if (type == TOKEN_NONE) // no type given
            {
                throw std::runtime_error("Undefined type for : '" + id + "'");
            }
            (*m_context)[id] = (std::make_shared<Variable>(
                m_offset++, type == TOKEN_STRING ? STRING : INTEGER, id));
        }

        Eat(TOKEN_IDENTIFIER);
        Eat(TOKEN_ASSIGNMENT);
        return std::make_unique<AstAssignment>(id, Assignment(), m_context);
    }
    return Or();
}

std::unique_ptr<Ast> Parser::If(
    const std::string &func) // if (<assignment>) { <body> } [else { <body> } ]
{
    if (CurrentToken().Type == TOKEN_IF)
    {
        Eat(TOKEN_IF);
        Eat(TOKEN_LEFTPAR);
        auto condition = Assignment();
        Eat(TOKEN_RIGHTPAR);
        auto body = ParseBody(func);
        std::unique_ptr<Ast> elseBody = nullptr;
        if (CurrentToken().Type == TOKEN_ELSE)
        {
            Eat(TOKEN_ELSE);
            elseBody = ParseBody(func);
        }

        return std::make_unique<AstIf>(std::move(condition), std::move(body),
                                       std::move(elseBody));
    }
    return Assignment();
}

std::unique_ptr<Ast> Parser::Return(const std::string &func)
{
    Eat(TOKEN_RETURN);
    std::unique_ptr<Ast> expr = Assignment();
    return std::make_unique<AstReturn>(func, std::move(expr));
}

std::unique_ptr<Ast> Parser::ParseWhile(const std::string &func)
{
    Eat(TOKEN_WHILE);
    std::cout << "hein" << std::endl;
    Eat(TOKEN_LEFTPAR);
    auto condition = Assignment();
    Eat(TOKEN_RIGHTPAR);
    auto body = ParseBody(func);
    return std::make_unique<AstWhile>(std::move(condition), std::move(body));
}

std::unique_ptr<Ast> Parser::ParseBody(const std::string &func)
{
    std::vector<std::unique_ptr<Ast>> children;
    Eat(TOKEN_LEFT_BRACKET);

    while (CurrentToken().Type != TOKEN_RIGHT_BRACKET)
    {
        if (CurrentToken().Type == TOKEN_RETURN)
        {
            children.push_back(Return(func));
        }
        else if (CurrentToken().Type == TOKEN_WHILE)
        {
            children.push_back(ParseWhile(func));
        }
        else
        {
            children.push_back(Base(func));
        }
        if (CurrentToken().Type == TOKEN_SEMICOLON)
        {
            Eat(TOKEN_SEMICOLON);
        }
        if (CurrentToken().Type == TOKEN_RIGHT_BRACKET)
        {
            break;
        }
    }
    Eat(TOKEN_RIGHT_BRACKET);
    return std::make_unique<AstBlock>(std::move(children));
}

std::unique_ptr<Ast> Parser::Base(const std::string &func)
{
    return If(func);
}

std::shared_ptr<Function> Parser::ParseFunction()
{
    // Read args
    std::cout << Token::tokenToString(CurrentToken().Type) << std::endl;
    bool isStatic = false;
    if (CurrentToken().Type == TOKEN_STATIC)
    {
        Eat(TOKEN_STATIC);
        isStatic = true;
    }
    m_context =
        std::make_shared<std::map<std::string, std::shared_ptr<Variable>>>();
    m_offset = 1;

    std::string id = Eat(TOKEN_IDENTIFIER);
    std::vector<std::string> params;
    Eat(TOKEN_LEFTPAR);
    while (CurrentToken().Type != TOKEN_RIGHTPAR)
    {
        if (CurrentToken().Type != TOKEN_STRING_TYPE
            && CurrentToken().Type != TOKEN_INT_TYPE)
        {
            Eat(TOKEN_NONE);
        }
        Eat(CurrentToken().Type);
        TokenType type = CurrentToken().Type;
        std::string name = Eat(TOKEN_IDENTIFIER);
        m_context->insert(std::make_pair(
            name,
            std::make_shared<Variable>(
                m_offset++, type == TOKEN_STRING ? STRING : INTEGER, name)));
        std::cout << "Insert " << name << " into function " << id << std::endl;
        params.push_back(name);
    }
    Eat(TOKEN_RIGHTPAR);

    std::shared_ptr<Function> res =
        std::make_shared<Function>(id, params, m_context, isStatic);

    m_functions[id] = res;

    res->SetBody(ParseBody(id));
    return res;
}

// Change m_context everytime
std::unique_ptr<Program> Parser::Parse()
{
    std::unordered_map<std::string, std::shared_ptr<Function>> funcs;
    while (CurrentToken().Type != TOKEN_EOF)
    {
        funcs[CurrentToken().Data] =
            ParseFunction(); // For now we only have functions
    }

    return std::make_unique<Program>(std::move(funcs));
}

Token Parser::CurrentToken()
{
    return m_lexer.Peek();
}

std::unordered_map<std::string, int> Parser::GetOffsets()
{
    auto res = std::unordered_map<std::string, int>();
    int offset = 1;
    for (const auto &s : *m_context)
    {
        std::cout << s.first << " offsrt is " << offset * 8 << std::endl;
        res[s.first] = offset * 8;
        offset++;
    }
    return res;
}

std::string Parser::Eat(const TokenType type)
{
    TokenType curType = CurrentToken().Type;

    auto data = m_lexer.Pop().Data;
    if (curType != type)
    {
        throw std::invalid_argument(
            "Expected type : " + Token::tokenToString(type) + " but got : "
            + Token::tokenToString(curType) + " with value " + data);
    }
    return data;
}
