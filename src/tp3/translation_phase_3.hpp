#pragma once

#include <string>
#include <vector>
#include <unordered_map>

struct TP3Token;

struct CChar;
struct CCharSequence;
struct CharacterConstant;
struct EscapeSequence;
struct HeaderName;
struct HexadecimalEscapeSequence;
struct Identifier;
struct OctalEscapeSequence;
struct PPNumber;
struct PreprocessingToken;
struct Punctuator;
struct SChar;
struct SCharSequence;
struct SimpleEscapeSequence;
struct StringLiteral;

class TP3
{
private:
    static unsigned int NUM_EXEC;
    static const std::unordered_map<std::string, int> PUNCTUATOR_MAP;
    static const std::unordered_map<char, int> ESCAPE_SEQUENCE_MAP;

public:
    TP3(const std::string& src,
        std::vector<TP3Token*>& tp3TokenVec);

    bool execute();

private:
    TP3Token* getTP3Token();
    unsigned int getWhiteSpace();
    CChar* getCChar();
    CCharSequence* getCCharSequence();
    CharacterConstant* getCharacterConstant();
    EscapeSequence* getEscapeSequence();
    HeaderName* getHeaderName();
    HexadecimalEscapeSequence* getHexadecimalEscapeSequence();
    Identifier* getIdentifier();
    OctalEscapeSequence* getOctalEscapeSequence();
    PPNumber* getPPNumber();
    PreprocessingToken* getPPToken();
    Punctuator* getPunctuator();
    SChar* getSChar();
    SCharSequence* getSCharSequence();
    SimpleEscapeSequence* getSimpleEscapeSequence();
    StringLiteral* getStringLiteral();

    char getChar(std::size_t idx) const {return idx < mSrc.size() ? mSrc[idx] : 0;}

    void outputResult() const;

    const std::string& mSrc;
    std::size_t mIdx;
    std::vector<TP3Token*>& mTP3TokenVec;

    bool mIsValid = true;
};