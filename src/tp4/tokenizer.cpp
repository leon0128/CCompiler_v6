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

ControlLine* Tokenizer::getControlLine()
{
    ControlLine retval;
    auto befidx = mIdx;

    if(isPunctuator(mIdx = nextIdx(mIdx), Punctuator::HASH))
    {
        if(isIdentifier(mIdx = nextIdx(mIdx + 1), "include"))
        {
            mIdx++;
            if((retval.uni.sInclude.ppTokens = getPPTokens()) != nullptr)
            {
                if(isOther(mIdx = nextIdx(mIdx), '\n'))
                {
                    mIdx++;
                    retval.tag = ControlLine::INCLUDE;
                }
            }
        }
        else if(isIdentifier(mIdx = nextIdx(mIdx + 1), "define"))
        {
            if(isIdentifier(mIdx = nextIdx(mIdx + 1)))
            {
                Identifier* identifier = mTP3TokenVec[mIdx]->uni.ppToken->uni.identifier;
                auto tmpidx = mIdx;
                if(isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::LPAREN))
                {
                    if(tmpidx + 1 == mIdx)
                    {
                        
                    }
                }
            }
        }
    }
}

ElifGroup* Tokenizer::getElifGroup()
{
    ElifGroup retval;
    bool isValid = false;
    auto befidx = mIdx;

    if(isPunctuator(mIdx = nextIdx(mIdx), Punctuator::HASH))
    {
        if(isIdentifier(mIdx = nextIdx(mIdx + 1), "elif"))
        {
            mIdx++;
            if((retval.ppTokens = getPPTokens()) != nullptr)
            {
                if(isOther(mIdx = nextIdx(mIdx), '\n'))
                {
                    mIdx++;
                    retval.group = getGroup();
                    isValid = true;
                }
            }
        }
    }

    if(isValid)
        return new ElifGroup(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

ElifGroups* Tokenizer::getElifGroups()
{
    std::vector<ElifGroup*> egvec;

    for(auto eg = getElifGroup(); eg != nullptr; eg = getElifGroup())
        egvec.push_back(eg);
    
    if(!egvec.empty())
    {
        ElifGroups* retval = new ElifGroups();
        retval->elifGroupVec = std::move(egvec);
        return retval;
    }
    else
        return nullptr;
}

ElseGroup* Tokenizer::getElseGroup()
{
    ElseGroup retval;
    bool isValid = true;
    auto befidx = mIdx;

    if(isPunctuator(mIdx = nextIdx(mIdx), Punctuator::HASH))
    {
        if(isIdentifier(mIdx = nextIdx(mIdx + 1), '\n'))
        {
            mIdx++;
            retval.group = getGroup();
            isValid = true;
        }
    }

    if(isValid)
        return new ElseGroup(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
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
        if(isPunctuator(mIdx, Punctuator::HASH))
        {
            mIdx++;
            if((retval.uni.nonDirective = getNonDirective()) != nullptr)
                retval.tag = GroupPart::NON_DIRECTIVE;
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

IfGroup* Tokenizer::getIfGroup()
{
    IfGroup retval;
    auto befidx = mIdx;

    if(isPunctuator(mIdx = nextIdx(mIdx), Punctuator::HASH))
    {
        if(isIdentifier(mIdx = nextIdx(mIdx + 1), "if"))
        {
            mIdx++;
            if((retval.uni.sIf.ppTokens = getPPTokens()) != nullptr)
            {
                if(isOther(mIdx = nextIdx(mIdx), '\n'))
                {
                    mIdx++;
                    retval.uni.sIf.group = getGroup();
                    retval.tag = IfGroup::IF;
                }
            }
        }
        else if(isIdentifier(mIdx = nextIdx(mIdx + 1), "ifdef"))
        {
            if(isIdentifier(mIdx = nextIdx(mIdx + 1)))
            {
                retval.uni.sIfdef.identifier = mTP3TokenVec[mIdx]->uni.ppToken->uni.identifier;
                if(isOther(mIdx = nextIdx(mIdx + 1), '\n'))
                {
                    mIdx++;
                    retval.uni.sIfdef.group = getGroup();
                    retval.tag = IfGroup::IFDEF;
                }
            }
        }
        else if(isIdentifier(mIdx = nextIdx(mIdx + 1), "ifndef"))
        {
            if(isIdentifier(mIdx = nextIdx(mIdx + 1)))
            {
                retval.uni.sIfndef.identifier = mTP3TokenVec[mIdx]->uni.ppToken->uni.identifier;
                if(isOther(mIdx = nextIdx(mIdx + 1), '\n'))
                {
                    mIdx++;
                    retval.uni.sIfndef.group = getGroup();
                    retval.tag = IfGroup::IFNDEF;
                }
            }
        }
    }

    if(retval.tag != IfGroup::NONE)
        return new IfGroup(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

IfSection* Tokenizer::getIfSection()
{
    IfSection retval;

    bool isValid = false;
    auto befidx = mIdx;

    if((retval.ifGroup = getIfGroup()) != nullptr)
    {
        retval.elifGroups = getElifGroups();
        retval.elseGroup = getElseGroup();

        if(isPunctuator(mIdx = nextIdx(mIdx), Punctuator::HASH))
        {
            if(isIdentifier(mIdx = nextIdx(mIdx + 1), "endif"))
            {
                if(isOther(mIdx = nextIdx(mIdx + 1), '\n'))
                {
                    mIdx++;
                    isValid = true;
                }
            }
        }
    }

    if(isValid)
        return new IfSection(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

PreprocessingFile* Tokenizer::getPreprocessingFile()
{
    PreprocessingFile* retval = new PreprocessingFile();
    retval->group = getGroup();
    return retval;
}

bool Tokenizer::isPunctuator(std::size_t idx, int tag) const
{
    if(getTP3Token(idx) == nullptr)
        return false;

    if(mTP3TokenVec[idx]->tag != TP3Token::PP_TOKEN)
        return false;
    
    if(mTP3TokenVec[idx]->uni.ppToken->tag != PreprocessingToken::PUNCTUATOR)
        return false;
    
    if(static_cast<Punctuator::Tag>(tag) != Punctuator::NONE)
        return static_cast<Punctuator::Tag>(tag) == mTP3TokenVec[idx]->uni.ppToken->uni.punctuator->tag;
    else
        return true;
}

bool Tokenizer::isIdentifier(std::size_t idx, std::string ident) const
{
    if(getTP3Token(idx) == nullptr)
        return false;
    
    if(mTP3TokenVec[idx]->tag != TP3Token::PP_TOKEN)
        return false;
    
    if(mTP3TokenVec[idx]->uni.ppToken->tag != PreprocessingToken::IDENTIFIER)
        return false;
    
    if(!ident.empty())
        return mTP3TokenVec[idx]->uni.ppToken->uni.identifier->sequence == ident;
    else
        return true;
}

bool Tokenizer::isOther(std::size_t idx, char c) const
{
    if(getTP3Token(idx) == nullptr)
        return false;
    
    if(mTP3TokenVec[idx]->tag != TP3Token::PP_TOKEN)
        return false;
    
    if(mTP3TokenVec[idx]->uni.ppToken->tag != PreprocessingToken::OTHER)
        return false;
    
    if(c != 0)
        return mTP3TokenVec[idx]->uni.ppToken->uni.other == c;
    else
        return true;
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