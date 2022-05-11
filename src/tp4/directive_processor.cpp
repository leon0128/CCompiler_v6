#include "directive_processor.hpp"
#include "macro.hpp"
#include "calculator.hpp"
#include "integer.hpp"
#include "../lexical_elements.hpp"
#include "../global_data.hpp"
#include "../compiler.hpp"
#include "../tp3/translation_phase_3.hpp"
#include "../tp7/converter.hpp"
#include "../tp7/tokenizer.hpp"
#include <fstream>

std::unordered_map<std::string, TP4::Macro*> TP4::DirectiveProcessor::MACRO_MAP;
const std::vector<std::string> TP4::DirectiveProcessor::SYSTEM_INCLUDE_PATH_VEC
    = {"/usr/lib/gcc/x86_64-linux-gnu/9/include/",
       "/usr/local/include/",
       "/usr/include/x86_64-linux-gnu/",
       "/usr/include/"};

TP4::DirectiveProcessor::DirectiveProcessor(const PreprocessingFile* const ppFile,
                                       std::vector<PreprocessingToken*>& ppTokenVec) noexcept(true):
    mPPFile(ppFile),
    mPPTokenVec(ppTokenVec),
    mUsedMacro(),
    mIsValid(true)
{
}

bool TP4::DirectiveProcessor::execute()
{
    proc(mPPFile);

    return mIsValid;
}

void TP4::DirectiveProcessor::proc(const ControlLine* const controlLine)
{
    if(!mIsValid)
        return;

    switch(controlLine->tag)
    {
        case(ControlLine::INCLUDE):
            ctrlInclude(controlLine);
            break;
        case(ControlLine::DEFINE_REPLACEMENT_LIST):
            ctrlDefine(controlLine);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LIST_REPLACEMENT_LIST):
            ctrlDefineIdentifierList(controlLine);
            break;
        case(ControlLine::DEFINE_VARIABLE_REPLACEMENT_LIST):
            ctrlDefineVariable(controlLine);
            break;
        case(ControlLine::DEFINE_IDENTIFIER_LIST_VARIABLE_REPLACEMENT_LIST):
            ctrlDefineIdentifierListVariable(controlLine);
            break;
        case(ControlLine::UNDEF):
            ctrlUndef(controlLine);
            break;
        case(ControlLine::LINE):
            ctrlLine(controlLine);
            break;
        case(ControlLine::ERROR):
            ctrlError(controlLine);
            break;
        case(ControlLine::NEW_LINE):
            ctrlNewLine(controlLine);
            break;
        
        default:
            mIsValid = false;
            unexpectedEnumerationError("ControlLine", controlLine->tag);
            break;
    }
}

void TP4::DirectiveProcessor::proc(const Group* const group)
{
    if(!mIsValid)
        return;

    for(auto&& gp : group->groupPartVec)
        proc(gp);
}

void TP4::DirectiveProcessor::proc(const GroupPart* const groupPart)
{
    if(!mIsValid)
        return;

    switch(groupPart->tag)
    {
        case(GroupPart::IF_SECTION):
            proc(groupPart->uni.ifSection);
            break;
        case(GroupPart::CONTROL_LINE):
            proc(groupPart->uni.controlLine);
            break;
        case(GroupPart::TEXT_LINE):
            proc(groupPart->uni.textLine);
            break;
        case(GroupPart::NON_DIRECTIVE):
            proc(groupPart->uni.nonDirective);
            break;
        
        default:
            mIsValid = false;
            unexpectedEnumerationError("GroupPart", groupPart->tag);
            break;
    }
}

