#pragma once

#include <string>
#include <vector>

struct TP3Token;

struct PreprocessingToken;

class TP3
{
public:
    TP3(const std::string& src,
        std::vector<TP3Token*>& tp3TokenVec);

    bool execute();

private:
    TP3Token* getTP3Token();
    unsigned int getWhiteSpace();
    PreprocessingToken* getPPToken(){return nullptr;}    
    

    const std::string& mSrc;
    std::size_t mIdx;
    std::vector<TP3Token*>& mTP3TokenVec;

    bool mIsValid = true;
};