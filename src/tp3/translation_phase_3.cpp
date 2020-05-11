#include "translation_phase_3.hpp"
#include "../lexical_elements.hpp"
#include <iostream>
#include <fstream>
#include <utility>

unsigned int TP3::NUM_EXEC = 0;
const std::unordered_map<std::string, int> TP3::PUNCTUATOR_MAP
    = {{"[", Punctuator::RSQUALE_PAREN},
       {"]", Punctuator::LSQUALE_PAREN},
       {"(", Punctuator::RPAREN},
       {")", Punctuator::LPAREN},
       {"{", Punctuator::LBLOCK_PAREN},
       {"}", Punctuator::RBLOCK_PAREN},
       {".", Punctuator::DOT},
       {"->", Punctuator::ARROW},
       {"++", Punctuator::INCLEMENT},
       {"--", Punctuator::DECLEMENT},
       {"&", Punctuator::BITWISE_AND},
       {"*", Punctuator::ASTERISK},
       {"+", Punctuator::PLUS},
       {"-", Punctuator::MINUS},
       {"~", Punctuator::TILDE},
       {"!", Punctuator::NOT},
       {"/", Punctuator::SLASH},
       {"%", Punctuator::PERCENT},
       {"<<", Punctuator::LSHIFT},
       {">>", Punctuator::RSHIFT},
       {"<", Punctuator::LESS},
       {">", Punctuator::GREATER},
       {"<=", Punctuator::LESS_EQUAL},
       {">=", Punctuator::GREATER_EQUAL},
       {"==", Punctuator::EQUAL},
       {"!=", Punctuator::NOT_EQUAL},
       {"^", Punctuator::BITWISE_EXCLUSIVE_OR},
       {"|", Punctuator::BITWISE_INCLUSIVE_OR},
       {"&&", Punctuator::LOGICAL_AND},
       {"||", Punctuator::LOGICAL_OR},
       {"?", Punctuator::QUESTION},
       {":", Punctuator::COLON},
       {";", Punctuator::SEMICOLON},
       {"...", Punctuator::DOTDOTDOT},
       {"=", Punctuator::ASSIGN},
       {"*=", Punctuator::ASTERISK_ASSIGN},
       {"/=", Punctuator::SLASH_ASSIGN},
       {"%=", Punctuator::PERCENT_ASSIGN},
       {"+=", Punctuator::PLUS_ASSING},
       {"-=", Punctuator::MINUS_ASSIGN},
       {"<<=", Punctuator::LSHIFT_ASSIGN},
       {">>=", Punctuator::RSHIFT_ASSIGN},
       {"&=", Punctuator::BITWISE_AND_ASSIGN},
       {"^=", Punctuator::BITWISE_EXCLUSIVE_OR_ASSING},
       {"|=", Punctuator::BITWISE_INCLUSIVE_OR_ASSING},
       {",", Punctuator::COMMA},
       {"#", Punctuator::HASH},
       {"##", Punctuator::HASHHASH},
       {"<:", Punctuator::LSQUALE_PAREN},
       {":>", Punctuator::RSQUALE_PAREN},
       {"<%", Punctuator::LBLOCK_PAREN},
       {"%>", Punctuator::RBLOCK_PAREN},
       {"%:", Punctuator::HASH},
       {"%:%:", Punctuator::HASHHASH}};

const std::unordered_map<char, int> TP3::ESCAPE_SEQUENCE_MAP
    = {{'\'', SimpleEscapeSequence::SINGLE_QUOTATION},
       {'"', SimpleEscapeSequence::DOUBLE_QUOTATION},
       {'?', SimpleEscapeSequence::QUESTION},
       {'\\', SimpleEscapeSequence::BACKSLASH},
       {'a', SimpleEscapeSequence::ALERT},
       {'b', SimpleEscapeSequence::BACKSPACE},
       {'f', SimpleEscapeSequence::FORM_FEED},
       {'n', SimpleEscapeSequence::NEW_LINE},
       {'r', SimpleEscapeSequence::CARRIAGE_RETURN},
       {'t', SimpleEscapeSequence::HORIZONTAL_TAB},
       {'v', SimpleEscapeSequence::VERTICAL_TAB}};

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
    for(auto t = getTP3Token(); mIdx < mSrc.size(); t = getTP3Token())
        mTP3TokenVec.push_back(t);

    if(mIdx != mSrc.size())
    {
        mIsValid = false;
        std::cout << "TP3 error:\n"
                     "    what: source code cannot be evaluated to the end.\n"
                     "    idx: " << mIdx
                  << std::endl;
    }

    if(mIsValid)
        outputResult();

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
    unsigned int retval = 0;
    bool isContinued = true;

    while(isContinued)
    {
        char c = getChar(mIdx);
        if(c == ' ' ||
           c == '\t' ||
           c == '\v' ||
           c == '\f')
        {
            retval++;
            mIdx++;
        }
        else
        {
            char nc = getChar(mIdx + 1);
            if(c == '/' &&
               nc == '*')
            {
                auto endpos = mSrc.find("*/", mIdx + 2);
                if(endpos != std::string::npos)
                {
                    retval++;
                    mIdx = endpos + 2;
                }
                else
                {
                    isContinued = false;
                    std::cout << "TP3 warning:\n"
                                 "    what: /**/ comment is not closed.\n"
                                 "    idx: " << mIdx
                              << std::endl;
                }
            }
            else if(c == '/' &&
                    nc == '/')
            {
                auto endpos = mSrc.find('\n', mIdx + 2);
                if(endpos != std::string::npos)
                {
                    isContinued = false;
                    retval++;
                    mIdx = endpos;
                }
                else
                {
                    isContinued = false;
                    retval++;
                    mIdx = mSrc.size();
                }
            }
            else
                isContinued = false;
        }
    }

    return retval;
}