void TP4::DirectiveProcessor::proc(const IfSection* const ifSection)
{
    if(!mIsValid)
        return;
    
    bool isConsumed = false;

    switch(ifSection->ifGroup->tag)
    {
        case(IfGroup::IF):
        {
            if(evalCondition(ifSection->ifGroup->uni.sIf.ppTokens))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uni.sIf.group != nullptr)
                    proc(ifSection->ifGroup->uni.sIf.group);
            }
            break;
        }
        case(IfGroup::IFDEF):
        {
            if(evalCondition(ifSection->ifGroup->uni.sIfdef.identifier))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uni.sIfdef.group != nullptr)
                    proc(ifSection->ifGroup->uni.sIfdef.group);
            }
            break;
        }
        case(IfGroup::IFNDEF):
        {
            if(evalCondition(ifSection->ifGroup->uni.sIfndef.identifier))
            {
                isConsumed = true;
                if(ifSection->ifGroup->uni.sIfndef.group != nullptr)
                    proc(ifSection->ifGroup->uni.sIfndef.group);
            }
            break;
        }

        default:
            mIsValid = false;
            unexpectedEnumerationError("IfGroup", ifSection->ifGroup->tag);
            break;
    }

    if(!isConsumed &&
       ifSection->elifGroups != nullptr)
    {
        for(auto&& eg : ifSection->elifGroups->elifGroupVec)
        {
            if(evalCondition(eg->ppTokens))
            {
                isConsumed = true;
                if(eg->group != nullptr)
                    proc(eg->group);
                break;
            }
        }
    }

    if(!isConsumed &&
       ifSection->elseGroup != nullptr)
    {
        isConsumed = true;
        if(ifSection->elseGroup->group != nullptr)
            proc(ifSection->elseGroup->group);
    }
}

void TP4::DirectiveProcessor::proc(const NonDirective* const nonDirective)
{
    if(!mIsValid)
        return;

    mIsValid = false;
    std::cout << "implementation error:\n"
                 "    what: non-directive is not evaluated.\n"
                 "    throw class: DirectiveProcessor for TP4"
              << std::endl;
}

void TP4::DirectiveProcessor::proc(const PreprocessingFile* const ppFile)
{
    if(!mIsValid)
        return;

    if(ppFile->group != nullptr)
        proc(ppFile->group);
}

void TP4::DirectiveProcessor::proc(const TextLine* const textLine)
{
    if(!mIsValid)
        return;
    
    if(textLine->ppTokens != nullptr)
    {
        std::vector<PreprocessingToken*> dstvec;
        replaceMacro(textLine->ppTokens->ppTokenVec, dstvec);
        mPPTokenVec.insert(mPPTokenVec.end(), dstvec.begin(), dstvec.end());
    }
}

