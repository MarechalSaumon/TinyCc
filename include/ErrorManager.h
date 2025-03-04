//
// Created by saumonbro on 3/4/25.
//

#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H
#include <string>
#include <vector>
#include <Lexer/Token.hpp>


int Levenshtein(const std::string& s1, const std::string& s2);

std::string bold_on();
std::string bold_off();

std::string GetErrorMessage(const Token& token, std::vector<TokenType> expected);

bool IsSimilarToType(const std::string& word);

#endif //ERRORMANAGER_H
