#include "calculator.hpp"

TP4::Calculator::Calculator(const ConstantExpression* const ce,
                            Integer& i) noexcept(true):
    mConstantExpression(ce),
    mInteger(i),
    mIsValid(true)
{
}