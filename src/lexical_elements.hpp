#pragma once

#include <string>
#include <vector>

#define M_CREATE_TAG(...) \
    enum Tag\
    {\
        NONE,\
        __VA_ARGS__\
    } tag;

#define M_CREATE_UNION_MEMBER(base_name, ...) \
    struct S ## base_name\
    {\
        __VA_ARGS__\
    } s ## base_name;

#define M_CREATE_UNION(init, ...) \
    union Uni\
    {\
        __VA_ARGS__\
        Uni():\
            init{}\
    } uni;

#define M_CREATE_TOKEN_STRUCT(name, enum, union) \
    struct name : BaseToken\
    {\
        enum\
        union\
        name():\
            BaseToken(),\
            tag(NONE),\
            uni(){}\
    };

#define M_CREATE_TOKEN_STRUCT_VECTOR(class_name, ele_name, mem_name) \
    struct class_name : public BaseToken\
    {\
        std::vector<ele_name> mem_name;\
        class_name():\
            BaseToken(),\
            mem_name(){}\
    };

#define M_CREATE_TOKEN_STRUCT_STRING(class_name) \
    struct class_name : public BaseToken\
    {\
        std::string sequence;\
        class_name():\
            BaseToken(),\
            sequence(){}\
    };

#define M_CREATE_TOKEN_STRUCT_TAG(class_name, enum) \
    struct class_name : public BaseToken\
    {\
        enum\
        class_name():\
            BaseToken(),\
            tag(NONE){}\
    };

struct BaseToken;
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
struct UniversalCharacterName;

struct BaseToken
{
private:
    //static std::vector<BaseToken*> ALLOC_BASE_TOKEN_VEC;

public:
    virtual ~BaseToken() = default;
};

// TP3Token
M_CREATE_TOKEN_STRUCT
(
    TP3Token,
    M_CREATE_TAG(PP_TOKEN,
                 WHITE_SPACE),
    M_CREATE_UNION(ppToken(nullptr),
                   PreprocessingToken* ppToken;
                   unsigned int numSpace;)
)

// CChar
M_CREATE_TOKEN_STRUCT
(
    CChar,
    M_CREATE_TAG(ANY_MEMBER,
                 ESCAPE_SEQUENCE),
    M_CREATE_UNION(anyMember(0),
                   char anyMember;
                   EscapeSequence* escapeSequence;)
)

// CCharSequence
M_CREATE_TOKEN_STRUCT_VECTOR(CCharSequence,
                             CChar,
                             cCharVec)

// CharacterConstant
struct CharacterConstant : public BaseToken
{
    M_CREATE_TAG(C_CHAR_SEQUENCE,
                 L_C_CHAR_SEQUENCE,
                 u_C_CHAR_SEQUENCE,
                 U_C_CHAR_SEQUENCE)
    
    CCharSequence* cCharSequence;

    CharacterConstant():
        BaseToken(),
        tag(NONE),
        cCharSequence(nullptr){}
};

// EscapeSequence
M_CREATE_TOKEN_STRUCT
(
    EscapeSequence,
    M_CREATE_TAG(SIMPLE_ESCAPE_SEQUENCE,
                 OCTAL_ESCAPE_SEQUENCE,
                 HEXADECIMAL_ESCAPE_SEQUENCE,
                 UNIVERSAL_CHARACTER_NAME),
    M_CREATE_UNION(simpleEscapeSequence(nullptr),
                   SimpleEscapeSequence* simpleEscapeSequence;
                   OctalEscapeSequence* octalEscapeSequence;
                   HexadecimalEscapeSequence* hexadecimalEscapeSequence;
                   UniversalCharacterName* universalCharacterName;)
)

// HeaderName
struct HeaderName : public BaseToken
{
    M_CREATE_TAG(H_CHAR_SEQUENCE,
                 Q_CHAR_SEQUENCE)

    std::string sequence;

    HeaderName():
        BaseToken(),
        tag(NONE),
        sequence(){}
};

// HexadecimalEscapeSequence
M_CREATE_TOKEN_STRUCT_STRING(HexadecimalEscapeSequence)

// Identifier
M_CREATE_TOKEN_STRUCT_STRING(Identifier)

