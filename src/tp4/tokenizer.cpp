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

Group* Tokenizer::getGroup()
{
    std::vector<GroupPart*> gpVec;

    for(auto gp = getGroupPart(); gp != nullptr; gp = getGroup())
        gpVec.push_back(gp);

    if(!gpVec.empty())
    {
        Group* retval = new Group();
        retval->groupPartVec = std::move(gpVec);
        return retval;
    }
    else
        return nullptr;
}

GroupPart* Tokenizer::getGroupPart()
{
    GroupPart retval;

    auto befidx = mIdx;

    if((retval.uni.ifSection = getIfSection()) != nullptr)
        retval.tag = GroupPart::IF_SECTION;
    else if((retval.uni.controlLine = getControlLine()) != nullptr)
        retval.tag = GroupPart::CONTROL_LINE;
    else if((retval.uni.textLine = getTextLine()) != nullptr)
        retval.tag = GroupPart::TEXT_LINE;
    else
    {
        mIdx = nextIdx(mIdx);
        if(getTP3Token(mIdx) != nullptr)
        {
            if(mTP3TokenVec[mIdx]->uni.ppToken->tag == PreprocessingToken::PUNCTUATOR)
            {
                if(mTP3TokenVec[mIdx]->uni.ppToken->uni.punctuator->tag == Punctuator::HASH)
                {
                    mIdx++;
                    if((retval.uni.nonDirective = getNonDirective()) != nullptr)
                        retval.tag = GroupPart::NON_DIRECTIVE;
                }
            }
        }
    }

    if(retval.tag != GroupPart::NONE)
        return new GroupPart(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

IfSection* Tokenizer::getIfSection()
{
    IfSection retval;

    if((retval.ifGroup = getIfGroup()) != nullptr)
    {
        
    }
}

PreprocessingFile* Tokenizer::getPreprocessingFile()
{
    PreprocessingFile* retval = new PreprocessingFile();
    retval->group = getGroup();
    return retval;
}

std::size_t Tokenizer::nextIdx(std::size_t idx) const
{
    std::size_t retval = idx;
    for(; idx < mTP3TokenVec.size(); idx++)
    {
        if(mTP3TokenVec[idx]->tag == TP3Token::PP_TOKEN)
            break;
    }

    return idx;
}

TP3Token* Tokenizer::getTP3Token(std::size_t idx) const
{
    return idx < mTP3TokenVec.size() ? mTP3TokenVec[idx] : nullptr;
}