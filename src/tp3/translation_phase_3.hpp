#pragma once

#include <string>
#include <vector>

struct TP3Token;

class TP3
{
public:
    TP3(const std::string& src,
        std::vector<TP3Token*>& tp3TokenVec);

    bool execute();

private:
    const std::string& mSrc;
    std::size_t mIdx;
    std::vector<TP3Token*>& mTP3TokenVec;

    bool mIsValid = true;
};