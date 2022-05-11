#pragma once

#include <string>
#include <vector>

struct ReplacementList;
struct IdentifierList;
struct Identifier;

namespace TP4
{

struct Macro
{
    enum Tag
    {
        NONE,
        OBJECT,
        FUNCTION
    } tag;

    Identifier* identifier;
    ReplacementList* replacementList;
    IdentifierList* argumentList;
    bool isVariable;

    Macro():
        tag(NONE),
        identifier(nullptr),
        replacementList(nullptr),
        argumentList(nullptr),
        isVariable(false){}

    friend bool operator==(const Macro&, const Macro&);
    friend bool operator!=(const Macro&, const Macro&);
};

}