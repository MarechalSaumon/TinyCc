//
// Created by saumonbro on 2/19/25.
//

#ifndef PARSER_H
#define PARSER_H
#include <map>

#include "Ast.h"
#include "Function.h"
#include "Lexer.h"
#include "Program.h"


class Parser {
public:



    explicit Parser(std::istream& input);

    std::unique_ptr<Ast> Factor();

    std::unique_ptr<Ast> Pow();

    std::unique_ptr<Ast> Mul();

    std::unique_ptr<Ast> Expr();

    std::unique_ptr<Ast> And();

    std::unique_ptr<Ast> Or();

    std::unique_ptr<Ast> Assignment();

    std::shared_ptr<Function> ParseFunction();

    std::unique_ptr<Program> Parse();

    Token CurrentToken();

    std::unordered_map<std::string, int> GetOffsets();

private:
    std::string Eat(TokenType type);
    Lexer m_lexer;

    std::unordered_map<std::string, std::shared_ptr<Function>> m_functions;
    std::shared_ptr<std::map<std::string, std::unique_ptr<Ast>>> m_context;

};



#endif //PARSER_H