bool TP4::DirectiveProcessor::evalCondition(const PPTokens* const ppTokens)
{
    std::vector<PreprocessingToken*> ptvec = ppTokens->ppTokenVec;

    // replace "defined identifier".
    for(std::size_t i = 0; i + 1 < ptvec.size(); i++)
    {
        if(((ptvec[i]->tag == PreprocessingToken::IDENTIFIER) ? (ptvec[i]->uni.identifier->sequence == "defined") : false) &&
           ptvec[i + 1]->tag == PreprocessingToken::IDENTIFIER)
        {
            PreprocessingToken* rep = new PreprocessingToken();
            rep->tag = PreprocessingToken::PP_NUMBER;
            rep->uni.ppNumber = new PPNumber();
            rep->uni.ppNumber->sequence = (MACRO_MAP.find(ptvec[i + 1]->uni.identifier->sequence) != MACRO_MAP.end()) ? "1" : "0";
            
            ptvec.erase(ptvec.begin() + i);
            ptvec[i] = rep;
        }
    }
    // replace "defined ( identifier )".
    for(std::size_t i = 0; i + 3 < ptvec.size(); i++)
    {
        if(((ptvec[i]->tag == PreprocessingToken::IDENTIFIER) ? (ptvec[i]->uni.identifier->sequence == "defined") : false) &&
           ((ptvec[i + 1]->tag == PreprocessingToken::PUNCTUATOR) ? (ptvec[i]->uni.punctuator->tag == Punctuator::LPAREN) : false) &&
           (ptvec[i + 2]->tag == PreprocessingToken::IDENTIFIER) &&
           ((ptvec[i + 3]->tag == PreprocessingToken::PUNCTUATOR) ? (ptvec[i + 3]->uni.punctuator->tag == Punctuator::RPAREN) : false))
        {
            PreprocessingToken* rep = new PreprocessingToken();
            rep->tag = PreprocessingToken::PP_NUMBER;
            rep->uni.ppNumber = new PPNumber();
            rep->uni.ppNumber->sequence = (MACRO_MAP.find(ptvec[i + 2]->uni.identifier->sequence) != MACRO_MAP.end()) ? "1" : "0";

            ptvec.erase(ptvec.begin() + i, ptvec.begin() + i + 3);
            ptvec[i] = rep;
        }
    }

    // replace macro
    std::vector<PreprocessingToken*> dstvec;
    replaceMacro(ptvec, dstvec);

    // all remaining identifiers are replaced with the pp-number 0
    for(auto&& pt : dstvec)
    {
        if(pt->tag == PreprocessingToken::IDENTIFIER)
        {
            PreprocessingToken* rep = new PreprocessingToken();
            rep->tag = PreprocessingToken::PP_NUMBER;
            rep->uni.ppNumber = new PPNumber();
            rep->uni.ppNumber->sequence = "0";

            pt = rep;
        }
    }

    // convert to token from preprocessing-token
    std::vector<Token*> tokenVec;
    TP7::Converter converter(dstvec, tokenVec);
    if(!converter.execute())
    {
        mIsValid = false;
        std::cout << "TP4 error:\n"
                     "    what: failed to convert to Token class from PreprocessingToken.\n"
                  << std::endl;
        return false;
    }
    
    // 
    ConstantExpression* constantExpression = nullptr;
    TP7::Tokenizer tokenizer(tokenVec);
    if(!tokenizer.execute(constantExpression))
    {
        mIsValid = false;
        std::cout << "TP4 error:\n"
                     "    what: failed to tokenize to constant-expression from Token vector."
                  << std::endl;
        return false;
    }

    // evaluate constant-expression
    Integer integer;
    Calculator calculator(constantExpression, integer);
    if(!(mIsValid = calculator.execute()))
        return false;
    
    switch(integer.tag)
    {
        case(Integer::SIGNED):
            return integer.uni.sint != 0;
        case(Integer::UNSIGNED):
            return integer.uni.uint != 0;

        default:
            mIsValid = false;
            unexpectedEnumerationError("Integer", integer.tag);
            return false;
    }
}

bool TP4::DirectiveProcessor::evalCondition(const Identifier* const identifier)
{
    return MACRO_MAP.find(identifier->sequence) != MACRO_MAP.end();
}

