#include "tokenizer.hpp"
#include "../lexical_elements.hpp"

TP7::Tokenizer::Tokenizer(const std::vector<Token*>& tvec) noexcept(true):
    mTokenVec(tvec),
    mIsVaild(true)
{
}