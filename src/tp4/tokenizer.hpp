#pragma once

#include <vector>
#include <string>
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
struct TextLine;

class Tokenizer
{
private:
    static const std::unordered_map<std::string, int> DIRECTIVE_NAME_MAP;

public:
    Tokenizer(const std::vector<TP3Token*>&,
              PreprocessingFile*&);

    bool execute();

private:
    ControlLine* getControlLine();
    ElifGroup* getElifGroup();
    ElifGroups* getElifGroups();
    ElseGroup* getElseGroup();
    Group* getGroup();
    GroupPart* getGroupPart();
    IdentifierList* getIdentifierList();
    IfGroup* getIfGroup();
    IfSection* getIfSection();
    NonDirective* getNonDirective();
    PPTokens* getPPTokens();
    PreprocessingFile* getPreprocessingFile();
    ReplacementList* getReplacementList();
    TextLine* getTextLine();

    bool isPunctuator(std::size_t idx, int tag) const;
    bool isIdentifier(std::size_t idx, std::string ident = std::string()) const;
    bool isOther(std::size_t idx, char c = 0) const;

    std::size_t nextIdx(std::size_t) const;
    TP3Token* getTP3Token(std::size_t) const;

    const std::vector<TP3Token*>& mTP3TokenVec;
    PreprocessingFile*& mPPFile;
    std::size_t mIdx;

    bool mIsValid;
};