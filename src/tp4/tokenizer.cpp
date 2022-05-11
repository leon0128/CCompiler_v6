#include "tokenizer.hpp"
#include "../lexical_elements.hpp"
#include <iostream>

const std::unordered_map<std::string, int> TP4::Tokenizer::DIRECTIVE_NAME_MAP
    = {{"if", 0},
       {"ifdef", 0},
       {"ifndef", 0},
       {"elif", 0},
       {"else", 0},
       {"endif", 0},
       {"include", 0},
       {"define", 0},
       {"undef", 0},
       {"line", 0},
       {"error", 0}};

TP4::Tokenizer::Tokenizer(const std::vector<TP3Token*>& tp3TokenVec,
                     PreprocessingFile*& ppFile):
    mTP3TokenVec(tp3TokenVec),
    mPPFile(ppFile),
    mIdx(0),
    mIsValid(true)
{
}

bool TP4::Tokenizer::execute()
{
    mPPFile = getPreprocessingFile();

    if(mIdx != mTP3TokenVec.size() &&
       !mTP3TokenVec.empty())
    {
        mIsValid = false;
        std::cout << "TP4 Tokenizer error:\n"
                     "    what: token sequence is not evaluated until the end.\n"
                     "    idx: " << mIdx
                  << std::endl;
    }

    return mIsValid;
}

