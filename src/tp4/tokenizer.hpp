#pragma once

#include <vector>

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
public:
    Tokenizer(const std::vector<TP3Token*>&,
              PreprocessingFile*&);

    bool execute();

private:
    ControlLine* getControlLine(){return nullptr;}
    ElifGroups* getElifGroups(){return nullptr;}
    ElseGroup* getElseGroup(){return nullptr;}
    Group* getGroup();
    GroupPart* getGroupPart();
    IfGroup* getIfGroup(){return nullptr;}
    IfSection* getIfSection();
    NonDirective* getNonDirective(){return nullptr;}
    PreprocessingFile* getPreprocessingFile();
    TextLine* getTextLine(){return nullptr;}

    std::size_t nextIdx(std::size_t) const;
    TP3Token* getTP3Token(std::size_t) const;

    const std::vector<TP3Token*>& mTP3TokenVec;
    PreprocessingFile*& mPPFile;
    std::size_t mIdx;

    bool mIsValid;
};