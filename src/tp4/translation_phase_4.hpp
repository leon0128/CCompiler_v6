#pragma once

#include <vector>

struct TP3Token;
struct PreprocessingToken;
struct PreprocessingFile;

namespace TP4
{

class TP4
{
public:
    TP4(const std::vector<TP3Token*>&,
        std::vector<PreprocessingToken*>&);

    bool execute();

private:
    const std::vector<TP3Token*>& mTP3TokenVec;
    std::vector<PreprocessingToken*>& mPPTokenVec;
    PreprocessingFile* mPPFile;

    bool mIsValid;
};

}