ControlLine* TP4::Tokenizer::getControlLine()
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
                bool isExsitedLparen = false;
                if(isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::LPAREN))
                {
                    if(tmpidx + 1 == mIdx)
                    {
                        isExsitedLparen = true;
                        if(isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::DOTDOTDOT))
                        {
                            if(isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::RPAREN))
                            {
                                mIdx++;
                                if((retval.uni.sDefineVariableReplacementList.replacementList = getReplacementList()) != nullptr)
                                {
                                    if(isOther(mIdx = nextIdx(mIdx), '\n'))
                                    {
                                        mIdx++;
                                        retval.uni.sDefineVariableReplacementList.identifier = identifier;
                                        retval.tag = ControlLine::DEFINE_VARIABLE_REPLACEMENT_LIST;
                                    }
                                }
                            }
                        }
                        else
                        {
                            IdentifierList* identifierList = getIdentifierList();
                            if(isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::RPAREN))
                            {
                                mIdx++;
                                if((retval.uni.sDefineIdentifierListReplacementList.replacementList = getReplacementList()) != nullptr)
                                {
                                    if(isOther(mIdx = nextIdx(mIdx), '\n'))
                                    {
                                        mIdx++;
                                        retval.uni.sDefineIdentifierListReplacementList.identifier = identifier;
                                        retval.uni.sDefineIdentifierListReplacementList.identifierList = identifierList;
                                        retval.tag = ControlLine::DEFINE_IDENTIFIER_LIST_REPLACEMENT_LIST;
                                    }
                                }
                            }
                            else if(identifierList != nullptr &&
                                    isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::COMMA))
                            {
                                if(isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::DOTDOTDOT))
                                {
                                    if(isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::RPAREN))
                                    {
                                        mIdx++;
                                        if((retval.uni.sDefineIdentifierListVariableReplacementList.replacementList = getReplacementList()) != nullptr)
                                        {
                                            if(isOther(mIdx = nextIdx(mIdx), '\n'))
                                            {
                                                mIdx++;
                                                retval.uni.sDefineIdentifierListVariableReplacementList.identifier = identifier;
                                                retval.uni.sDefineIdentifierListVariableReplacementList.identifierList = identifierList;
                                                retval.tag = ControlLine::DEFINE_IDENTIFIER_LIST_VARIABLE_REPLACEMENT_LIST;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                if(!isExsitedLparen)
                {
                    mIdx = tmpidx + 1;
                    if((retval.uni.sDefineReplacementList.replacementList = getReplacementList()) != nullptr)
                    {
                        if(isOther(mIdx = nextIdx(mIdx), '\n'))
                        {
                            mIdx++;
                            retval.uni.sDefineReplacementList.identifier = identifier;
                            retval.tag = ControlLine::DEFINE_REPLACEMENT_LIST;
                        }
                    }
                }
            }
        }
        else if(isIdentifier(mIdx = nextIdx(mIdx + 1), "undef"))
        {
            if(isIdentifier(mIdx = nextIdx(mIdx + 1)))
            {
                retval.uni.sUndef.identifier = mTP3TokenVec[mIdx]->uni.ppToken->uni.identifier;
                retval.tag = ControlLine::UNDEF;
                mIdx++;
            }
        }
        else if(isIdentifier(mIdx = nextIdx(mIdx + 1), "line"))
        {
            mIdx++;
            if((retval.uni.sLine.ppTokens = getPPTokens()) != nullptr)
            {
                if(isOther(mIdx = nextIdx(mIdx), '\n'))
                {
                    mIdx++;
                    retval.tag = ControlLine::LINE;
                }
            }
        }
        else if(isIdentifier(mIdx = nextIdx(mIdx + 1), "error"))
        {
            mIdx++;
            retval.uni.sError.ppTokens = getPPTokens();
            if(isOther(mIdx = nextIdx(mIdx), '\n'))
            {
                mIdx++;
                retval.tag = ControlLine::ERROR;
            }
        }
        else if(isOther(mIdx = nextIdx(mIdx + 1), '\n'))
        {
            mIdx++;
            retval.tag = ControlLine::NEW_LINE;
        }
    }

    if(retval.tag != ControlLine::NONE)
        return new ControlLine(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

ElifGroup* TP4::Tokenizer::getElifGroup()
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

ElifGroups* TP4::Tokenizer::getElifGroups()
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

ElseGroup* TP4::Tokenizer::getElseGroup()
{
    ElseGroup retval;
    bool isValid = true;
    auto befidx = mIdx;

    if(isPunctuator(mIdx = nextIdx(mIdx), Punctuator::HASH))
    {
        if(isIdentifier(mIdx = nextIdx(mIdx + 1), "else"))
        {
            if(isOther(mIdx = nextIdx(mIdx + 1), '\n'))
            {
                mIdx++;
                retval.group = getGroup();
                isValid = true;
            }
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

Group* TP4::Tokenizer::getGroup()
{
    std::vector<GroupPart*> gpVec;

    for(auto gp = getGroupPart(); gp != nullptr; gp = getGroupPart())
    {
        if(!gpVec.empty())
        {
            if(gpVec.back()->tag == GroupPart::TEXT_LINE &&
               gp->tag == GroupPart::TEXT_LINE)
            {
                PPTokens* pt = nullptr;
                if(gpVec.back()->uni.textLine->ppTokens != nullptr &&
                   gp->uni.textLine->ppTokens != nullptr)
                {
                    gpVec.back()->uni.textLine->ppTokens->ppTokenVec.insert(gpVec.back()->uni.textLine->ppTokens->ppTokenVec.end(), gp->uni.textLine->ppTokens->ppTokenVec.begin(), gp->uni.textLine->ppTokens->ppTokenVec.end());
                    pt = gpVec.back()->uni.textLine->ppTokens;
                }
                else if(gpVec.back()->uni.textLine->ppTokens != nullptr)
                    pt = gpVec.back()->uni.textLine->ppTokens;
                else if(gp->uni.textLine->ppTokens != nullptr)
                    pt = gp->uni.textLine->ppTokens;

                gpVec.back()->uni.textLine->ppTokens = pt;
            }
            else
                gpVec.push_back(gp);
        }
        else
            gpVec.push_back(gp);
    }

    if(!gpVec.empty())
    {
        Group* retval = new Group();
        retval->groupPartVec = std::move(gpVec);
        return retval;
    }
    else
        return nullptr;
}

GroupPart* TP4::Tokenizer::getGroupPart()
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

IdentifierList* TP4::Tokenizer::getIdentifierList()
{
    std::vector<Identifier*> ivec;

    if(isIdentifier(mIdx = nextIdx(mIdx)))
    {
        ivec.push_back(mTP3TokenVec[mIdx]->uni.ppToken->uni.identifier);

        while(isPunctuator(mIdx = nextIdx(mIdx + 1), Punctuator::COMMA))
        {
            if(isIdentifier(mIdx = nextIdx(mIdx + 1)))
                ivec.push_back(mTP3TokenVec[mIdx]->uni.ppToken->uni.identifier);
            else
            {
                mIdx--;
                break;
            }
        }
    }

    if(!ivec.empty())
    {
        IdentifierList* retval = new IdentifierList();
        retval->identifierList = std::move(ivec);
        return retval;
    }
    else
        return nullptr;
}

IfGroup* TP4::Tokenizer::getIfGroup()
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

IfSection* TP4::Tokenizer::getIfSection()
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

NonDirective* TP4::Tokenizer::getNonDirective()
{
    if(isIdentifier(nextIdx(mIdx)))
    {
        if(DIRECTIVE_NAME_MAP.find(mTP3TokenVec[nextIdx(mIdx)]->uni.ppToken->uni.identifier->sequence) == DIRECTIVE_NAME_MAP.end())
            return nullptr;
    }

    NonDirective retval;
    bool isValid = false;
    auto befidx = mIdx;

    if((retval.ppTokens = getPPTokens()) != nullptr)
    {
        if(isOther(mIdx = nextIdx(mIdx), '\n'))
        {
            mIdx++;
            isValid = true;
        }
    }

    if(isValid)
        return new NonDirective(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

PPTokens* TP4::Tokenizer::getPPTokens()
{
    std::vector<PreprocessingToken*> ptvec;
    auto befidx = mIdx;

    for(bool isNewLine = isOther(mIdx, '\n'); !isNewLine && mIdx < mTP3TokenVec.size(); isNewLine = isOther(mIdx += 1, '\n'))
    {
        if(mTP3TokenVec[mIdx]->tag == TP3Token::PP_TOKEN)
            ptvec.push_back(mTP3TokenVec[mIdx]->uni.ppToken);
    }
    
    if(!ptvec.empty())
    {
        PPTokens* retval = new PPTokens();
        retval->ppTokenVec = std::move(ptvec);
        return retval;
    }
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

PreprocessingFile* TP4::Tokenizer::getPreprocessingFile()
{
    PreprocessingFile* retval = new PreprocessingFile();
    retval->group = getGroup();
    return retval;
}

ReplacementList* TP4::Tokenizer::getReplacementList()
{
    ReplacementList* retval = new ReplacementList();
    retval->tp3Tokens = getTP3Tokens();
    return retval;
}

TP3Tokens* TP4::Tokenizer::getTP3Tokens()
{
    std::vector<TP3Token*> ttvec;
    auto befidx = mIdx;

    for(bool isNewLine = isOther(mIdx, '\n'); !isNewLine && mIdx < mTP3TokenVec.size(); isNewLine = isOther(mIdx += 1, '\n'))
        ttvec.push_back(mTP3TokenVec[mIdx]);
    
    if(!ttvec.empty())
    {
        TP3Tokens* retval = new TP3Tokens();
        retval->tp3TokenVec = std::move(ttvec);
        return retval;
    }
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

TextLine* TP4::Tokenizer::getTextLine()
{
    if(isPunctuator(nextIdx(mIdx), Punctuator::HASH))
        return nullptr;

    TextLine retval;
    auto befidx = mIdx;
    bool isValid = false;

    retval.ppTokens = getPPTokens();
    if(isOther(mIdx = nextIdx(mIdx), '\n'))
    {
        mIdx++;
        isValid = true;
    }

    if(isValid)
        return new TextLine(retval);
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

bool TP4::Tokenizer::isPunctuator(std::size_t idx, int tag) const
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

bool TP4::Tokenizer::isIdentifier(std::size_t idx, std::string ident) const
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

bool TP4::Tokenizer::isOther(std::size_t idx, char c) const
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

std::size_t TP4::Tokenizer::nextIdx(std::size_t idx) const
{
    for(; idx < mTP3TokenVec.size(); idx++)
    {
        if(mTP3TokenVec[idx]->tag == TP3Token::PP_TOKEN)
            break;
    }

    return idx;
}

TP3Token* TP4::Tokenizer::getTP3Token(std::size_t idx) const
{
    return idx < mTP3TokenVec.size() ? mTP3TokenVec[idx] : nullptr;
}