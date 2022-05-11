#pragma once

#include "integer.hpp"

struct ConstantExpression;

namespace TP4
{

class Calculator
{
public:
    Calculator(const ConstantExpression* const,
               Integer&) noexcept(true);

    bool execute(){return mIsValid;}

private:
    const ConstantExpression* const mConstantExpression;
    Integer& mInteger;

    bool mIsValid;
};

}