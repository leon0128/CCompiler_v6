#include "converter.hpp"
#include "../lexical_elements.hpp"

TP7::Converter::Converter(const std::vector<PreprocessingToken*>& ptvec,
                          std::vector<Token*>& tvec) noexcept(true):
    mPPTokenVec(ptvec),
    mTokenVec(tvec),
    mIsValid(true)
{
}