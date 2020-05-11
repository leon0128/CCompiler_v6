#include "tokenizer.hpp"
#include "../lexical_elements.hpp"
#include <iostream>

Tokenizer::Tokenizer(const std::vector<TP3Token*>& tp3TokenVec,
                     PreprocessingFile*& ppFile):
    mTP3TokenVec(tp3TokenVec),
    mPPFile(ppFile),
    mIsValid(true)
{
}

bool Tokenizer::execute()
{
    mPPFile = getPreprocessingFile();

    return mIsValid;
}

PreprocessingFile* Tokenizer::getPreprocessingFile()
{
    PreprocessingFile* retval = new PreprocessingFile();
    retval->group = getGroup();
    return retval;
}

void Tokenizer::proceedIdx()
{
    for(std::size_t i = mIdx; i < mTP3TokenVec.size(); i++)
    {
        if(mTP3TokenVec[i]->tag == TP3Token::PP_TOKEN)
            break;
    }
}