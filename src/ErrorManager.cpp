//
// Created by saumonbro on 3/4/25.
//

#include <ErrorManager.h>
#include <sstream>
#include <Lexer/Lexer.h>
#include <Lexer/Token.hpp>

unsigned long min(unsigned long a, unsigned long b, unsigned long c)
{
    if (a <= b && a <= c)
    {
        return a;
    }
    return (b <= c && b <= a) ? b : c;
}

unsigned long levenshtein_aux(const std::string& s1, const std::string& s2, unsigned long i,  unsigned long j)
{
    if (i == 0)
    {
        return j;
    }
    if (j == 0)
    {
        return i;
    }

    if (s1[i - 1] == s2[j - 1])
    {
        return levenshtein_aux(s1, s2, i - 1, j - 1);
    }

    unsigned long del = levenshtein_aux(s1, s2, i - 1, j);
    unsigned long ins = levenshtein_aux(s1, s2, i, j - 1);
    unsigned long sub = levenshtein_aux(s1, s2, i - 1, j - 1);

    return min(del, ins, sub) + 1;
}

class bold {
    std::string_view const &s;
public:
    bold(std::string_view const &s) : s(s) {}

    friend std::ostream &operator<<(std::ostream &os, bold const &b) {
        os << "\x1b[1m" << b.s << "\x1b[0m";
        return os;
    }
};


std::string bold_on()
{
    std::stringstream os;
    os << "\x1b[1m";
    return os.str();
}

std::string bold_off()
{
    std::stringstream os;
    os << "\x1b[0m";
    return os.str();
}


int Levenshtein(const std::string& s1, const std::string& s2)
{
    return static_cast<int>(levenshtein_aux(s1, s2, s1.size(), s2.size()));
}

static bool isSpecialCharacter(char c)
{
    static const std::string s("{(});,");
    return s.find(c) != std::string::npos;
}

std::string GetTypeErrorMessage(const Token& token)
{
    const std::string& token_data = token.Data;

    std::string error = "Unknown type name : ";

    std::string hint = "Did you forget the type ?";

    if (isSpecialCharacter(token.Data[0]))
    {
        hint = "Unexpected special character: '" + token.Data + "'";
    }
    else if (Levenshtein(token_data, "string") <= 2)
    {
        hint = "Did you mean 'string' ?";
    }
    else if (Levenshtein(token_data, "int") <= 2)
    {
        hint = "Did you mean 'int' ?";
    }
    return (error + "'" + token_data + "'. " + hint);
}

std::string GetErrorMessage(const Token& token, std::vector<TokenType> expected)
{
    std::string error;
    if (expected.size() == 1)
    {
        error = "Expected: " + Token::tokenToString(expected[0]);
    }
    else
    {
        error = "Expected one of:";
        for (const auto tok : expected)
        {
            error += (" " + Token::tokenToString(tok));
        }
    }

    if (expected[0] == TOKEN_STRING_TYPE || expected[0] == TOKEN_INT_TYPE)
    {
        return GetTypeErrorMessage(token);
    }

    return error + ". But got " + Token::tokenToString(token.Type);
}




bool IsSimilarToType(const std::string& word)
{
    return Levenshtein(word, "string") <= 3 || Levenshtein(word, "int") <= 2;
}
