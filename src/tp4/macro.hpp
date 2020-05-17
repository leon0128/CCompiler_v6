#pragma once

#include <string>
#include <vector>

struct ReplacementList;
struct IdentifierList;
struct Identifier;

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
};