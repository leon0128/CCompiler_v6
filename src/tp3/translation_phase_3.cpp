#include "translation_phase_3.hpp"
#include "../lexical_elements.hpp"

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
    
}