void TP4::DirectiveProcessor::ctrlInclude(const ControlLine* const controlLine)
{
    PPTokens pt;
    replaceMacro(controlLine->uni.sInclude.ppTokens->ppTokenVec, pt.ppTokenVec);

    std::string includeDirective = "# include ";
    for(auto&& p : pt.ppTokenVec)
        p->string(includeDirective);

    std::vector<TP3Token*> tp3vec;
    TP3::TP3 tp3(includeDirective, tp3vec);
    if(!tp3.execute())
    {
        mIsValid = false;
        std::cout << "TP4 directive-processor error:\n"
                     "    what: \n"
                  << std::flush;
        return;
    }

    unsigned int numToken = 0;
    HeaderName* hn = nullptr;
    for(std::size_t i = 4; i < tp3vec.size(); i++)
    {
        if(tp3vec[i]->tag == TP3Token::PP_TOKEN)
        {
            numToken++;
            hn = (tp3vec[i]->uni.ppToken->tag == PreprocessingToken::HEADER_NAME) ? tp3vec[i]->uni.ppToken->uni.headerName : nullptr;
        }
    }

    if(numToken != 1 || hn == nullptr)
    {
        mIsValid = false;
        std::cout << "TP4 directive-processor error:\n"
                     "    what: form of include directive is invalid.\n"
                     "    line: " << includeDirective
                  << std::endl;
        return;
    }

    std::string filename;
    std::string directory = (GLOBAL::CURRENT_FILENAME.rfind("/") != std::string::npos) ? GLOBAL::CURRENT_FILENAME.substr(0, GLOBAL::CURRENT_FILENAME.rfind("/") + 1) : "";
    switch(hn->tag)
    {
        case(HeaderName::H_CHAR_SEQUENCE):
        {
            for(auto&& p : SYSTEM_INCLUDE_PATH_VEC)
            {
                std::ifstream fstr(p + hn->sequence);
                if(fstr.is_open())
                {
                    filename = p + hn->sequence;
                    break;
                }
            }
            break;
        }
        case(HeaderName::Q_CHAR_SEQUENCE):
        {
            {
                std::ifstream fstr(directory + hn->sequence);
                if(fstr.is_open())
                    filename = directory + hn->sequence;
            }

            if(filename.empty())
            {
                for(auto&& p : SYSTEM_INCLUDE_PATH_VEC)
                {
                    std::ifstream fstr(p + hn->sequence);
                    if(fstr.is_open())
                    {
                        filename = p + hn->sequence;
                        break;
                    }
                }
            }

            break;
        }

        default:
            mIsValid = false;
            unexpectedEnumerationError("HeaderName", hn->tag);
            break;
    }
    
    if(filename.empty())
    {
        mIsValid = false;
        std::string tmp;
        hn->string(tmp);
        std::cout << "TP4 directive-processor error:\n"
                     "    what: include file cannot open.\n"
                     "    filename: " << tmp << "\n"
                     "    current file: " << GLOBAL::CURRENT_FILENAME
                   << std::endl;
        return;
    }

    std::vector<PreprocessingToken*> res;
    mIsValid = Compiler::procPhase4(filename, res);

    mPPTokenVec.insert(mPPTokenVec.end(), res.begin(), res.end());
}

void TP4::DirectiveProcessor::ctrlDefine(const ControlLine* const controlLine)
{
    Macro macro;
    macro.tag = Macro::OBJECT;
    macro.identifier = controlLine->uni.sDefineReplacementList.identifier;
    macro.replacementList = controlLine->uni.sDefineReplacementList.replacementList;
    
    insertMacro(macro);
}

void TP4::DirectiveProcessor::ctrlDefineIdentifierList(const ControlLine* const controlLine)
{
    Macro macro;
    macro.tag = Macro::FUNCTION;
    macro.identifier = controlLine->uni.sDefineIdentifierListReplacementList.identifier;
    macro.replacementList = controlLine->uni.sDefineIdentifierListReplacementList.replacementList;
    macro.argumentList = controlLine->uni.sDefineIdentifierListReplacementList.identifierList;

    insertMacro(macro);
}

void TP4::DirectiveProcessor::ctrlDefineVariable(const ControlLine* const controlLine)
{
    Macro macro;
    macro.tag = Macro::FUNCTION;
    macro.identifier = controlLine->uni.sDefineVariableReplacementList.identifier;
    macro.replacementList = controlLine->uni.sDefineVariableReplacementList.replacementList;
    macro.isVariable = true;

    insertMacro(macro);
}

void TP4::DirectiveProcessor::ctrlDefineIdentifierListVariable(const ControlLine* const controlLine)
{
    Macro macro;
    macro.tag = Macro::FUNCTION;
    macro.identifier = controlLine->uni.sDefineIdentifierListVariableReplacementList.identifier;
    macro.replacementList = controlLine->uni.sDefineIdentifierListVariableReplacementList.replacementList;
    macro.argumentList = controlLine->uni.sDefineIdentifierListVariableReplacementList.identifierList;
    macro.isVariable = true;

    insertMacro(macro);
}

