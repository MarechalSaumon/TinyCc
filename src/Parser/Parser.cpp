//
// Created by saumonbro on 2/19/25.
//
#include <Class.h>
#include <ErrorManager.h>
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
#include <Variables/Variable.h>
#include <complex>
#include <map>
#include <AST/AstLiteralString.h>
#include <Context.h>
#include <Method.h>

Parser::Parser(std::istream &input)
    : m_lexer(input)
    , m_context(new std::map<std::string, std::shared_ptr<Variable>>())
    , m_offset(0)
    , global_context_()
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


    if (!m_functions.contains(func_name)) // stdlib ?
    {
        Logger::Log("Reading stdlib function.", INFO);
        std::vector<std::string> params;
        auto func = std::make_shared<Function>(func_name, params, m_context, global_context_);
        Eat(TOKEN_RIGHTPAR);

        return std::make_unique<AstFunctionCall>(func,
                                                 std::move(arguments));
    }
    else
    {
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
    if (CurrentToken().Type == TOKEN_STRING)
    {
        std::string label = Utils::GetNewLocalLabel();
        rodata_.emplace_back(label, Eat(TOKEN_STRING));
        Logger::Log("Pushing ");
        return std::make_unique<AstLiteralString>( label);
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


    throw std::runtime_error("Unexpected token as factor: "
                             + Token::tokenToString(CurrentToken().Type));
}

std::unique_ptr<Ast> Parser::Dot()
{
    std::unique_ptr<Ast> res = Factor();
    if (CurrentToken().Type == TOKEN_DOT)
    {
        Eat(TOKEN_DOT);
        res = std::make_unique<AstBinary>(std::move(res), std::move(Pow()), TOKEN_DOT);
    }
    return res;
}

std::unique_ptr<Ast> Parser::Pow()
{
    std::unique_ptr<Ast> res = Dot();
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


VariableType GetType(const std::string &name)
{
    if (name == "int")
    {
        return INTEGER;
    }
    if (name == "string")
    {
        return STRING;
    }
    if (name == "void")
    {
        return VOID;
    }
    throw std::runtime_error("Unknown variable type: " + name);
}

std::unique_ptr<Ast> Parser::Assignment()
{
    if (CurrentToken().Type == TOKEN_SET || CurrentToken().Type == TOKEN_TYPE)
    {
        if (CurrentToken().Type == TOKEN_SET)
            Eat(TOKEN_SET);


        VariableType type = NONE;
        if (CurrentToken().Type != TOKEN_IDENTIFIER) // must be a type (initialization)
        {
            if (CurrentToken().Type != TOKEN_TYPE)
            {
                throw std::runtime_error(GetErrorMessage(CurrentToken(), {TOKEN_TYPE}));
            }
            type = GetType(CurrentToken().Data);
            Eat(TOKEN_TYPE);
        }
        const std::string& id = CurrentToken().Data; // Oh no

        if (!m_context->contains(id))
        {
            if (type == NONE) // no type given
            {
                throw std::runtime_error(GetErrorMessage(CurrentToken(), {TOKEN_TYPE}));
            }


            (*m_context)[id] = make_variable(m_offset++, id, type);
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
    // std::cout << "hein" << std::endl;
    Eat(TOKEN_LEFTPAR);
    auto condition = Assignment();
    Eat(TOKEN_RIGHTPAR);
    auto body = ParseBody(func);
    return std::make_unique<AstWhile>(std::move(condition), std::move(body));
}

std::unique_ptr<Ast> Parser::ParseBody(const std::string &func)
{
    std::vector<std::unique_ptr<Ast>> children;
    Eat(TOKEN_LEFT_BRACE);

    while (CurrentToken().Type != TOKEN_RIGHT_BRACE)
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
        if (CurrentToken().Type == TOKEN_RIGHT_BRACE)
        {
            break;
        }
    }
    Eat(TOKEN_RIGHT_BRACE);
    return std::make_unique<AstBlock>(std::move(children));
}

std::unique_ptr<Ast> Parser::Base(const std::string &func)
{
    return If(func);
}

std::vector<std::string> Parser::WriteParametersInContext()
{
    m_context = std::make_shared<std::map<std::string, std::shared_ptr<Variable>>>();
    m_offset = 1;
    std::vector<std::string> params;
    Eat(TOKEN_LEFTPAR);
    while (CurrentToken().Type != TOKEN_RIGHTPAR)
    {
        if (CurrentToken().Type != TOKEN_TYPE)
        {
            throw std::runtime_error(GetErrorMessage(CurrentToken(), {TOKEN_TYPE}));
        }
        Eat(CurrentToken().Type);
        TokenType type = CurrentToken().Type;
        std::string name = Eat(TOKEN_IDENTIFIER);

        m_context->insert(std::make_pair(
            name,
            make_variable(
                m_offset++, name, type == TOKEN_STRING ? STRING : INTEGER)));

        // std::cout << "Insert " << name << " into function " << id << std::endl;
        params.push_back(name);
    }
    Eat(TOKEN_RIGHTPAR);
    return params;
}

template<typename T>
std::shared_ptr<T> Parser::ParseFunction()
{
    static_assert(std::is_base_of_v<Function, T>, "T must derive from Function");
    // Read args
    // std::cout << Token::tokenToString(CurrentToken().Type) << std::endl;
    bool isStatic = false;
    if (CurrentToken().Type == TOKEN_STATIC)
    {
        Eat(TOKEN_STATIC);
        isStatic = true;
    }
    VariableType returnType = GetType(Eat(TOKEN_TYPE));

    std::string id = Eat(TOKEN_IDENTIFIER);
    std::vector<std::string> params = WriteParametersInContext();

    std::shared_ptr<T> res =
        std::make_shared<T>(id, params, m_context, global_context_, isStatic, returnType);

    m_functions[id] = res;

    res->SetBody(ParseBody(id));
    return res;
}

bool Parser::IsPublic()
{
    bool isPublic = false;
    if (CurrentToken().Type == TOKEN_VISIBILITY)
    {
        if (Eat(TOKEN_VISIBILITY) == "public")
        {
            isPublic = true;
        }
    }
    return isPublic;
}

void Parser::ParseFields(std::vector<Attribute> &attributes)
{
    Eat(TOKEN_FIELDS);
    Eat(TOKEN_LEFT_BRACE);
    while (CurrentToken().Type != TOKEN_RIGHT_BRACE)
    {
        bool isPublic = IsPublic();
        VariableType type = GetType(Eat(TOKEN_TYPE));
        std::string name = Eat(TOKEN_IDENTIFIER);
        attributes.emplace_back(type, name, isPublic);
        Eat(TOKEN_SEMICOLON);
    }

    Eat(TOKEN_RIGHT_BRACE);
}

std::unique_ptr<Class> Parser::ParseClass()
{
    Eat(TOKEN_CLASS);
    std::string id = Eat(TOKEN_IDENTIFIER);

    Eat(TOKEN_LEFT_BRACE);

    std::vector<Attribute> attributes;
    std::set<std::shared_ptr<Method>> methods;

    while (CurrentToken().Type != TOKEN_RIGHT_BRACE)
    {
        if (CurrentToken().Type == TOKEN_FIELDS)
        {
            ParseFields(attributes);
        }
        else if (CurrentToken().Type == TOKEN_IDENTIFIER && CurrentToken().Data == id) // constructor
        {
            Eat(TOKEN_IDENTIFIER);
            std::vector<std::string> params = WriteParametersInContext();
            std::shared_ptr<Method> res = std::make_shared<Method>(id, params, m_context, global_context_, false, VOID);
            res->SetBody(ParseBody(id));
        }
        else
        {
            bool isPublic = IsPublic();
            (void)isPublic;
            std::shared_ptr<Method> func = ParseFunction<Method>();
            methods.insert(func);
        }
    }


    return std::make_unique<Class>(methods, attributes, id);
}

// Change m_context everytime
std::unique_ptr<Program> Parser::Parse()
{
    std::unordered_map<std::string, std::shared_ptr<Function>> funcs;
    while (CurrentToken().Type != TOKEN_EOF)
    {
        if (CurrentToken().Type == TOKEN_CLASS)
        {
            global_context_->AddClass(ParseClass());
        }
        else
        {
            funcs[CurrentToken().Data] =
                ParseFunction<Function>(); // For now, we only have functions
        }
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
    for (const auto& [identifier, _] : *m_context)
    {
        res[identifier] = offset * 8;
        offset++;
    }
    return res;
}

std::string Parser::GetCurrentLine()
{
    return m_lexer.GetCurrentLine();
}

int Parser::GetLineNumber() const
{
    return m_lexer.GetLineNumber();
}


std::string Parser::Eat(const TokenType type)
{
    TokenType curType = CurrentToken().Type;

    auto data = m_lexer.Pop().Data;
    if (curType != type)
    {
        throw std::invalid_argument(
            "Expected: " + Token::tokenToString(type) + " but got : "
            + Token::tokenToString(curType) + " with value " + data);
    }
    return data;
}


std::vector<std::pair<std::string, std::string>> Parser::GetRodata()
{
    return rodata_;
}