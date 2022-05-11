#pragma once

#include <vector>

struct Token;
struct PreprocessingToken;

namespace TP7
{

class Converter
{
public:
    Converter(const std::vector<PreprocessingToken*>&,
              std::vector<Token*>&) noexcept(true);

    bool execute(){return mIsValid;}

private:
    const std::vector<PreprocessingToken*>& mPPTokenVec;
    std::vector<Token*> mTokenVec;

    bool mIsValid;
};

}