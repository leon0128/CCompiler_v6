#pragma once

#include <cstdlib>
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

struct TP3Node;

struct BaseToken;

struct PreprocessingToken;

struct BaseToken
{
private:
    //static std::vector<BaseToken*> ALLOC_BASE_TOKEN_VEC;

public:
    virtual BaseToken~();
};