void TP4::DirectiveProcessor::ctrlUndef(const ControlLine* const controlLine)
{
    auto iter = MACRO_MAP.find(controlLine->uni.sUndef.identifier->sequence);
    if(iter != MACRO_MAP.end())
        MACRO_MAP.erase(iter);
}

void TP4::DirectiveProcessor::ctrlLine(const ControlLine* const controlLine)
{
    mIsValid = false;
    std::string tmp;
    controlLine->string(tmp); 
    std::cout << "implementation error:\n"
                 "    what: line directive is not implemented.\n"
                 "    contents: " << tmp
              << std::endl;
}

void TP4::DirectiveProcessor::ctrlError(const ControlLine* const controlLine)
{
    std::string tmp;
    controlLine->string(tmp);

    mIsValid = false;
    std::cout << "TP4 DirectiveProcessor error:\n"
                 "    what: " << tmp
              << std::endl;
}

void TP4::DirectiveProcessor::replaceMacro(const std::vector<PreprocessingToken*>& srcvec,
                                           std::vector<PreprocessingToken*>& dstvec)
{
    for(std::size_t i = 0; i < srcvec.size(); i++)
    {
        auto iter = MACRO_MAP.end();
        if((srcvec[i]->tag == PreprocessingToken::IDENTIFIER) ? ((iter = MACRO_MAP.find(srcvec[i]->uni.identifier->sequence)) != MACRO_MAP.end()) : false)
        {
            std::size_t lastidx = i; 
            switch(iter->second->tag)
            {
                
                case(Macro::OBJECT):
                    lastidx = replaceObjectMacro(srcvec, i, dstvec);
                    break;
                case(Macro::FUNCTION):
                    if(i + i < srcvec.size() ? (srcvec[i]->tag == PreprocessingToken::PUNCTUATOR) : false)
                    {
                        if(srcvec[i]->uni.punctuator->tag == Punctuator::LPAREN)
                            lastidx = replaceFunctionMacro(srcvec, i, dstvec);
                    }
                    break;
                
                default:
                    mIsValid = false;
                    unexpectedEnumerationError("Macro", iter->second->tag);
                    break;
            }

            i = lastidx;
        }
        else
            dstvec.push_back(srcvec[i]);
    }
}

std::size_t TP4::DirectiveProcessor::replaceObjectMacro(const std::vector<PreprocessingToken*>& srcvec,
                                                        std::size_t idx,
                                                        std::vector<PreprocessingToken*>& dstvec)
{
    auto iter = MACRO_MAP.find(srcvec[idx]->uni.identifier->sequence);
    std::vector<TP3Token*> repvec = iter->second->replacementList->tp3Tokens->tp3TokenVec;

    // ## operator
    std::pair<const PreprocessingToken*, std::size_t> lhs(nullptr, 0), rhs(nullptr, 0);
    for(std::size_t i = 0; i < repvec.size(); i++)
    {
        if(repvec[i]->tag == TP3Token::PP_TOKEN)
        {
            if(repvec[i]->uni.ppToken->tag == PreprocessingToken::PUNCTUATOR ? (repvec[i]->uni.ppToken->uni.punctuator->tag == Punctuator::HASHHASH) : false)
            {
                for(std::size_t j = i; j >= 1; j--)
                {
                    if(repvec[j - 1]->tag == TP3Token::PP_TOKEN)
                    {
                        lhs.first = repvec[j - 1]->uni.ppToken, lhs.second = j - 1;
                        break;
                    }
                }
                for(std::size_t j = i + 1; j < repvec.size(); j++)
                {
                    if(repvec[j]->tag == TP3Token::PP_TOKEN)
                    {
                        rhs.first = repvec[j]->uni.ppToken, rhs.second = j;
                        break;
                    }
                }

                TP3Token* res = concatenateToken(lhs.first, rhs.first);
                if(res != nullptr)
                {
                    repvec.erase(repvec.begin() + lhs.second,
                                 repvec.begin() + rhs.second);
                    repvec[rhs.second] = res;
                }
            }
        }
    }

    std::vector<PreprocessingToken*> newsrcvec;
    for(auto&& tt : repvec)
    {
        if(tt->tag == TP3Token::PP_TOKEN)
            newsrcvec.push_back(tt->uni.ppToken);
    }

    replaceMacro(newsrcvec, dst);
    
    return idx;
}

