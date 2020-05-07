#include "translation_phase_3.hpp"
#include "../lexical_elements.hpp"
#include <iostream>

TP3::TP3(const std::string& src,
         std::vector<TP3Token*>& tp3TokenVec):
    mSrc(src),
    mIdx(),
    mTP3TokenVec(tp3TokenVec),
    mIsValid(true)
{
}

bool TP3::execute()
{
    for(auto t = getTP3Token(); t != nullptr && mIsValid; t = getTP3Token())
        mTP3TokenVec.push_back(t);

    if(mIdx != mSrc.size() && mIsValid)
    {
        mIsValid = false;
        std::cout << "TP3 error:\n"
                     "    what: source code cannot be evaluated to the end.\n"
                     "    idx: " << mIdx
                  << std::endl;
    }

    return mIsValid;
}

TP3Token* TP3::getTP3Token()
{
    TP3Token retval;
    auto befidx = mIdx;
    bool isValid = true;

    if((retval.uni.numSpace = getWhiteSpace()) != 0)
        retval.tag = TP3Token::WHITE_SPACE;
    else if((retval.uni.ppToken = getPPToken()) != nullptr)
        retval.tag = TP3Token::PP_TOKEN;
    else
        isValid = false;

    if(isValid)
        return new TP3Token(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

unsigned int TP3::getWhiteSpace()
{
    
}