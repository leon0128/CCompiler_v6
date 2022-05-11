#pragma once

#include <cstdint>

namespace TP4
{

struct Integer
{
public:
    enum Tag
    {
        NONE,
        SIGNED,
        UNSIGNED
    } tag;

    union Uni
    {
        std::intmax_t sint;
        std::uintmax_t uint;
    } uni;

    Integer():
        tag(NONE),
        uni(){}
};

}