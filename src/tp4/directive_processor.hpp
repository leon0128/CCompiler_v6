#pragma once

#include <vector>
#include <unordered_map>

struct TP3Token;
struct Identifier;
struct IdentifierList;
struct PreprocessingToken;

struct ControlLine;
struct ElifGroup;
struct ElifGroups;
struct ElseGroup;
struct Group;
struct GroupPart;
struct IfGroup;
struct IfSection;
struct NonDirective;
struct PPTokens;
struct PreprocessingFile;
struct ReplacementList;
struct TP3Tokens;
struct TextLine;

namespace TP4
{

struct Macro;

class DirectiveProcessor
{
private:
    static std::unordered_map<std::string, class Macro*> MACRO_MAP;
    static const std::vector<std::string> SYSTEM_INCLUDE_PATH_VEC;

public:
    DirectiveProcessor(const PreprocessingFile* const,
                       std::vector<PreprocessingToken*>&) noexcept(true);

    bool execute();

private:
    void proc(const ControlLine* const);
    void proc(const Group* const);
    void proc(const GroupPart* const);
    void proc(const IfSection* const);
    void proc(const NonDirective* const);
    void proc(const PreprocessingFile* const);
    void proc(const TextLine* const);

    bool evalCondition(const PPTokens* const);
    bool evalCondition(const Identifier* const);

    void ctrlInclude(const ControlLine* const);
    void ctrlDefine(const ControlLine* const);
    void ctrlDefineIdentifierList(const ControlLine* const);
    void ctrlDefineVariable(const ControlLine* const);
    void ctrlDefineIdentifierListVariable(const ControlLine* const);
    void ctrlUndef(const ControlLine* const);
    void ctrlLine(const ControlLine* const);
    void ctrlError(const ControlLine* const);
    void ctrlNewLine(const ControlLine* const){}

    void replaceMacro(const std::vector<PreprocessingToken*>& src,
                      std::vector<PreprocessingToken*>& dst);
    std::size_t replaceObjectMacro(const std::vector<PreprocessingToken*>& src,
                                   std::size_t idx,
                                   std::vector<PreprocessingToken*>& dst);
    std::size_t replaceFunctionMacro(const std::vector<PreprocessingToken*>& src,
                                     std::size_t idx,
                                     std::vector<PreprocessingToken*>& dst);
    TP3Token* concatenateToken(const PreprocessingToken* const,
                               const PreprocessingToken* const);
    TP3Token* stringnizeToken(const PreprocessingToken* const);
    bool insertMacro(const Macro&);

    const PreprocessingFile* const mPPFile;
    std::vector<PreprocessingToken*>& mPPTokenVec;

    std::unordered_map<std::string, int> mUsedMacro;

    bool mIsValid;
};

}