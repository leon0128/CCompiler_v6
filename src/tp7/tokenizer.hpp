#pragma once

#include <vector>

struct ConstantExpression;
struct Token;
struct TranslationUnit;

namespace TP7
{

class Tokenizer
{
public:
    Tokenizer(const std::vector<Token*>&) noexcept(true);

    bool execute(TranslationUnit*&){return mIsVaild;}
    bool execute(ConstantExpression*&){return mIsVaild;} // for TP4

private:
    const std::vector<Token*>& mTokenVec;

    bool mIsVaild;
};

}