std::size_t TP4::DirectiveProcessor::replaceFunctionMacro(const std::vector<PreprocessingToken*>& srcvec,
                                                          std::size_t idx,
                                                          std::vector<PreprocessingToken*>& dstvec)
{
    auto iter = MACRO_MAP.find(srcvec[idx]->uni.identifier->sequence);
    std::vector<TP3Token*> repvec = iter->second->replacementList->tp3Tokens->tp3TokenVec;

    std::vector<std::vector<PreprocessingToken*>> argsvec, repargsvec;
    unsigned int nest = 1;
    std::size_t first = idx + 2, last = idx + 2;

    // get arguments
    for(std::size_t i = idx + 2; i < srcvec.size(); i++)
    {
        if((srcvec[i]->tag == PreprocessingToken::PUNCTUATOR ? (srcvec[i]->uni.punctuator->tag == Punctuator::COMMA) : false) &&
           nest == 1)
        {
            argsvec.emplace_back(srcvec.begin() + first, srcvec.begin() + i);
            first = i + 1;
        }
        else if(srcvec[i]->tag == PreprocessingToken::PUNCTUATOR ? (srcvec[i]->uni.punctuator->tag == Punctuator::LPAREN) : false)
            nest++;
        else if(srcvec[i]->tag == PreprocessingToken::PUNCTUATOR ? (srcvec[i]->uni.punctuator->tag == Punctuator::RPAREN) : false)
        {
            nest--;
            if(nest == 0)
            {
                last = i;
                break;
            }
        }
    }

    // check number of arguments
    std::size_t numParam = 0;
    if(iter->second->argumentList != nullptr)
        numParam = iter->second->argumentList->identifierList.size();
   
    if(iter->second->isVariable ? (argsvec.size() < numParam) : (argsvec.size() != numParam))
    {
        mIsValid = false;
        std::cout << "TP4 directive-processor error:\n"
                     "    what: number of arguments of function-like macro not matched.\n"
                     "    macro-name: " << iter->first << "\n"
                  << std::flush;
        return last;
    }

    // expand arguments
    for(auto&& args : argsvec)
    {
        repargsvec.emplace_back();
        replaceMacro(args, repargsvec.back());
    }

    // concatenate __VA_ARGS__
    for(std::size_t i = numParam; i + 1 )

    // set parameter
    std::unordered_map<std::string, unsigned int> argsmap;
    if()

    // # operator
    
    for(std::size_t i = 0; i + 1 < repvec.size(); i++)
    {
        if((repvec[i]->tag == TP3Token::PP_TOKEN) ? (repvec[i]->uni.ppToken->tag == PreprocessingToken::PUNCTUATOR) : false)
        {
            if(repvec[i]->uni.ppToken->uni.punctuator->tag == Punctuator::HASH)
            {
                std::size_t argidx = 0;
                for(; argidx < iter->second->argumentList->)
            }
        }
    }
}

bool TP4::DirectiveProcessor::insertMacro(const Macro& macro)
{
    bool isValid = true;

    auto iter = MACRO_MAP.find(macro.identifier->sequence);
    if(iter == MACRO_MAP.end())
        MACRO_MAP.emplace(macro.identifier->sequence, new Macro(macro));
    else if(macro != (*iter->second))
    {
        mIsValid = false;
        isValid = false;
        std::cout << "TP4 directive-processor error:\n"
                     "    what: macro define directive is redefined.\n"
                     "    macro name: " << iter->first
                  << std::endl;
    }

    return isValid;
}