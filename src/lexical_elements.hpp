#pragma once

#include <string>
#include <vector>
#include <iostream>

#define M_CREATE_TAG(...) \
    enum Tag : int\
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
        enum \
        union \
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

#define M_CREATE_TOKEN_STRUCT_ANY_MEMBER(class_name, elements, ...) \
    struct class_name : public BaseToken\
    {\
        elements\
        class_name():\
            BaseToken(),\
            __VA_ARGS__{}\
    };

#define M_VARIABLE_MACRO(...) \
    __VA_ARGS__

struct BaseToken;
struct TP3Token;

struct CChar;
struct CCharSequence;
struct CharacterConstant;
struct EscapeSequence;
struct HeaderName;
struct HexadecimalEscapeSequence;
struct Identifier;
struct IdentifierList;
struct OctalEscapeSequence;
struct PPNumber;
struct PreprocessingToken;
struct Punctuator;
struct SChar;
struct SCharSequence;
struct SimpleEscapeSequence;
struct StringLiteral;

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

template<typename Tag>
bool unexpectedEnumerationError(const char* class_name, Tag tag)
{
    std::cout << "implementation error:\n"
                 "    what: unexpected enumration.\n"
                 "    class: " << class_name << "\n"
                 "    tag: " << tag
              << std::endl;
    return false;
}

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
                             CChar*,
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
                 HEXADECIMAL_ESCAPE_SEQUENCE),
    M_CREATE_UNION(simpleEscapeSequence(nullptr),
                   SimpleEscapeSequence* simpleEscapeSequence;
                   OctalEscapeSequence* octalEscapeSequence;
                   HexadecimalEscapeSequence* hexadecimalEscapeSequence;)
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
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    HexadecimalEscapeSequence,
    unsigned char value;,
    value(0)
)

// Identifier
M_CREATE_TOKEN_STRUCT_STRING(Identifier)

// OctalEscapeSequence
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    OctalEscapeSequence,
    unsigned char value;,
    value(0)
)

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
    M_CREATE_UNION(headerName(nullptr),
                   HeaderName* headerName;
                   Identifier* identifier;
                   PPNumber* ppNumber;
                   CharacterConstant* characterConstant;
                   StringLiteral* stringLiteral;
                   Punctuator* punctuator;
                   char other;)
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
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    StringLiteral,
    SCharSequence* sCharSequence;,
    sCharSequence(nullptr)
)

// ControlLine
M_CREATE_TOKEN_STRUCT
(
    ControlLine,
    M_CREATE_TAG(INCLUDE,
                 DEFINE_REPLACEMENT_LIST,
                 DEFINE_IDENTIFIER_LIST_REPLACEMENT_LIST,
                 DEFINE_VARIABLE_REPLACEMENT_LIST,
                 DEFINE_IDENTIFIER_LIST_VARIABLE_REPLACEMENT_LIST,
                 UNDEF,
                 LINE,
                 ERROR,
                 NEW_LINE),
    M_CREATE_UNION(sInclude{nullptr},
                   M_CREATE_UNION_MEMBER(Include,
                                         PPTokens* ppTokens;)
                   M_CREATE_UNION_MEMBER(DefineReplacementList,
                                         Identifier* identifier;
                                         ReplacementList* replacementList;)
                   M_CREATE_UNION_MEMBER(DefineIdentifierListReplacementList,
                                         Identifier* identifier;
                                         IdentifierList* identifierList;
                                         ReplacementList* replacementList;)
                   M_CREATE_UNION_MEMBER(DefineVariableReplacementList,
                                         Identifier* identifier;
                                         ReplacementList* replacementList;)
                   M_CREATE_UNION_MEMBER(DefineIdentifierListVariableReplacementList,
                                         Identifier* identifier;
                                         IdentifierList* identifierList;
                                         ReplacementList* replacementList;)
                   M_CREATE_UNION_MEMBER(Undef,
                                         Identifier* identifier;)
                   M_CREATE_UNION_MEMBER(Line,
                                         PPTokens* ppTokens;)
                   M_CREATE_UNION_MEMBER(Error,
                                         PPTokens* ppTokens;))
)

// ElifGroup
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    ElifGroup,
    PPTokens* ppTokens;
    Group* group;,
    ppTokens(nullptr),
    group(nullptr)
)

// ElifGroups
M_CREATE_TOKEN_STRUCT_VECTOR
(
    ElifGroups,
    ElifGroup*,
    elifGroupVec
)

// ElseGroup
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    ElseGroup,
    Group* group;,
    group(nullptr)
)

// Group
M_CREATE_TOKEN_STRUCT_VECTOR
(
    Group,
    GroupPart*,
    groupPartVec
)

// GroupPart
M_CREATE_TOKEN_STRUCT
(
    GroupPart,
    M_CREATE_TAG(IF_SECTION,
                 CONTROL_LINE,
                 TEXT_LINE,
                 NON_DIRECTIVE),
    M_CREATE_UNION(ifSection(nullptr),
                   IfSection* ifSection;
                   ControlLine* controlLine;
                   TextLine* textLine;
                   NonDirective* nonDirective;)
)

// IfGroup
M_CREATE_TOKEN_STRUCT
(
    IfGroup,
    M_CREATE_TAG(IF,
                 IFDEF,
                 IFNDEF),
    M_CREATE_UNION(M_VARIABLE_MACRO(sIf{nullptr, nullptr}),
                   M_CREATE_UNION_MEMBER(If,
                                         PPTokens* ppTokens;
                                         Group* group;)
                   M_CREATE_UNION_MEMBER(Ifdef,
                                         Identifier* identifier;
                                         Group* group;)
                   M_CREATE_UNION_MEMBER(Ifndef,
                                         Identifier* identifier;
                                         Group* group;))
)

// IfSection
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    IfSection,
    IfGroup* ifGroup;
    ElifGroups* elifGroups;
    ElseGroup* elseGroup;,
    ifGroup(nullptr),
    elifGroups(nullptr),
    elseGroup(nullptr)
)

// NonDirective
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    NonDirective,
    PPTokens* ppTokens;,
    ppTokens(nullptr)
)

// PPTokens
M_CREATE_TOKEN_STRUCT_VECTOR
(
    PPTokens,
    TP3Token*,
    ppTokenVec
)

// PreprocessingFile
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    PreprocessingFile,
    Group* group;,
    group(nullptr)
)

// ReplacementList
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    ReplacementList,
    PPTokens* ppTokens;,
    ppTokens(nullptr)
)

// TextLine
M_CREATE_TOKEN_STRUCT_ANY_MEMBER
(
    TextLine,
    PPTokens* ppTokens;,
    ppTokens(nullptr)
)