// OctalEscapeSequence
M_CREATE_TOKEN_STRUCT_STRING(OctalEscapeSequence)

// PPNumber
M_CREATE_TOKEN_STRUCT_STRING(PPNumber)

// PreprocessingToken
M_CREATE_TOKEN_STRUCT
(
    PreprocessingToken,
    M_CREATE_TAG(HEADER_NAME,
                 IDENTIFIER,
                 PP_NUMBER,
                 CHARACTER_CONSTANT,
                 STRING_LITERAL,
                 PUNCTUATOR,
                 OTHER),
    M_CREATE_UNION(sHeaderName{nullptr},
                   M_CREATE_UNION_MEMBER(HeaderName,
                                         HeaderName* headerName;)
                   M_CREATE_UNION_MEMBER(Identifier,
                                         Identifier* identifier;)
                   M_CREATE_UNION_MEMBER(PPNumber,
                                         PPNumber* ppNumber;)
                   M_CREATE_UNION_MEMBER(CharacterConstant,
                                         CharacterConstant* characterConstant;)
                   M_CREATE_UNION_MEMBER(StringLiteral,
                                         StringLiteral* stringLiteral;)
                   M_CREATE_UNION_MEMBER(Punctuator,
                                         Punctuator* punctuator;)
                   M_CREATE_UNION_MEMBER(Other,
                                         char c;))
)

// Punctuator
M_CREATE_TOKEN_STRUCT_TAG
(
    Punctuator,
    M_CREATE_TAG(LSQUALE_PAREN,
                 RSQUALE_PAREN,
                 LPAREN,
                 RPAREN,
                 LBLOCK_PAREN,
                 RBLOCK_PAREN,
                 DOT,
                 ARROW,
                 INCLEMENT,
                 DECLEMENT,
                 BITWISE_AND,
                 ASTERISK,
                 PLUS,
                 MINUS,
                 TILDE,
                 NOT,
                 SLASH,
                 PERCENT,
                 LSHIFT,
                 RSHIFT,
                 LESS,
                 GREATER,
                 LESS_EQUAL,
                 GREATER_EQUAL,
                 EQUAL,
                 NOT_EQUAL,
                 BITWISE_EXCLUSIVE_OR,
                 BITWISE_INCLUSIVE_OR,
                 LOGICAL_AND,
                 LOGICAL_OR,
                 QUESTION,
                 COLON,
                 SEMICOLON,
                 DOTDOTDOT,
                 ASSIGN,
                 ASTERISK_ASSIGN,
                 SLASH_ASSIGN,
                 PERCENT_ASSIGN,
                 PLUS_ASSING,
                 MINUS_ASSIGN,
                 LSHIFT_ASSIGN,
                 RSHIFT_ASSIGN,
                 BITWISE_AND_ASSIGN,
                 BITWISE_EXCLUSIVE_OR_ASSING,
                 BITWISE_INCLUSIVE_OR_ASSING,
                 COMMA,
                 HASH,
                 HASHHASH)
)

// SChar
M_CREATE_TOKEN_STRUCT
(
    SChar,
    M_CREATE_TAG(ANY_MEMBER,
                 ESCAPE_SEQUENCE),
    M_CREATE_UNION(anyMember(0),
                   char anyMember;
                   EscapeSequence* escapeSequence;)
)

// SCharSequence
M_CREATE_TOKEN_STRUCT_VECTOR(SCharSequence,
                             SChar*,
                             sCharVec)

// SimpleEscapeSequence
M_CREATE_TOKEN_STRUCT_TAG
(
    SimpleEscapeSequence,
    M_CREATE_TAG(SINGLE_QUOTATION,
                 DOUBLE_QUOTATION,
                 QUESTION,
                 BACKSLASH,
                 ALERT,
                 BACKSPACE,
                 FORM_FEED,
                 NEW_LINE,
                 CARRIAGE_RETURN,
                 HORIZONTAL_TAB,
                 VERTICAL_TAB)
)

// StringLiteral
struct StringLiteral : public BaseToken
{
    M_CREATE_TAG(u8,
                 u,
                 U,
                 L)
    
    SCharSequence* sCharSequence;

    StringLiteral():
        BaseToken(),
        tag(NONE),
        sCharSequence(nullptr){}
};