CChar* TP3::getCChar()
{
    CChar retval;

    char c = getChar(mIdx);
    if(c != '\'' &&
       c != '\\' &&
       c != '\n')
    {
        mIdx++;
        retval.uni.anyMember = c;
        retval.tag = CChar::ANY_MEMBER;
    }
    else if((retval.uni.escapeSequence = getEscapeSequence()) != nullptr)
        retval.tag = CChar::ESCAPE_SEQUENCE;

    return retval.tag != CChar::NONE ? new CChar(retval) : nullptr;
}

CCharSequence* TP3::getCCharSequence()
{
    std::vector<CChar*> ccVec;

    for(auto cc = getCChar(); cc != nullptr; cc = getCChar())
        ccVec.push_back(cc);
    
    if(!ccVec.empty())
    {
        CCharSequence* retval = new CCharSequence();
        retval->cCharVec = std::move(ccVec);
        return retval;
    }
    else
        return nullptr;
}

CharacterConstant* TP3::getCharacterConstant()
{
    CCharSequence* sequence = nullptr;
    CharacterConstant::Tag tag = CharacterConstant::NONE;
    bool isValid = true;
    auto befidx = mIdx;

    char c = getChar(mIdx);
    char nc = getChar(mIdx + 1);

    if(c == '\'')
    {
        tag = CharacterConstant::C_CHAR_SEQUENCE;
        mIdx++;
    }
    else if(c == 'L' &&
            nc == '\'')
    {
        tag = CharacterConstant::L_C_CHAR_SEQUENCE;
        mIdx += 2;
    }
    else if(c == 'u' &&
            nc == '\'')
    {
        tag = CharacterConstant::u_C_CHAR_SEQUENCE;
        mIdx += 2;
    }
    else if(c == 'U' &&
            nc == '\'')
    {
        tag = CharacterConstant::U_C_CHAR_SEQUENCE;
        mIdx += 2;
    }
    else
        isValid = false;

    if(isValid)
    {
        if((sequence = getCCharSequence()) != nullptr)
        {
            if(getChar(mIdx) == '\'')
                mIdx++;
            else
                isValid = false;
        }
        else
            isValid = false;
    }

    if(isValid)
    {
        CharacterConstant* retval = new CharacterConstant();
        retval->tag = tag;
        retval->cCharSequence = sequence;
        return retval;
    }
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

EscapeSequence* TP3::getEscapeSequence()
{
    EscapeSequence retval;

    if((retval.uni.simpleEscapeSequence = getSimpleEscapeSequence()) != nullptr)
        retval.tag = EscapeSequence::SIMPLE_ESCAPE_SEQUENCE;
    else if((retval.uni.octalEscapeSequence = getOctalEscapeSequence()) != nullptr)
        retval.tag = EscapeSequence::OCTAL_ESCAPE_SEQUENCE;
    else if((retval.uni.hexadecimalEscapeSequence = getHexadecimalEscapeSequence()) != nullptr)
        retval.tag = EscapeSequence::HEXADECIMAL_ESCAPE_SEQUENCE;
    
    return retval.tag != EscapeSequence::NONE ? new EscapeSequence(retval) : nullptr;
}

HeaderName* TP3::getHeaderName()
{
    PreprocessingToken *bef = nullptr, *aft = nullptr;
    for(auto i = mTP3TokenVec.size(); i > 0; i--)
    {
        if(mTP3TokenVec[i - 1]->tag == TP3Token::PP_TOKEN)
        {
            if(aft != nullptr)
                aft = mTP3TokenVec[i - 1]->uni.ppToken;
            else
            {
                bef = mTP3TokenVec[i - 1]->uni.ppToken;
                break;
            }
        }
    }
    if(bef == nullptr || aft == nullptr)
        return nullptr;

    if(bef->tag == PreprocessingToken::PUNCTUATOR)
    {
        if(bef->uni.punctuator->tag != Punctuator::HASH)
            return nullptr;
    }
    else
        return nullptr;

    if(aft->tag == PreprocessingToken::IDENTIFIER)
    {
        if(aft->uni.identifier->sequence != "include" &&
           aft->uni.identifier->sequence != "pragma")
            return nullptr;
    }
    else
        return nullptr;

    std::string sequence;
    auto tag = HeaderName::NONE;
    auto befidx = mIdx;
    bool isValid = true;

    if(getChar(mIdx) == '<')
    {
        mIdx++;
        for(char c = mSrc[mIdx]; mIdx < mSrc.size(); mIdx++)
        {
            if(c != '>' &&
               c != '\n')
                sequence.push_back(c);
            else
                break;
        }

        if(getChar(mIdx) == '>')
        {
            mIdx++;
            tag = HeaderName::H_CHAR_SEQUENCE;
        }
        else
            isValid = false;
    }
    else if(getChar(mIdx) == '"')
    {
        mIdx++;
        for(char c = mSrc[mIdx]; mIdx < mSrc.size(); mIdx++)
        {
            if(c != '"' &&
               c != '\n')
                sequence.push_back(c);
            else
                break;
        }

        if(getChar(mIdx) == '"')
        {
            mIdx++;
            tag = HeaderName::Q_CHAR_SEQUENCE;
        }
        else
            isValid = false;
    }
    else
        isValid = false;

    if(isValid)
    {
        HeaderName* retval = new HeaderName();
        retval->sequence = std::move(sequence);
        retval->tag = tag;
        return retval;
    }
    else
    {
        mIdx = befidx;
        return nullptr;
    }
}

Identifier* TP3::getIdentifier()
{
    std::string sequence;
    
    char c = getChar(mIdx);
    if(c == '_' ||
       (c >= 'a' && c <= 'z') ||
       (c >= 'A' && c <= 'Z'))
    {
        sequence.push_back(c);
        mIdx++;

        for(c = getChar(mIdx);
            (c == '_') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9');
            c = getChar(mIdx))
        {
            sequence.push_back(c);
            mIdx++;
        }
    }

    if(!sequence.empty())
    {
        Identifier* retval = new Identifier();
        retval->sequence = std::move(sequence);
        return retval;
    }
    else
        return nullptr;
}

PPNumber* TP3::getPPNumber()
{
    std::string sequence;

    char c = getChar(mIdx);
    if(c >= '0' && c <= '9')
    {
        sequence.push_back(c);
        mIdx++;
    }
    else if(c == '.')
    {
        c = getChar(mIdx + 1);
        if(c >= '0' && c <= '9')
        {
            sequence.push_back('.');
            sequence.push_back(c);
            mIdx += 2;
        }
    }

    if(!sequence.empty())
    {
        for(c = getChar(mIdx); c != 0; c = getChar(mIdx))
        {
            if(c == 'e' ||
               c == 'E' ||
               c == 'p' ||
               c == 'P')
            {
                char nc = getChar(mIdx + 1);
                if(nc == '+' ||
                   nc == '-')
                {
                    sequence.push_back(c);
                    sequence.push_back(nc);
                    mIdx += 2;
                }
                else
                {
                    sequence.push_back(c);
                    mIdx++;
                }
            }
            else if((c >= '0' && c <= '9') ||
                    (c >= 'a' && c <= 'z') ||
                    (c >= 'A' && c <= 'Z') ||
                    c == '_' ||
                    c == '.')
            {
                sequence.push_back(c);
                mIdx++;
            }
            else
                break;
        }
    }

    if(!sequence.empty())
    {
        PPNumber* retval = new PPNumber();
        retval->sequence = std::move(sequence);
        return retval;
    }
    else
        return nullptr;
}

PreprocessingToken* TP3::getPPToken()
{
    PreprocessingToken retval;

    if((retval.uni.headerName = getHeaderName()) != nullptr)
        retval.tag = PreprocessingToken::HEADER_NAME;
    else if((retval.uni.characterConstant = getCharacterConstant()) != nullptr)
        retval.tag = PreprocessingToken::CHARACTER_CONSTANT;        
    else if((retval.uni.stringLiteral = getStringLiteral()) != nullptr)
        retval.tag = PreprocessingToken::STRING_LITERAL;        
    else if((retval.uni.identifier = getIdentifier()) != nullptr)
        retval.tag = PreprocessingToken::IDENTIFIER;
    else if((retval.uni.ppNumber = getPPNumber()) != nullptr)
        retval.tag = PreprocessingToken::PP_NUMBER;
    else if((retval.uni.punctuator = getPunctuator()) != nullptr)
        retval.tag = PreprocessingToken::PUNCTUATOR;
    else
    {
        retval.uni.other = getChar(mIdx++);
        retval.tag = PreprocessingToken::OTHER;
    }

    return new PreprocessingToken(retval);
}

Punctuator* TP3::getPunctuator()
{
    Punctuator::Tag tag = Punctuator::NONE;

    auto iter = PUNCTUATOR_MAP.find(mSrc.substr(mIdx, 4));
    if(iter != PUNCTUATOR_MAP.end())
    {
        tag = static_cast<Punctuator::Tag>(iter->second);
        mIdx += iter->first.size();
    }
    else if((iter = PUNCTUATOR_MAP.find(mSrc.substr(mIdx, 3))) != PUNCTUATOR_MAP.end())
    {
        tag = static_cast<Punctuator::Tag>(iter->second);
        mIdx += iter->first.size();
    }
    else if((iter = PUNCTUATOR_MAP.find(mSrc.substr(mIdx, 2))) != PUNCTUATOR_MAP.end())
    {
        tag = static_cast<Punctuator::Tag>(iter->second);
        mIdx += iter->first.size();
    }
    else if((iter = PUNCTUATOR_MAP.find(mSrc.substr(mIdx, 1))) != PUNCTUATOR_MAP.end())
    {
        tag = static_cast<Punctuator::Tag>(iter->second);
        mIdx += iter->first.size();
    }

    if(tag != Punctuator::NONE)
    {
        Punctuator* retval = new Punctuator();
        retval->tag = tag;
        return retval;
    }
    else
        return nullptr;
}

SimpleEscapeSequence* TP3::getSimpleEscapeSequence()
{
    if(getChar(mIdx) != '\\')
        return nullptr;
    
    auto iter = ESCAPE_SEQUENCE_MAP.find(getChar(mIdx + 1));
    if(iter != ESCAPE_SEQUENCE_MAP.end())
    {
        mIdx += 2;
        SimpleEscapeSequence* retval = new SimpleEscapeSequence();
        retval->tag = static_cast<SimpleEscapeSequence::Tag>(iter->second);
        return retval;
    }
    else
        return nullptr;
}

void TP3::outputResult() const
{
    std::string resFilename = "res/tp3_";
    resFilename += std::to_string(NUM_EXEC);
    resFilename += ".log";

    std::ofstream fstr(resFilename.c_str(), std::ios::out);

    if(fstr.is_open())
    {
        fstr << "===== Translation phase 3 =====\n"
                ">> NUM_EXEC: " << NUM_EXEC << "\n"
                ">> sequence size: " << mTP3TokenVec.size() << "\n"
                ">> result excluding white-space:"
             << std::endl;

        for(std::size_t i = 0; i < mTP3TokenVec.size(); i++)
        {
            switch(mTP3TokenVec[i]->tag)
            {
                case(TP3Token::PP_TOKEN):
                {
                    std::string str;
                    switch(mTP3TokenVec[i]->uni.ppToken->tag)
                    {
                        case(PreprocessingToken::HEADER_NAME):
                            str += mTP3TokenVec[i]->uni.ppToken->uni.headerName->sequence;
                            break;
                        case(PreprocessingToken::IDENTIFIER):
                            str += mTP3TokenVec[i]->uni.ppToken->uni.identifier->sequence;
                            break;
                        case(PreprocessingToken::PP_NUMBER):
                            str += mTP3TokenVec[i]->uni.ppToken->uni.ppNumber->sequence;
                            break;
                        case(PreprocessingToken::CHARACTER_CONSTANT):
                            str += "content display is not implemented";
                            break;
                        case(PreprocessingToken::STRING_LITERAL):
                            str += "content display is not implemented";
                            break;
                        case(PreprocessingToken::PUNCTUATOR):
                            str += std::to_string(static_cast<int>(mTP3TokenVec[i]->uni.ppToken->uni.punctuator->tag));
                            break;
                        case(PreprocessingToken::OTHER):
                            str += std::to_string(static_cast<unsigned int>(mTP3TokenVec[i]->uni.ppToken->uni.other));
                            break;
                        
                        default:
                            unexpectedEnumerationError("PreprocessingToken", mTP3TokenVec[i]->uni.ppToken->tag);
                            break;
                    }

                    fstr << "[idx: " << i << ", tag: " << mTP3TokenVec[i]->uni.ppToken->tag << "]\n"
                            "    " << str
                         << std::endl;
                    break;
                }
                case(TP3Token::WHITE_SPACE):
                    break;

                default:
                    unexpectedEnumerationError("TP3Token", mTP3TokenVec[i]->tag);
                    break;
            }
        }
    }
    else
    {
        std::cout << "TP3 warning:\n"
                     "    what: TP3 log file cannot be created because log file cannot be opened.\n"
                     "    filename: " << resFilename
                  << std::endl;
    }
}