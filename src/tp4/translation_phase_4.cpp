#include "translation_phase_4.hpp"
#include "tokenizer.hpp"
#include "../lexical_elements.hpp"

TP4::TP4(const std::vector<TP3Token*>& tp3TokenVec,
         std::vector<PreprocessingToken*>& ppTokenVec):
    mTP3TokenVec(tp3TokenVec),
    mPPTokenVec(ppTokenVec),
    mPPFile(nullptr),
    mIsValid(true)
{
}

bool TP4::execute()
{
    Tokenizer tokenizer(mTP3TokenVec, mPPFile);
    mIsValid = tokenizer.execute();

    return mIsValid;
}